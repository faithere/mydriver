#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <linux/time.h>
#include <linux/timer.h>

#include <linux/interrupt.h>
#include <asm/irq.h>  //IRQ TYPE IRQT_FALLING
#include <linux/irq.h>
#include <asm-arm/arch-s3c2410/irqs.h>

#include <asm/io.h>  //for ioread32 etc
/*******************MacroDef*******************/
#define BUTTON_IRQ_1 IRQ_EINT0
#define BUTTON_IRQ_2 IRQ_EINT2
#define BUTTON_IRQ_3 IRQ_EINT11
#define BUTTON_IRQ_4 IRQ_EINT19

#define BUTTON_IRQ_NUM 4 //BUTTON ROW NUM
#define BUTTON_NUM 16
#define BUTTON_COLUM 4  //BUTTON COLUMN NUM
/**********************************************/

/*******************Glb Data*******************/
int button_major;
int button_minor=1;
int button_dev_num=1;

//button qudou delay time value
int tdelay=20;
module_param(tdelay,int,0);

//key button character device data struct
struct keybutton
{
  unsigned short button_irq_num[BUTTON_IRQ_NUM];
  unsigned short key_value;
  long gpio_mem[BUTTON_COLUM];
  
  int cur_irq;
  struct semphore sem;
  struct timer_list delay_timer;
  struct cdev cdev;
};
struct keybutton *pkeybutton;
/**********************************************/


#ifdef TASKLET_ENABLE
//irq bottom halves
void irq_tl_handle(unsigned long );
DECLARE_TASKLET(button_tasklet,irq_tl_handle,0);
//end irq bottom halves
#endif

/*********************************************/

/*****************TIMER_FUNC******************/
static void delay_timer_fn(unsigned long data)
{
  struct keybutton *pkt=(struct keybutton *)data;
  unsigned int reg_value=0;
  //find which button is pressed
  switch (pkt->cur_irq)
    {
    case BUTTON_IRQ_1:
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[0]);
	if (reg_value&0x800) //button 10
	  {
	    pkt->key_value=10;
	    //	    break;
	  }	  
      
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[1]);
	if (reg_value&0x40) //button 11
	  {
	    pkt->key_value=11;
	    //	    break;
	  }	  

      reg_value=ioread32((unsigned int *)pkt->gpio_mem[2]);
	if (reg_value&0x2000) //button 12
	  {
	    pkt->key_value=12;
	    //	    break;
	  }	  
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[3]);
	if (reg_value&0x4) //button 16
	    {
	      pkt->key_value=16;
	    //      break;
	    }	  
      break;
    case BUTTON_IRQ_2:
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[0]);
	if (reg_value&0x800) //button 7
	  {
	    pkt->key_value=7;
	    //	    break;
	  }	  
      
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[1]);
	if (reg_value&0x40) //button 8
	  {
	    pkt->key_value=8;
	    //	    break;
	  }	  

      reg_value=ioread32((unsigned int *)pkt->gpio_mem[2]);
	if (reg_value&0x2000) //button 9
	  {
	    pkt->key_value=9;
	    //	    break;
	  }	  
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[3]);
	if (reg_value&0x4) //button 15
	    {
	      pkt->key_value=15;
	    //      break;
	    }	  
      break;
    case BUTTON_IRQ_3:
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[0]);
	if (reg_value&0x800) //button 4
	  {
	    pkt->key_value=4;
	    //	    break;
	  }	  
      
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[1]);
	if (reg_value&0x40) //button 5
	  {
	    pkt->key_value=5;
	    //	    break;
	  }	  

      reg_value=ioread32((unsigned int *)pkt->gpio_mem[2]);
	if (reg_value&0x2000) //button 6
	  {
	    pkt->key_value=6;
	    //	    break;
	  }	  
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[3]);
	if (reg_value&0x4) //button 14
	    {
	      pkt->key_value=14;
	    //      break;
	    }	  
      break;
    case BUTTON_IRQ_4:
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[0]);
	if (reg_value&0x800) //button 1
	  {
	    pkt->key_value=1;
	    //	    break;
	  }	  
      
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[1]);
	if (reg_value&0x40) //button 2
	  {
	    pkt->key_value=2;
	    //	    break;
	  }	  

      reg_value=ioread32((unsigned int *)pkt->gpio_mem[2]);
	if (reg_value&0x2000) //button 3
	  {
	    pkt->key_value=3;
	    //	    break;
	  }	  
      reg_value=ioread32((unsigned int *)pkt->gpio_mem[3]);
	if (reg_value&0x4) //button 13
	    {
	      pkt->key_value=13;
	    //      break;
	    }	  
      break;
    }
  del_timer(&pkt->delay_timer);
}
/*********************************************/

/*****************IRQ_FUNC********************/
static irqreturn_t button_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
  long curtime=jiffies;
  //first disable irq num
  disable_irq(irq);

  //do some handle
  pkeybutton->cur_irq=irq;
  //register timer
  pkeybutton->delay_timer.data=(unsigned long)pkeybutton;
  pkeybutton->delay_timer.function=delay_timer_fn;
  pkeybutton->delay_timer.expires=curtime+tdelay;
  add_timer(&pkeybutton->delay_timer);

#ifdef TASKLET_ENABLE
  tasklet_schedule(&button_tasklet);//notify kernel to call bottom halves
#endif

  //enbale irq num
  enable_irq(irq);
  return IRQ_HANDLED;
}

#ifdef TASKLET_ENABLE
static void irq_tl_handle(int irq, void *dev_id, struct pt_regs *regs)
{
  struct keybutton *pkeybutton=(struct keybutton *)data;
  long curtime=jiffies;
}
#endif
/*********************************************/

/****************FILE OPS*********************/

static int keybutton_open(struct inode *inode, struct file *flip)
{
  int res;
  unsigned short i=0;
  struct keybutton *keybutton_dev;


  keybutton_dev=container_of(inode->i_cdev,struct keybutton,cdev);
  flip->private_data=keybutton_dev; //Can we use global pointer pkeybutton directly?



  for (i=0;i<BUTTON_IRQ_NUM;i++)
    {
      //configure IRQ type
      set_irq_type(keybutton_dev->button_irq_num[i],IRQT_FALLING);
      //request for irq
      res=request_irq(keybutton_dev->button_irq_num[i],button_irq_handler,IRQF_TRIGGER_FALLING,IRQF_SHARED,NULL);
      if (!res)  //irq request fail
	{
	  if (res==-EINVAL)
	    {
	      printk(KERN_INFO "IRQ request number invalid!\n");
	      free_irq(keybutton_dev->button_irq_num[i],NULL);
	      return res;
	    }
	  if (res==-EBUSY)
	    {
	      printk(KERN_INFO "IRQ request number already used!\n");
	      free_irq(keybutton_dev->button_irq_num[i],NULL);
	      return res;
	    }
	}
    }
  printk(KERN_INFO "All IRQ request register success!\n");
  
  for (i=0;i<BUTTON_IRQ_NUM;i++)
    {
      //Enable irq
      irq_enable(keybutton_dev->button_irq_num[i]);
    }

}
static ssize_t keybutton_read(struct file * flip, char __user *buf , size_t count, loff_t *f_pos)
{
  //for the realization of function,must think from its input variable check & return value calculate.It's sage thinking ^_^
  struct keybutton *pkb=flip->private_data;
  ssize_t ret=0;
  
  if (down_interrupiable(&pkb->sem))
    return -ERESTARTSYS;
  
  if (count!=1)
    goto out;

  if (pkt->key_value==0)
    goto out;

  if (copy_to_user(buf,&pkt->key_value,1))
    {
      ret=-EFAULT;
      goto out;
    }
  ret=1;
  *f_pos=0;//always start from zero
 out:
  up(&pkt->sem);
  return ret;
} 
/*********************************************/

//character file operations ops
static struct file_operations keybutton_fops
{
    owner:THIS_MODULE,
    read:keybutton_read,
    //    write:keybutton_write,
    //    open:keybutton_open,
    //    release:keybutton_close,
    //    llseek:keybutton_lseek,
    //    compat_ioctl:keybutton_ioctl
};

static int button_init(void)
{
  unsigned short i;
  int result;
  unsigned int addr=0,reg_value=0;
  dev_t dev_num;//device number

  if (button_major)
    {
      dev_num=MKDEV(button_major,button_minor);
      result=register_chrdev_region(dev_num,button_dev_num,"keybutton");
      if (result<0)
	{
	  printk(KERN_WARNING,"character device button traditional register error!\n");
	  return result;
	}
    }
  else
    {
      result=alloc_chrdev_region(&dev_num,button_minor,button_dev_num,"keybutton");
      if (result<0)
	{
	  printk(KERN_WARNING,"character device button dynamic register error!\n");
	  return result;
	}
      button_major=MAJOR(dev_num);
    }
  
  pkeybutton=kmalloc(sizeof(struct keybutton),GFP_KERNEL);
  if (!pkeybutton)
    {
      printk(KERN_INFO,"kmalloc of keybutton failed\n");
      result=-ENOMEM;
      goto fail;
    }
  memset(pkeybutton,0,sizeof(struct keybutton));

  //init timer & semphore
  sema_init(&pkeybutton->sem,1);
  init_timer(&pkeybutton->delay_timer);
  //dispatch IRQ 
  pkeybutton->button_irq_num[0]=BUTTON_IRQ_1;
  pkeybutton->button_irq_num[1]=BUTTON_IRQ_2;
  pkeybutton->button_irq_num[2]=BUTTON_IRQ_3;
  pkeybutton->button_irq_num[3]=BUTTON_IRQ_4;

  cdev_init(&pkeybutton->cdev,&keybutton_fops);//void type function
  
  result=cdev_add(&pkeybutton->cdev,dev_num,button_dev_num);
  if (result<0)
    {
      printk(KERN_INFO "character device add failed!\n");
      goto fail;
    }
  printk(KERN_INFO "Character device keybutton add success!\n");
  
  //request interrupt
  //I remeber,interrupt request should init in the open function,for interrupt resouce is in short
 
  //GPIO Setting
  //GPECON
  addr=(unsigned int)ioremap(0x56000040,4);
  reg_value=ioread32((unsigned int *)addr);
  reg_value=(reg_value&0xfffff33)|0x0;
  iowrite32(reg_value,(unsigned int *)addr);
  //GPFCON
  addr=(unsigned int)ioremap(0x56000050,4);
  reg_value=ioread32((unsigned int *)addr);
  reg_value=(reg_value&0xffffffcc)|0x22;
  iowrite32(reg_value,(unsigned int *)addr);
  //GPGCON
  addr=(unsigned int)ioremap(0x56000060,4);
  reg_value=ioread32((unsigned int *)addr);
  reg_value=(reg_value&0xff3f3f0f)|0x002f0f80;
  iowrite32(reg_value,(unsigned int *)addr);

  //GPIO configure PIN function

  //ioremap memory for button check
  pkeybutton->gpio_mem[0]=(unsigned int)ioremap(0x56000044,4);
  pkeybutton->gpio_mem[1]=(unsigned int)ioremap(0x56000064,4);
  pkeybutton->gpio_mem[2]=(unsigned int)ioremap(0x56000044,4);
  pkeybutton->gpio_mem[3]=(unsigned int)ioremap(0x56000064,4);
  

fail:
  
  return result;
}


static void button_exit(void)
{
  //iounreamap

  //unregister IRQ

  //del timer
#ifdef TASKLET_ENABLE
  //del tasklet
#endif

  //unregister character device

}

MODULE_AUTHOR("Faithere");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Keybutton driver for FS2410");
module_init(button_init);
module_exit(button_exit);

