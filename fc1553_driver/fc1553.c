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

#include "fc1553.h"

int major=FC1553_MAJOR;

int minor=FC1553_MINOR;

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
  
  u32 *addr=pcard->bar0_start;
  
  if (down_interruptible(&(pcard->sem)))
    return -ERESTARTSYS;
  /*
  printk(KERN_INFO "pcard->bar0_start=0x%x\n",pcard->bar0_start);
  */
  *(pcard->mem_buf)=ioread32(addr+flip->f_pos);
  /*
  printk(KERN_INFO "read executed! read_val=0x%x\n",temp);
  */
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
  
  u32 *addr=pcard->bar0_start;
  
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
  
  iowrite32(*(pcard->mem_buf),addr+flip->f_pos);
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

/***************************************probe fucntion*********************************************/
static int fc1553_pci_probe(struct pci_dev *dev,const struct pci_device_id *id)
{
  /***************************variable for character dev setup****************************/
  int result,err;
  dev_t devno;
  u32 temp;
  //printk(KERN_INFO "==================================ENTER PROBE=================================\n");
  /*******************************pci  devices   setup************************************/
  /*enable pcie device*/
  if (pci_enable_device(dev))
    {
      printk(KERN_INFO "Cann't enable pci device!\n");
      return -EIO;
    }
  /*
     //set DMA flags
  if (pci_set_dma_mask(dev,DMA_MASK))
    {
      printk(KERN_INFO "Set DMA flags error!\n");
      return -ENODEV;
    }
   */
  
  /*****************************character devices setup***********************************/
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
  pfc1553_card->resource.res_start=pci_resource_start(dev,0);
  pfc1553_card->resource.res_end=pci_resource_end(dev,0);
  pfc1553_card->resource.res_length=pci_resource_len(dev,0);
  pfc1553_card->resource.virt_res_start=phys_to_virt(pfc1553_card->resource.res_start);
  pfc1553_card->resource.virt_res_end=phys_to_virt(pfc1553_card->resource.res_end);
  pfc1553_card->pdev=dev;
  /********************************request for pcie resource********************************/
  
  if (pci_request_region(dev,0,"FC_1553"))
    {
      printk(KERN_INFO "fc1553_card request region error!\n");
      goto fail_1;
    }
  
  pfc1553_card->addr_map=pci_iomap(dev,0,pfc1553_card->resource.res_length);
  if (!pfc1553_card->addr_map)
    {
      printk(KERN_INFO "pci_iomap error!\n");
      goto fail_1;
    }
  pfc1553_card->bar0_start=pfc1553_card->addr_map+0x18000;
	
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
  fc1553_pci_remove(dev); 
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
