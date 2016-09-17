#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <linux/time.h>
#include <linux/timer.h>

#include <asm/uaccess.h>  //copy_to_usr

#include <linux/interrupt.h>
#include <asm-arm/irq.h>  //IRQ TYPE IRQT_FALLING
#include <linux/irq.h>
#include <asm-arm/arch-s3c2410/irqs.h>
#include <asm-arm/arch-s3c2410/regs-gpio.h>

#include <linux/wait.h>

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
int button_major=250;
int button_minor=0;
int button_dev_num=1;

//button qudou delay time value

//float tdelay=0.5;
//module_param(tdelay,float,0);

//key button character device data struct
struct keybutton
{
  unsigned int button_irq_num[BUTTON_IRQ_NUM];
  unsigned int gpio_mem[BUTTON_COLUM];
  int cur_irq;
  short key_value;

  unsigned int GPECONF;
  unsigned int GPFCONF;
  unsigned int GPGCONF;

  struct semaphore sem;
  wait_queue_head_t my_waitq;
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
static void delay(int n)
{
  int t=100*n;
  while(t--);

}
/***************GPIO_INIT*********************/
static void gpio_conf_init(struct keybutton *dev)
{
  unsigned int addr=0;
  int reg_value=0;

    //GPIO Ioremap Setting
  /****************GPECON****************/
  /********GPE11------------Output********/
  /********GPE13------------Output********/
  dev->GPECONF=(unsigned int)ioremap(0x56000040,4);

  /***************GPFCON*****************/
  /********GPF0--------------EINT0*******/
  /********GPF2--------------EINT2*******/
  dev->GPFCONF=(unsigned int)ioremap(0x56000050,4);

  /****************GPGCON****************/
  /**********GPG2--------Output***********/
  /**********GPG3--------EINT11**********/
  /**********GPG6--------Output***********/
  /**********GPG11-------EINT19**********/
  dev->GPGCONF=(unsigned int)ioremap(0x56000060,4);


  //Pull down output pin /*This is very important*/
  /*GPEDAT11 & GPEDAT13 AS LOW*/
  /*GPGDAT2  & GPGDAT6  AS LOW*/
  addr=(unsigned int)ioremap(0x56000044,4);
  reg_value=ioread32((volatile unsigned int *)addr);
  //  printk(KERN_INFO "before GPEDAT=0x%x\n",reg_value);
  reg_value=reg_value & (~(1<<13 | 1<<11));
  iowrite32(reg_value,(volatile unsigned int *)addr);
  //  reg_value=ioread32((unsigned int *)addr);
  //  printk(KERN_INFO "after GPEDAT=0x%x\n",reg_value);
  //iounmap must done
  iounmap((volatile unsigned int *)addr);

  addr=(unsigned int)ioremap(0x56000064,4);
  reg_value=ioread32((volatile unsigned int *)addr);
  //  printk(KERN_INFO "before GPGDAT=0x%x\n",reg_value);
  reg_value=reg_value & (~(1<<6 | 1<<2));
  iowrite32(reg_value,(volatile unsigned int *)addr);
  //  reg_value=ioread32((unsigned int *)addr);
  //  printk(KERN_INFO "after GPGDAT=0x%x\n",reg_value);
  //iounmap must done
  iounmap((volatile unsigned int *)addr);


}
static void gpio_init_output(struct keybutton *dev)
{
  unsigned int reg_value;
  //GPIO Setting
  /****************GPECON****************/
  /********GPE11------------Output********/
  /********GPE13------------Output********/
  reg_value=ioread32((volatile unsigned int *)dev->GPECONF);
  //  printk(KERN_INFO "before GPECON=0x%x\n",reg_value);
  reg_value=(reg_value & (~(3<<26 | 3<<22))) | ((1<<26)|(1<<22));
  iowrite32(reg_value,(volatile unsigned int *)dev->GPECONF);
  //  reg_value=ioread32((unsigned int *)addr);
  //  printk(KERN_INFO "after GPECON=0x%x\n",reg_value);

  /***************GPFCON*****************/
  /********GPF0--------------EINT0*******/
  /********GPF2--------------EINT2*******/
  reg_value=ioread32((volatile unsigned int *)dev->GPFCONF);
  //  printk(KERN_INFO "before GPFCON=0x%x\n",reg_value);
  reg_value=(reg_value & (~(3<<4 | 3))) | ((2<<4)|2);
  iowrite32(reg_value,(volatile unsigned int *)dev->GPFCONF);
  //  reg_value=ioread32((unsigned int *)addr);
  //  printk(KERN_INFO "after GPFCON=0x%x\n",reg_value);

  /****************GPGCON****************/
  /**********GPG2--------Output***********/
  /**********GPG3--------EINT11**********/
  /**********GPG6--------Output***********/
  /**********GPG11-------EINT19**********/
  reg_value=ioread32((volatile unsigned int *)dev->GPGCONF);
  //  printk(KERN_INFO "before GPGCON=0x%x\n",reg_value);
  reg_value=(reg_value & (~(3<<22 | 3<<12 | 3<<6 | 3<<4))) | (2<<22 | 1<<12 | 2<<6 | 1<<4);
  iowrite32(reg_value,(volatile unsigned int *)dev->GPGCONF);
  //  reg_value=ioread32((unsigned int *)addr);
  //  printk(KERN_INFO "after GPGCON=0x%x\n",reg_value);
}

static void gpio_init_input(struct keybutton *dev)
{
  unsigned int reg_value;
  
  /****************GPECON****************/
  /********GPE11------------Iutput********/
  /********GPE13------------Iutput********/
  reg_value=ioread32((volatile unsigned int *)dev->GPECONF);
  //  printk(KERN_INFO "before GPECON=0x%x\n",reg_value);
  reg_value=(reg_value & (~(3<<26 | 3<<22)));
  iowrite32(reg_value,(volatile unsigned int *)dev->GPECONF);
  //  reg_value=ioread32((unsigned int *)addr);
  //  printk(KERN_INFO "after GPECON=0x%x\n",reg_value);

  /****************GPGCON****************/
  /**********GPG2--------Output***********/
  /**********GPG6--------Output***********/
  reg_value=ioread32((volatile unsigned int *)dev->GPGCONF);
  //  printk(KERN_INFO "before GPGCON=0x%x\n",reg_value);
  reg_value=(reg_value & (~(3<<12 | 3<<4)));
  iowrite32(reg_value,(volatile unsigned int *)dev->GPGCONF);
  //  reg_value=ioread32((unsigned int *)addr);
  //  printk(KERN_INFO "after GPGCON=0x%x\n",reg_value);
}

/*****************TIMER_FUNC******************/
static void delay_timer_fn(unsigned long data)
{
  struct keybutton *pkt=(struct keybutton *)data;
  unsigned int reg_value=0;

#ifdef DEBUG_ENABLE
  printk(KERN_INFO "KEY PRESS\n");
  printk(KERN_INFO "KEY=%d\n",pkt->cur_irq);
#endif

  if (pkt->cur_irq)  //cur_irq must equal non-zero
    {
      //set gpio as input
      gpio_init_input(pkt);

      //#ifdef DEBUG_ENABLE
      //find which button is pressed
      switch (pkt->cur_irq)
	{
	  
	case BUTTON_IRQ_1:
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[0]);
	  if (reg_value&0x800) //button 10
	    {
	      pkt->key_value=10;
	      break;
	    }	  

	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[1]);
	  if (reg_value&0x40) //button 11
	    {
	      pkt->key_value=11;
	      break;
	    }	  
	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[2]);
	  if (reg_value&0x2000) //button 12
	    {
	      pkt->key_value=12;
	      break;
	    }	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[3]);
	  if (reg_value&0x4) //button 16
	    {
	      pkt->key_value=16;
	      break;
	    }	 
	  
      break;
	case BUTTON_IRQ_2:
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[0]);
	  if (reg_value&0x800) //button 7
	    {
	      pkt->key_value=7;
	      break;
	    }	  
	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[1]);
	  if (reg_value&0x40) //button 8
	    {
	      pkt->key_value=8;
	      break;
	    }	  
	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[2]);
	  if (reg_value&0x2000) //button 9
	    {
	      pkt->key_value=9;
	      break;
	  }	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[3]);
	  if (reg_value&0x4) //button 15
	    {
	      pkt->key_value=15;
	      break;
	    }	  
	  break;
	case BUTTON_IRQ_3:
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[0]);
	  if (reg_value&0x800) //button 4
	    {
	      pkt->key_value=4;
	      break;
	    }	  
      
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[1]);
	  if (reg_value&0x40) //button 5
	    {
	      pkt->key_value=5;
	      break;
	    }	  
	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[2]);
	  if (reg_value&0x2000) //button 6
	    {
	      pkt->key_value=6;
	      break;
	  }	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[3]);
 	  if (reg_value&0x4) //button 14
	    {
	      pkt->key_value=14;
	      break;
	    }	  
	  break;
	case BUTTON_IRQ_4:
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[0]);
	  if (reg_value&0x800) //button 1
	    {
	      pkt->key_value=1;
	      break;
	    }	  
	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[1]);
	  if (reg_value&0x40) //button 2
	  {
	    pkt->key_value=2;
	    break;
	  }	  
	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[2]);
	  if (reg_value&0x2000) //button 3
	    {
	      pkt->key_value=3;
	      break;
	    }	  
	  reg_value=ioread32((volatile unsigned int *)pkt->gpio_mem[3]);
	if (reg_value&0x4) //button 13
	  {
	    pkt->key_value=13;
	    break;
	  }	  
	break;

	}

      wake_up_interruptible(&pkt->my_waitq);
      //reset gpio as output and pull-down

      gpio_init_output(pkt);
      //#endif
    }
}
/*********************************************/

/*****************IRQ_FUNC********************/
static irqreturn_t button_irq_handler(int irq, void *dev_id)
{
  struct keybutton *pkb=(struct keybutton*)dev_id;
  
#ifdef DEBUG_ENABLE
  printk(KERN_INFO "KEY PRESS\n");
#endif

  pkb->cur_irq=irq;
  delay(100);
  mod_timer(&(pkb->delay_timer),jiffies+HZ/6);
  
#ifdef TASKLET_ENABLE
  tasklet_schedule(&button_tasklet);//notify kernel to call bottom halves
#endif
  
  return IRQ_HANDLED;
}
/********************************************/

/******************TASKLET*******************/
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
  int res=0;
  unsigned short i=0;
  struct keybutton *keybutton_dev;
  char *keyirq_name="";
  
  keybutton_dev=container_of(inode->i_cdev,struct keybutton,cdev);
  flip->private_data=keybutton_dev; //Can we use global pointer pkeybutton directly?

  for (i=0;i<BUTTON_IRQ_NUM;i++)
    {
      keyirq_name="";
      sprintf(keyirq_name,"keybutton%d",i);

#ifdef DEBUG_ENABLE
      //request for irq
      printk(KERN_INFO "irq_num=%d\n",keybutton_dev->button_irq_num[i]);
#endif

      res=request_irq(keybutton_dev->button_irq_num[i],button_irq_handler,IRQF_TRIGGER_FALLING|IRQF_DISABLED,keyirq_name,(void *)keybutton_dev);
      if (res)  //irq request fail
	{				       

#ifdef DEBUG_ENABLE
	  printk(KERN_INFO "ret_value=%d\n",res);
#endif

	  if (res==-EINVAL)
	    {
	      printk(KERN_INFO "IRQ request number invalid!\n");
	      //      free_irq(keybutton_dev->button_irq_num[i],NULL);
	      return res;
	    }
	  if (res==-EBUSY)
	    {
	      printk(KERN_INFO "IRQ request number already used!\n");
	      //      free_irq(keybutton_dev->button_irq_num[i],NULL);
	      return res;
	    }
	}
    }

#ifdef DEBUG_ENABLE
  printk(KERN_INFO "All IRQ request register success!\n");
#endif

  return 0;
}
static ssize_t keybutton_read(struct file * flip, char __user *buf , size_t count, loff_t *f_pos)
{
  //for the realization of function,must think from its input variable check & return value calculate.It's sage thinking ^_^
  struct keybutton *pkb=flip->private_data;
  ssize_t ret=0;
  
  if (down_interruptible(&pkb->sem))
    return -ERESTARTSYS;

#ifdef DEBUG_ENABLE  
  printk(KERN_INFO "enter read\n");
#endif

  if (count!=1)
    goto out;
  
  pkb->key_value=0;
  
  wait_event_interruptible(pkb->my_waitq,pkb->key_value!=0);

#ifdef DEBUG_ENABLE
  printk(KERN_INFO "wake up!\n");
#endif

  if (copy_to_user(buf,&pkb->key_value,1))
    {
      ret=-EFAULT;
      goto out;
    }

  ret=1;
  *f_pos=0;//always start from zero
 out:
  up(&pkb->sem);
  return ret;
} 

static int keybutton_close(struct inode *inode, struct file *flip)
{
  int i=0;
  struct keybutton *pkt=flip->private_data;
  //unregister IRQ
  for (i=0;i<BUTTON_IRQ_NUM;i++)
    free_irq(pkt->button_irq_num[i],pkt);
  return 0;
}

/*********************************************/

//character file operations ops
static struct file_operations keybutton_fops =
{
    owner:THIS_MODULE,
    read:keybutton_read,
    //    write:keybutton_write,
    open:keybutton_open,
    release:keybutton_close
    //    llseek:keybutton_lseek,
    //    compat_ioctl:keybutton_ioctl
};

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
	  printk(KERN_WARNING "character device button traditional register error!\n");
	  return result;
	}
    }
  else
    {
      result=alloc_chrdev_region(&dev_num,button_minor,button_dev_num,"keybutton");
      if (result<0)
	{
	  printk(KERN_WARNING "character device button dynamic register error!\n");
	  return result;
	}
      button_major=MAJOR(dev_num);
    }
  
  pkeybutton=kmalloc(sizeof(struct keybutton),GFP_KERNEL);
  if (!pkeybutton)
    {
      printk(KERN_INFO "kmalloc of keybutton failed\n");
      result=-ENOMEM;
      goto fail;
    }
 
  memset(pkeybutton,0,sizeof(struct keybutton));
  
  //init keybutton device
  //ioremap memory for button check
  pkeybutton->gpio_mem[0]=(unsigned int)ioremap(0x56000044,4);
  pkeybutton->gpio_mem[1]=(unsigned int)ioremap(0x56000064,4);
  pkeybutton->gpio_mem[2]=(unsigned int)ioremap(0x56000044,4);
  pkeybutton->gpio_mem[3]=(unsigned int)ioremap(0x56000064,4);
 
#ifdef DEBUG_ENABLE
  pkeybutton->key_data_addr[0]=(unsigned int)ioremap(0x56000044,4); //KEY1
  pkeybutton->key_data_addr[3]=(unsigned int)ioremap(0x56000044,4); //KEY4
  pkeybutton->key_data_addr[6]=(unsigned int)ioremap(0x56000044,4); //KEY7
  pkeybutton->key_data_addr[9]=(unsigned int)ioremap(0x56000044,4); //KEY10

  pkeybutton->key_data_addr[1]=(unsigned int)ioremap(0x56000044,4); //KEY2
  pkeybutton->key_data_addr[4]=(unsigned int)ioremap(0x56000044,4); //KEY5
  pkeybutton->key_data_addr[7]=(unsigned int)ioremap(0x56000044,4); //KEY8
  pkeybutton->key_data_addr[10]=(unsigned int)ioremap(0x56000044,4); //KEY11

  pkeybutton->key_data_addr[2]=(unsigned int)ioremap(0x56000044,4); //KEY3
  pkeybutton->key_data_addr[5]=(unsigned int)ioremap(0x56000044,4); //KEY6
  pkeybutton->key_data_addr[8]=(unsigned int)ioremap(0x56000044,4); //KEY9
  pkeybutton->key_data_addr[11]=(unsigned int)ioremap(0x56000044,4); //KEY12

  pkeybutton->key_data_addr[12]=(unsigned int)ioremap(0x56000044,4); //KEY13
  pkeybutton->key_data_addr[13]=(unsigned int)ioremap(0x56000044,4); //KEY14
  pkeybutton->key_data_addr[14]=(unsigned int)ioremap(0x56000044,4); //KEY15
  pkeybutton->key_data_addr[15]=(unsigned int)ioremap(0x56000044,4); //KEY16
#endif

  //init timer & semphore
  sema_init(&pkeybutton->sem,1);
  init_timer(&pkeybutton->delay_timer);
  init_waitqueue_head(&pkeybutton->my_waitq);

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


 
  //GPIO Setting except interrupt pin, set as output and pull down
  gpio_conf_init(pkeybutton);
  gpio_init_output(pkeybutton);

  //register & start timer
  pkeybutton->delay_timer.data=(unsigned long)pkeybutton;
  pkeybutton->delay_timer.function=delay_timer_fn;
  pkeybutton->delay_timer.expires=jiffies + HZ;
  add_timer(&pkeybutton->delay_timer);

fail:
  //unregister cdev & iounmap & del timer & kfree memory
  return result;
}


static void button_exit(void)
{

  //iounreamap
  iounmap((volatile unsigned int *)pkeybutton->gpio_mem[0]);
  iounmap((volatile unsigned int *)pkeybutton->gpio_mem[1]);
  iounmap((volatile unsigned int *)pkeybutton->gpio_mem[2]);
  iounmap((volatile unsigned int *)pkeybutton->gpio_mem[3]);

  //del timer
  del_timer(&pkeybutton->delay_timer);
#ifdef TASKLET_ENABLE
  //del tasklet
#endif

  //unregister character device
  cdev_del(&pkeybutton->cdev);

  kfree(pkeybutton);
}

MODULE_AUTHOR("Faithere");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Keybutton driver for FS2410");
module_init(button_init);
module_exit(button_exit);

