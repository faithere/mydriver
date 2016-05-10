#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/mm.h>   /********for flag GFP_KERNEL**********/
#include <linux/semaphore.h>
#include <../arch/powerpc/include/asm/uaccess.h>
#include <linux/version.h>

#include "fc1553.h"
#include "xdma_hw.h"
#include "xstatus.h"

#define DMA_BD_CNT 3999
#define MAX_POOL 10

int major=FC1553_MAJOR;

int minor=FC1553_MINOR;

/*******************************global variable for DMA***********************************/
PktBuf pktArray[MAX_POOL][DMA_BD_CNT];
struct PktPool pktPool[MAX_POOL];
struct PktPool *pktPoolHead=NULL;
struct PktPool *pktPoolTail=NULL;

struct privData *dmaData=NULL;
u32 DriverState = UNINITIALIZED;

/*****************************************************************************************/

/*******************************external function*****************************************/


/*****************************************************************************************/

/**********************************type-defination****************************************/
struct fc1553_pcie_card 
{
  struct pci_bar resource;
  
  u32 int_num;
  void __iomem *addr_map;
  u32 *bar0_start;
  u32 *mem_buf;
  
  struct pci_dev *pdev;
  struct semaphore sem;
  struct cdev cdev;
};
/******************************************************************************************/

module_param(major,int,S_IRUGO);
module_param(minor,int,S_IRUGO);

/*struct fc1553_pcie_card fc1553_card;*/
struct fc1553_pcie_card *pfc1553_card;

static struct pci_device_id fc1553_pci_tbl[] __initdata={
  {PCI_DEVICE(FC1553_VENDOR,FC1553_DEVICE),},
  {0,},
};

MODULE_DEVICE_TABLE(pci,fc1553_pci_tbl);



/***************************methond for fc1553*******************************/
static int fc1553_open(struct inode *inode, struct file *flip)
{
  /*
  unsigned short temp;
  */
  struct fc1553_pcie_card *dev;
  dev=container_of(inode->i_cdev,struct fc1553_pcie_card,cdev);
  flip->private_data=dev;
  /***************************request for IRQ**************************/
 
  /********************************************************************
    test
  printk(KERN_INFO "fc1553_pcie_card open!\n");
  pci_read_config_word(dev->pdev,0x0,&temp);
  printk(KERN_INFO "Vendor ID=0x%x\n",temp);
  return OK;
    end
  ********************************************************************/
  return OK;
}

static ssize_t fc1553_read(struct file *flip, char __user *buff, size_t count, loff_t *offp)
{
  ssize_t result;
  
  struct fc1553_pcie_card *pcard=flip->private_data;
  
  u8 *addr=pcard->bar0_start;
  addr+=flip->f_pos;
  if (down_interruptible(&(pcard->sem)))
    return -ERESTARTSYS;

  //printk(KERN_INFO "pcard->bar0_start=0x%x,actual read address is 0x%x\n",pcard->bar0_start,(u32)addr);

  *(pcard->mem_buf)=ioread32(addr);

  // printk(KERN_INFO "read executed! read_val=0x%x\n",temp);

  count=sizeof(pcard->mem_buf);
  if (copy_to_user(buff,pcard->mem_buf,count))
    {
      printk(KERN_INFO "read error!\n");
      result=-EFAULT;
      goto out;
    }
  result=count;
 out:
  up(&(pcard->sem));
  return result;
}

static ssize_t fc1553_write(struct file *flip, const char __user *buff, size_t count, loff_t *offp)
{
  ssize_t result;
  
  struct fc1553_pcie_card *pcard=flip->private_data;
  
  u8 *addr=pcard->bar0_start;
  addr+=flip->f_pos;

  if (down_interruptible(&(pcard->sem)))
    return -ERESTARTSYS;
  
  count=sizeof(*(pcard->mem_buf));

  if (copy_from_user(pcard->mem_buf,buff,count))
    {
      printk(KERN_INFO "write error!\n");
      result=-EFAULT;
      goto out;
    }
  /*
  printk(KERN_INFO "value of offset=0x%x\n",pcard->buf_write->offset);
  printk(KERN_INFO "value of flip->f_ops=0x%x\n",flip->f_ops);
  */
  
  iowrite32(*(pcard->mem_buf),addr);
  result=count;
  
 out:
  up(&(pcard->sem));
  return result;
}

static loff_t fc1553_llseek(struct file *flip,loff_t off, int whence)
{
  struct fc1553_pcie_card *pcard=flip->private_data;
  loff_t new_pos;

  switch (whence)
    {
    case 0:  /*SEEK_SET*/
      new_pos=off;
      break;
    default: /*DO NOT OCCER*/
      return -EINVAL;
    }
  if (new_pos < 0)
    return -EINVAL;
  flip->f_pos=new_pos;
  /*
  printk(KERN_INFO "flip->f_ops=0x%x\n",new_pos);
  */
  return new_pos;
}

static int fc1553_release(struct inode *inode,struct file *flip)
{
  //pci_iounmap(flip->private_data->pdev,(flip->private_data->pdev.bar0_start)-0x18000);
  
}

/*character fc1553 interface*/
static struct file_operations fc1553_fops={
  owner: THIS_MODULE,
  read: fc1553_read,
  write: fc1553_write,
  open: fc1553_open,
  release: fc1553_release,
  llseek: fc1553_llseek
};

/*remove function*/
static void fc1553_pci_remove(struct pci_dev *dev)
{
  pci_release_region(pfc1553_card->pdev,0);
  pci_iounmap(pfc1553_card->pdev,pfc1553_card->addr_map);
  /*
  printk(KERN_INFO "pci remove called!\n");
  */
}
/********************************************DMA Controller Function***********************************/
/*****************************************************************************/
/**
* Reset the DMA engine.
*
* Should not be invoked during initialization stage because hardware has
* just come out of a system reset. Should be invoked during shutdown stage.
*
* New BD fetches will stop immediately. Reset will be completed once the
* user logic completes its reset. DMA disable will be completed when the
* BDs already being processed are completed.
*
* @param  InstancePtr is a pointer to the DMA engine instance to be worked on.
*
* @return None.
*
* @note
*   - If the hardware is not working properly, and the self-clearing reset
*     bits do not clear, this function will be terminated after a timeout.
*
******************************************************************************/
void Dma_Reset(Dma_Engine * InstancePtr)
{
  Dma_BdRing *RingPtr;
  int i=0;
  u32 dirqval;
  
  log_verbose(KERN_INFO "Resetting DMA instance %p\n", InstancePtr);
  
  RingPtr = &Dma_mGetRing(InstancePtr);
 
 
  /* Disable engine interrupts before issuing software reset */
  Dma_mEngIntDisable(InstancePtr);

  /* Start reset process then wait for completion. Disable DMA and
   * assert reset request at the same time. This causes user logic to
   * be reset.
   */
  log_verbose(KERN_INFO "Disabling DMA. User reset request.\n");
  i=0;
  Dma_mSetCrSr(InstancePtr, (DMA_ENG_DISABLE|DMA_ENG_USER_RESET));
  
  /* Loop until the reset is done. The bits will self-clear. */
  while (Dma_mGetCrSr(InstancePtr) & (DMA_ENG_STATE_MASK|DMA_ENG_USER_RESET)) 
    {
      i++;
      if(i >= 100000)
        {
	  printk(KERN_INFO "CR is now 0x%x\n", Dma_mGetCrSr(InstancePtr));
	  break;
        }
    }

  /* Now reset the DMA engine, and wait for its completion. */
  log_verbose(KERN_INFO "DMA reset request.\n");
  i=0;
  Dma_mSetCrSr(InstancePtr, (DMA_ENG_RESET));

  /* Loop until the reset is done. The bit will self-clear. */
  while (Dma_mGetCrSr(InstancePtr) & DMA_ENG_RESET) 
    {
      i++;
      if(i >= 100000)
        {
	  printk(KERN_INFO "CR is now 0x%x\n", Dma_mGetCrSr(InstancePtr));
	  break;
        }
    }
  
  /* Clear Interrupt register. Not doing so may cause interrupts
   * to be asserted after the engine reset if there is any
   * interrupt left over from before.
   */
  dirqval = Dma_mGetCrSr(InstancePtr);
  printk("While resetting engine, got %x in eng status reg\n", dirqval);
  if(dirqval & DMA_ENG_INT_ACTIVE_MASK)
    Dma_mEngIntAck(InstancePtr, (dirqval & DMA_ENG_ALLINT_MASK));

  RingPtr->RunState = XST_DMA_SG_IS_STOPPED;
}


/*****************************************************************************/
/**
 * This function initializes a DMA engine.  This function must be called
 * prior to using the DMA engine. Initialization of an engine includes setting
 * up the register base address, setting up the instance data, and ensuring the
 * hardware is in a quiescent state.
 *
 * @param  InstancePtr is a pointer to the DMA engine instance to be worked on.
 * @param  BaseAddress is where the registers for this engine can be found.
 *
 * @return None.
 *
 *****************************************************************************/
void Dma_Initialize(Dma_Engine * InstancePtr, u32 *BaseAddress, u32 Type)
{
  log_verbose(KERN_INFO "Initializing DMA()\n");
  
  /* Set up the instance */
  log_verbose(KERN_INFO "Clearing DMA instance %p\n", InstancePtr);
  memset(InstancePtr, 0, sizeof(Dma_Engine));
  
  log_verbose(KERN_INFO "DMA base address is 0x%x\n", BaseAddress);
  InstancePtr->RegBase = BaseAddress;
  InstancePtr->Type = Type;

  /* Initialize the engine and ring states. */
  InstancePtr->BdRing.RunState = XST_DMA_SG_IS_STOPPED;
  InstancePtr->EngineState = INITIALIZED;
  
  /* Initialize the ring structure */
  InstancePtr->BdRing.ChanBase = BaseAddress;
  if(Type == DMA_ENG_C2S)
    InstancePtr->BdRing.IsRxChannel = 1;
  else
    InstancePtr->BdRing.IsRxChannel = 0;
  
  /* Reset the device and return */
  Dma_Reset(InstancePtr);
}

/*
 *Get Configuration of DMA Engine
 */
static void ReadDMAEngineConfiguration(struct pci_dev * pdev, struct privData * dmaInfo)
{
  u8* base;
  u32 offset;
  u32 val, type, dirn, num, bc;
  int i;
  Dma_Engine * eptr;

  /* DMA registers are in BAR0 */
  base = (dmaInfo->barInfo[0].baseVAddr);
  
  printk(KERN_INFO "Hardware design version 0x%x\n", XIo_In32(base+0x8000));
  
  /* Walk through the capability register of all DMA engines */
  for(offset = DMA_OFFSET, i=0; offset < DMA_SIZE; offset += DMA_ENGINE_PER_SIZE, i++)
    {
      log_verbose(KERN_INFO "Reading engine capability from 0x%x\n",(base+offset+REG_DMA_ENG_CAP));
      val = Dma_mReadReg((base+offset), REG_DMA_ENG_CAP);
      log_verbose(KERN_INFO "REG_DMA_ENG_CAP returned 0x%x\n", val);
      
      if(val & DMA_ENG_PRESENT_MASK)
        {
	  log_verbose(KERN_INFO "Engine capability is %x\n", val);
	  eptr = &(dmaInfo->Dma[i]);
	  
	  log_verbose(KERN_INFO "DMA Engine present at offset %x: ", offset);

	  dirn = (val & DMA_ENG_DIRECTION_MASK);
	  if(dirn == DMA_ENG_C2S)
	    printk("C2S, ");
	  else
	    printk("S2C, ");

	  type = (val & DMA_ENG_TYPE_MASK);
	  if(type == DMA_ENG_BLOCK)
	    printk("Block DMA, ");
	  else if(type == DMA_ENG_PACKET)
	    printk("Packet DMA, ");
	  else
	    printk("Unknown DMA %x, ", type);

	  num = (val & DMA_ENG_NUMBER) >> DMA_ENG_NUMBER_SHIFT;
	  printk("Eng. Number %d, ", num);

	  bc = (val & DMA_ENG_BD_MAX_BC) >> DMA_ENG_BD_MAX_BC_SHIFT;
	  printk("Max Byte Count 2^%d\n", bc);

	  if(type != DMA_ENG_PACKET) {
	    log_normal(KERN_ERR "This driver is capable of only Packet DMA\n");
	    continue;
	  }

	  /* Initialise this engine's data structure. This will also
	   * reset the DMA engine.
	   */
	  Dma_Initialize(eptr, (base + offset), dirn);
	  eptr->pdev = pdev;

	  dmaInfo->engineMask |= (1LL << i);
        }
    }
  log_verbose(KERN_INFO "Engine mask is 0x%llx\n", dmaInfo->engineMask);
}

/******************************************DMA FUNCITON END*********************************************/

/***************************************probe fucntion*********************************************/
static int fc1553_pci_probe(struct pci_dev *pdev,const struct pci_device_id *id)
{
  /***************************variable for character dev setup****************************/
  int result,err;
  dev_t devno;
  u32 temp;
  //printk(KERN_INFO "==================================ENTER PROBE=================================\n");
  int pciRet;
  int i;
  /*
  static struct file_operations xdmaDevFileOps;
  struct timer_list * timer = &poll_timer;
  */

  /* Initialize device before it is used by driver. Ask low-level
   * code to enable I/O and memory. Wake up the device if it was
   * suspended. Beware, this function can fail.
   */
  pciRet = pci_enable_device(pdev);
  if (pciRet < 0)
    {
      printk(KERN_ERR "PCI device enable failed.\n");
      return pciRet;
    }

  /* Initialise packet pools for passing of packet arrays between this
   * and user drivers.
   */
  for(i=0; i<MAX_POOL; i++)
    {
      pktPool[i].pbuf = pktArray[i];      // Associate array with pool.
      
      if(i == (MAX_POOL-1))
	pktPool[i].next = NULL;
      else
	pktPool[i].next = &pktPool[i+1];
    }
  pktPoolTail = &pktPool[MAX_POOL-1];
  pktPoolHead = &pktPool[0];

#ifdef DEBUG_VERBOSE
  for(i=0; i<MAX_POOL; i++)
    printk("pktPool[%d] %p pktarray %p\n", i, &pktPool[i], pktPool[i].pbuf);
  printk("pktPoolHead %p pktPoolTail %p\n", pktPoolHead, pktPoolTail);
#endif

  /* Allocate space for holding driver-private data - for storing driver
   * context.
   */
  dmaData = kmalloc(sizeof(struct privData), GFP_KERNEL);
  if(dmaData == NULL)
    {
      printk(KERN_ERR "Unable to allocate DMA private data.\n");
      pci_disable_device(pdev);
      return XST_FAILURE;
    }
  //printk("dmaData at %p\n", dmaData);
  dmaData->barMask = 0;
  dmaData->engineMask = 0;
  dmaData->userCount = 0;

#if defined(DEBUG_NORMAL) || defined(DEBUG_VERBOSE)
  /* Display PCI configuration space of device. */
  //ReadConfig(pdev);
#endif

#ifdef DEBUG_VERBOSE
  /* Display PCI information on parent. */
  //ReadRoot(pdev);
#endif
  
  /*
   * Enable bus-mastering on device. Calls pcibios_set_master() to do
   * the needed architecture-specific settings.
   */
  pci_set_master(pdev);
  
  /* Reserve PCI I/O and memory resources. Mark all PCI regions
   * associated with PCI device as being reserved by owner. Do not
   * access any address inside the PCI regions unless this call returns
   * successfully.
   */
  pciRet = pci_request_regions(pdev, "FC1553_CARD");
  if (pciRet < 0) {
    printk(KERN_ERR "Could not request PCI regions.\n");
    kfree(dmaData);
    pci_disable_device(pdev);
    return pciRet;
  }

  /* Returns success if PCI is capable of 32-bit DMA */
  pciRet = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
  if (pciRet < 0) 
    {
      printk(KERN_ERR "pci_set_dma_mask failed\n");
      pci_release_regions(pdev);
      kfree(dmaData);
      pci_disable_device(pdev);
      return pciRet;
    }

  /* First read all the BAR-related information. Then read all the
   * DMA engine information. Map the BAR region to the system only
   * when it is needed, for example, when a user requests it.
   */
  for(i=0; i<MAX_BARS; i++) 
    {
      u32 size;
      
      /* Atleast BAR0 must be there. */
      if ((size = pci_resource_len(pdev, i)) == 0) {
	if (i == 0) {
	  printk(KERN_ERR "BAR 0 not valid, aborting.\n");
	  pci_release_regions(pdev);
	  kfree(dmaData);
	  pci_disable_device(pdev);
	  return XST_FAILURE;
	}
	else
	  continue;
      }
      /* Set a bitmask for all the BARs that are present. */
      else
	(dmaData->barMask) |= ( 1 << i );
      
      /* Check all BARs for memory-mapped or I/O-mapped. The driver is
       * intended to be memory-mapped.
       */
      if (!(pci_resource_flags(pdev, i) & IORESOURCE_MEM)) {
	printk(KERN_ERR "BAR %d is of wrong type, aborting.\n", i);
	pci_release_regions(pdev);
	kfree(dmaData);
	pci_disable_device(pdev);
	return XST_FAILURE;
      }

        /* Get base address of device memory and length for all BARs */
        dmaData->barInfo[i].basePAddr = pci_resource_start(pdev, i);
        dmaData->barInfo[i].baseLen = size;

	/* Map bus memory to CPU space. The ioremap may fail if size
         * requested is too long for kernel to provide as a single chunk
         * of memory, especially if users are sharing a BAR region. In
         * such a case, call ioremap for more number of smaller chunks
         * of memory. Or mapping should be done based on user request
         * with user size. Neither is being done now - maybe later.
         */
        if((dmaData->barInfo[i].baseVAddr =
            ioremap((dmaData->barInfo[i].basePAddr), size)) == 0UL)
        {
            printk(KERN_ERR "Cannot map BAR %d space, invalidating.\n", i);
            (dmaData->barMask) &= ~( 1 << i );
        }
        else
            log_verbose(KERN_INFO "[BAR %d] Base PA %x Len %d VA %x\n", i,(u32) (dmaData->barInfo[i].basePAddr),(u32) (dmaData->barInfo[i].baseLen),(u32) (dmaData->barInfo[i].baseVAddr));
    }
    log_verbose(KERN_INFO "Bar mask is 0x%x\n", (dmaData->barMask));
    log_normal(KERN_INFO "DMA Base VA %x\n",(u32)(dmaData->barInfo[0].baseVAddr));

    /* Disable global interrupts */
    Dma_mIntDisable(dmaData->barInfo[0].baseVAddr);

    dmaData->pdev=pdev;
    dmaData->index = pdev->device;
    
    /* Initialize DMA common registers? !!!! */

    /* Read DMA engine configuration and initialise data structures */
    ReadDMAEngineConfiguration(pdev, dmaData);

    /* Save private data pointer in device structure */
    pci_set_drvdata(pdev, dmaData);

  /*********************************pci  devices   setup**************************************/
  /*enable pcie device
    if (pci_enable_device(dev))
    {
    printk(KERN_INFO "Cann't enable pci device!\n");
    return -EIO;
    }
    
    //set DMA flags
    if (pci_set_dma_mask(dev,DMA_MASK))
    {
    printk(KERN_INFO "Set DMA flags error!\n");
    return -ENODEV;
    }
  *******************************************************************************************/
  
  /*******************************character devices setup************************************/
  /*static old_manner register*/
  if (major)
    {
      devno=MKDEV(major,minor);
      result=register_chrdev_region(devno,1,"FC_1553");
    }
  else  /*dynamic new_manner register*/
    {
      result=alloc_chrdev_region(&devno,minor,1,"FC_1553");
      major=MAJOR(devno);
    }

  if (result<0)
    {
      printk(KERN_INFO "chrdev register failed! Cann't get major %d\n",major);
      return result;
    }
  
  /*allocate kernel memory for char devices*/
  pfc1553_card=kmalloc(sizeof(struct fc1553_pcie_card),GFP_KERNEL);
  if (!pfc1553_card)
    {
      printk(KERN_INFO "kmalloc of fc1553_card failed!\n");
      result=-ENOMEM;
      goto fail;
    }
  memset(pfc1553_card,0,sizeof(struct fc1553_pcie_card));/*clear memory to zero*/
  
  pfc1553_card->mem_buf=kmalloc(128*sizeof(int),GFP_KERNEL);
  if (!pfc1553_card->mem_buf)
    {
      printk(KERN_INFO "kmalloc of mem_buf failed!\n");
      result=-ENOMEM;
      goto fail;
    }
  /*
  pfc1553_card->buf_write=kmalloc(sizeof(struct buf_write),GFP_KERNEL);
  if (!pfc1553_card->buf_write)
    {
      printk(KERN_INFO "kmalloc of buf_write failed!\n");
      result=-ENOMEM;
      goto fail;
    }
  */
  /****************initialize member of char dev, and setup chr dev to list*****************/
  pfc1553_card->resource.res_start=pci_resource_start(pdev,0);
  pfc1553_card->resource.res_end=pci_resource_end(pdev,0);
  pfc1553_card->resource.res_length=pci_resource_len(pdev,0);
  pfc1553_card->resource.virt_res_start=phys_to_virt(pfc1553_card->resource.res_start);
  pfc1553_card->resource.virt_res_end=phys_to_virt(pfc1553_card->resource.res_end);
  pfc1553_card->pdev=pdev;
  /********************************request for pcie resource********************************/
  /*
  if (pci_request_region(dev,0,"FC_1553"))
    {
      printk(KERN_INFO "fc1553_card request region error!\n");
      goto fail_1;
    }
  */
  pfc1553_card->addr_map=dmaData->barInfo[0].baseVAddr;
  if (!pfc1553_card->addr_map)
    {
      printk(KERN_INFO "pci_iomap error!\n");
      goto fail_1;
    }
  pfc1553_card->bar0_start=pfc1553_card->addr_map;//+0x18000;
	
  /********************************************************************************************
  printk(KERN_INFO "pfc1553_card->resource.res_start=0x%x\n",pfc1553_card->resource.res_start);
  printk(KERN_INFO "pfc1553_card->resource.res_end=0x%x\n",pfc1553_card->resource.res_end);
  printk(KERN_INFO "pfc1553_card->resource.virt_res_start=0x%x\n",pfc1553_card->resource.virt_res_start);
  printk(KERN_INFO "pfc1553_card->resource.virt_res_end=0x%x\n",pfc1553_card->resource.virt_res_end);
  printk(KERN_INFO "value of bar0_start=0x%x\n",pfc1553_card->bar0_start);
  iowrite32(0x12345678,pfc1553_card->bar0_start);
  temp=ioread32(pfc1553_card->bar0_start);
  printk(KERN_INFO "value of read is 0x%x\n",temp);
  ********************************************************************************************/
 
  /**********************************************end*******************************************/
 
  pfc1553_card->int_num=255;
  sema_init(&(pfc1553_card->sem),1);
  
  cdev_init(&(pfc1553_card->cdev),&fc1553_fops);
  pfc1553_card->cdev.owner=THIS_MODULE;
  pfc1553_card->cdev.ops=&fc1553_fops;
  
  err=cdev_add(&(pfc1553_card->cdev),devno,1);
  if (err)
    {
      printk(KERN_WARNING "Error in adding character dev!\n");
      return -ENODEV;
    }
  printk(KERN_INFO "fc1553_card device add success!\n");
  return OK;
  
 fail_1:
  fc1553_pci_remove(pdev); 
 fail:
  //fc1553_exit();
 result=-ENODEV;
 return result;
}

/****************************************fc1553_pcie information************************************/
static struct pci_driver fc1553_pci_driver={
 name: "FC_1553",
 id_table: fc1553_pci_tbl,
 probe: fc1553_pci_probe,
 remove: fc1553_pci_remove
};

/**********************************************module exit function*********************************/
static void __exit fc1553_exit(void)
{
  kfree(pfc1553_card->mem_buf);

  kfree(pfc1553_card);

  pci_unregister_driver(&fc1553_pci_driver);
  
  unregister_chrdev_region(MKDEV(FC1553_MAJOR,FC1553_MINOR),1);
  
  printk(KERN_INFO "exit fc1553_module\n");
}
/**************************************************end*********************************************/

/**********************************************module init function*********************************/
static int __init fc1553_init(void)
{
  /*check system support pci or not
  if (!pci_present())
    {
      printk(KERN_WARNING "Current system doesn't support PCI bus\n");
      return -ENODEV;
    }
   *del for pc_present,maybe too old */
  /*register pci driver*/
  if (pci_register_driver(&fc1553_pci_driver)!=0)
    {
      printk(KERN_INFO "pci register error!\n");
      pci_unregister_driver(&fc1553_pci_driver);
      return -ENODEV;
    }
  return OK;
}
/**************************************************end*********************************************/

MODULE_AUTHOR("Faithere");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("driver for fc1553 card");
module_init(fc1553_init);
module_exit(fc1553_exit);
