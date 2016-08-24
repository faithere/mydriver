#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <linux/time.h>
#include <linux/timer.h>

#include <asm/irqs.h>  //define for irq num
#include <linux/interrupt.h>


/*******************MacroDef*******************/
#define BUTTON_IRQ_1 EINT0
#define BUTTON_IRQ_2 EINT1
#define BUTTON_IRQ_3 EINT2
#define BUTTON_IRQ_4 EINT3

#define BUTTON_IRQ_NUM 4
#define BUTTON_NUM 16
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
  unsigned short key_value[BUTTON_NUM];
  
  struct semphore sem;
  struct timer_list delay_timer;
  struct cdev cdev;
};
struct keybutton *pkeybutton;
/**********************************************/

//character file operations ops
static struct file_operations keybutton_fops
{
    owner:THIS_MODULE,
    read:keybutton_read,
    write:keybutton_write,
    open:keybutton_open,
    release:keybutton_close,
    llseek:keybutton_lseek,
    compat_ioctl:keybutton_ioctl
};

//irq bottom halves
void irq_tl_handle(unsigned long );
DECLARE_TASKLET(button_tasklet,irq_tl_handle,0);
//end irq bottom halves

/*********************************************/


/*****************IRQ_FUNC********************/
static irqreturn_t button_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
  //first disable irq num
  disable_irq(irq);

  //do some handle




  tasklet_schedule(&button_tasklet);//notify kernel to call bottom halves
  //enbale irq num
  enable_irq(irq);
  return IRQ_HANDLED;
}

static void irq_tl_handle(int irq, void *dev_id, struct pt_regs *regs)
{
  struct keybutton *pkeybutton=(struct keybutton *)data;
  long curtime=jiffies;

  //find which button is pressed
  switch (irq)
    {
    case BUTTON_IRQ_1:
      break;
    case BUTTON_IRQ_2:
      break;
    case BUTTON_IRQ_3:
      break;
    case BUTTON_IRQ_4:
      break;
    }
  //register timer
  pkeybutton->delay_timer.function=delay_timer_fn;
  pkeybutton->delay_timer.expire=curtime+tdelay;
  add_timer(&pkeybutton->delay_timer);


}

/****************FILE OPS*********************/

int keybutton_open(struct inode *inode, struct file *flip)
{
  int res;
  unsigned short i=0;
  struct keybutton *keybutton_dev;

  keybutton_dev=container_of(inode->i_cdev,struct keybutton,cdev);
  flip->private_data=keybutton_dev; //Can we use global pointer pkeybutton directly?



  for (i=0;i<BUTTON_IRQ_NUM;i++)
    {
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

/*********************************************/
static int button_init(void)
{
  int result;
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

fail:
  
  return result;
}


static void button_exit(void)
{
}

MODULE_AUTHOR("Faithere");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Keybutton driver for FS2410");
module_init(button_init);
module_exit(button_exit);

