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

int major=FC1553_MAJOR;
int minor=FC1553_MINOR;

/*******************************global variable for DMA***********************************/

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


/******************************************DMA FUNCITON END*********************************************/

/***************************************probe fucntion*********************************************/
static int fc1553_pci_probe(struct pci_dev *pdev,const struct pci_device_id *id)
{
  /***************************variable for character dev setup****************************/
  int result,err,pciRet;

  u64 addr_u64;
  u32 * addr_pu32;
  void *addr_pvoid;
  void *addr_virtual;
  

  printk(KERN_INFO "==================================ENTER PROBE=================================\n");

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

  addr_u64=pci_resource_start(pdev,0); 
  
  //addr_pu32=pci_resource_start(pdev,0);
  
  //addr_pvoid=pci_resource_start(pdev,0); 

  addr_virtual=(u64)ioremap(addr_u64,4);
  printk(KERN_INFO "addr_virtual=0x%p\n",addr_virtual);
  
  printk(KERN_INFO "ioread32(addr_virtual)=0x%x\n",ioread32(addr_virtual));
  //printk(KERN_INFO "ioread32(addr_pu32)=0x%x\n",ioread32(addr_pu32));
  //printk(KERN_INFO "ioread32(addr_pvoid)=0x%x\n",ioread32(addr_pvoid));

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
  unregister_chrdev_region(MKDEV(FC1553_MAJOR,FC1553_MINOR),1);
  printk(KERN_INFO "exit fc1553_module\n");
}
/**************************************************end**********************************************/

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
