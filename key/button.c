/*
 * File Name: button.c
 *
 * Descriptions: 
 *		The example of buttons module driver code.
 *
 * Author: 
 *		Mike Lee
 * Kernel Version: 2.6.20
 *
 * Update:
 * 		-	2007.07.23	Mike Lee	 Creat this file
 */
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/hardware.h>
#include <asm/delay.h>
#include <asm/uaccess.h>
#include <asm-arm/arch-s3c2410/regs-gpio.h>
#include <linux/irq.h>
#include <asm/io.h>
#include <asm-arm/arch-s3c2410/irqs.h>
#include <asm-arm/irq.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/cdev.h>

#define     BUTTON_IRQ1 IRQ_EINT0
#define     BUTTON_IRQ2 IRQ_EINT2
#define     BUTTON_IRQ3 IRQ_EINT11
#define     BUTTON_IRQ4 IRQ_EINT19
#define     DEVICE_NAME "buttons"
#define     BUTTONMINOR 0
#define     MAX_BUTTON_BUF  16
    
#define NOKEY     0

static int buttonMajor=0;
static unsigned char buttonRead(void);
static int flag=0;

typedef struct {
    unsigned int buttonStatus;      
    unsigned char buf[MAX_BUTTON_BUF]; 
    unsigned int head,tail;         
    wait_queue_head_t wq; 
    struct cdev cdev;         
} BUTTON_DEV;

static BUTTON_DEV buttondev;

#define BUF_HEAD    (buttondev.buf[buttondev.head])     
#define BUF_TAIL    (buttondev.buf[buttondev.tail])    
#define INCBUF(x,mod)   ((++(x)) & ((mod)-1))      

static void (*buttonEvent)(void);

static void buttonEvent_dummy(void) {}

static void delay(int n)
{
        long int t;

        t=n*100;
        while(t > 0)
        {
                t--;
        }
}

static void buttonEvent_1(void)
{
        if(buttondev.buttonStatus==8) 
        {
                BUF_HEAD=8;
        }
        else if(buttondev.buttonStatus==7) 
        {
                BUF_HEAD=7;
        } 
        else if(buttondev.buttonStatus==6) 
        {
                BUF_HEAD=6;
        }
        else if(buttondev.buttonStatus==5)
        {
                BUF_HEAD=5;
        }
        else if(buttondev.buttonStatus==4)
        {
                BUF_HEAD=4;
        }
        else if(buttondev.buttonStatus==3)
        {
                BUF_HEAD=3;
        }
        else if(buttondev.buttonStatus==2)
        {
                BUF_HEAD=2;
        }
        else if(buttondev.buttonStatus==1)
        {
                BUF_HEAD=1;
        }
        else
                BUF_HEAD=NOKEY;
        buttondev.head=INCBUF(buttondev.head,MAX_BUTTON_BUF);
        flag=1;
        wake_up_interruptible(&(buttondev.wq));
        printk("buttonEvent_1\n");
}

static irqreturn_t isr_button(int irq,void *dev_id,struct pt_regs *regs)
{
     unsigned long GPBDAT,GPFDAT,GPGDAT;

     GPBDAT=(unsigned long)ioremap(0x56000014,4);
     GPFDAT=(unsigned long)ioremap(0x56000054,4);
     GPGDAT=(unsigned long)ioremap(0x56000064,4);

    printk("Occured key board Inetrrupt,irq=%d\n",irq-44);
    switch (irq) {
    case BUTTON_IRQ1:
                (*(volatile unsigned long *)GPBDAT) &=~(1<<6);
                (*(volatile unsigned long *)GPBDAT) |=1<<7;
                if((*(volatile unsigned long *)GPFDAT) &(1<< 0)==0) 
                        buttondev.buttonStatus=1;
                delay(900);
                (*(volatile unsigned long *)GPBDAT) &=~(1<<7);                                   
                (*(volatile unsigned long *)GPBDAT) |=1<<6;
                if((*(volatile unsigned long *)GPFDAT) &(1<< 0)==0)
                        buttondev.buttonStatus=2;

                break;
    case BUTTON_IRQ2:
                (*(volatile unsigned long *)GPBDAT) &=~(1<<6);                                   
                (*(volatile unsigned long *)GPBDAT) |=1<<7;   
                if((*(volatile unsigned long *)GPFDAT) &(1<< 2)==0)
                        buttondev.buttonStatus=3;
                delay(900);
                (*(volatile unsigned long *)GPBDAT) &=~(1<<7);                
                (*(volatile unsigned long *)GPBDAT) |=1<<6;
                if((*(volatile unsigned long *)GPFDAT) &(1<< 2)==0)
                        buttondev.buttonStatus=4;
                break;
    case BUTTON_IRQ3:
                (*(volatile unsigned long *)GPBDAT) &=~(1<<6);                                   
                (*(volatile unsigned long *)GPBDAT) |=1<<7;   
                if((*(volatile unsigned long *)GPGDAT) &(1<< 3)==0)
                        buttondev.buttonStatus=5;
                delay(900);
                (*(volatile unsigned long *)GPBDAT) &=~(1<<7);                
                (*(volatile unsigned long *)GPBDAT) |=1<<6;
                if((*(volatile unsigned long *)GPGDAT) &(1<< 3)==0)
                        buttondev.buttonStatus=6;
                break;
    case BUTTON_IRQ4:
                (*(volatile unsigned long *)GPBDAT) &=~(1<<6);                                   
                (*(volatile unsigned long *)GPBDAT) |=1<<7;   
                if((*(volatile unsigned long *)GPGDAT) &(1<< 11)==0)
                        buttondev.buttonStatus=7;
                delay(900);
                (*(volatile unsigned long *)GPBDAT) &=~(1<<7);                
                (*(volatile unsigned long *)GPBDAT) |=1<<6;
                if((*(volatile unsigned long *)GPGDAT) &(1<< 11)==0)
                        buttondev.buttonStatus=8;
                break;    
    default:
    		break;
     }
    
    buttonEvent();
    return 0;
}

static int button_open(struct inode *inode,struct file *filp) 
{
        int ret;
        buttondev.head=buttondev.tail=0;
        buttonEvent=buttonEvent_1;  

        ret=request_irq(BUTTON_IRQ1,isr_button,SA_INTERRUPT,DEVICE_NAME,NULL);
        if(ret) 
        {
                printk("BUTTON_IRQ1: could not register interrupt\n");
                return ret;
        }
        ret=request_irq(BUTTON_IRQ2,isr_button,SA_INTERRUPT,DEVICE_NAME,NULL);
        if(ret) 
        {
                printk("BUTTON_IRQ2: could not register interrupt\n");
                return ret;
        }
        ret=request_irq(BUTTON_IRQ3,isr_button,SA_INTERRUPT,DEVICE_NAME,NULL);
        if(ret) 
        {
                printk("BUTTON_IRQ3: could not register interrupt\n");
                return ret;
        }     
        ret=request_irq(BUTTON_IRQ4,isr_button,SA_INTERRUPT,DEVICE_NAME,NULL);
        if(ret) 
        {
                printk("BUTTON_IRQ4: could not register interrupt\n");
                return ret;
        }
        return 0;
}

static int button_release(struct inode *inode,struct file *filp)
{
        buttonEvent=buttonEvent_dummy;
        free_irq(BUTTON_IRQ1,NULL);
        free_irq(BUTTON_IRQ2,NULL);
        free_irq(BUTTON_IRQ3,NULL);
        free_irq(BUTTON_IRQ4,NULL);
        return 0;
}

static unsigned char buttonRead(void)
{
        unsigned char button_ret;
        button_ret=BUF_TAIL;
        buttondev.tail=INCBUF(buttondev.tail,MAX_BUTTON_BUF);
        return button_ret;
}  

static ssize_t button_read(struct file *filp,char *buffer,size_t count,loff_t *ppos)
{
        static unsigned char button_ret;
retry:
        printk("retry start\n");
        if(buttondev.head!=buttondev.tail) 
        {
                button_ret=buttonRead();
                copy_to_user(buffer,(char *)&button_ret,sizeof(unsigned char));
                printk("the button_ret is 0x%x\n",button_ret);
                return sizeof(unsigned char);
        }
        else 
        {
                if(filp->f_flags & O_NONBLOCK)
                        return -EAGAIN;
                printk("sleep\n");

                wait_event_interruptible(buttondev.wq,flag);
                flag=0;
                printk("sleep_after\n");
                if(signal_pending(current))
                {
                        printk("rturn -ERESTARTSYS\n");
                        return -ERESTARTSYS;
                }
                goto retry;
        }

        return sizeof(unsigned char);
}

static struct file_operations button_fops= {
        .owner  =   THIS_MODULE,
        .open   =   button_open,
        .read   =   button_read,
        .release    =   button_release,
};

static int __init s3c2410_buttons_init(void)
{
        int ret, err;
        dev_t dev = MKDEV(buttonMajor, 0);

        set_irq_type(BUTTON_IRQ1,IRQT_FALLING);
        set_irq_type(BUTTON_IRQ2,IRQT_FALLING);
        set_irq_type(BUTTON_IRQ3,IRQT_FALLING);
        set_irq_type(BUTTON_IRQ4,IRQT_FALLING);
 
        buttonEvent=buttonEvent_dummy;

        if (buttonMajor)
                ret = register_chrdev_region(dev, 1, DEVICE_NAME);
        else 
        {
                ret = alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
                buttonMajor = MAJOR(dev);
        }
        if (ret < 0)
                return ret;

        cdev_init(&buttondev.cdev, &button_fops);
        buttondev.cdev.owner = THIS_MODULE;
        buttondev.cdev.ops = &button_fops;
        err = cdev_add (&buttondev.cdev, dev, 1);
        if (err)
                printk(KERN_NOTICE "Error %d adding button", err);

        buttondev.buttonStatus=NOKEY;
        init_waitqueue_head(&(buttondev.wq));
        printk(DEVICE_NAME "initialized\n");
        	return 0;
}
 
static void __exit s3c2410_buttons_exit(void)
{
        cdev_del(&buttondev.cdev);
        unregister_chrdev_region(MKDEV (buttonMajor, 0), 1);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mike Lee");
MODULE_DESCRIPTION ("The buttons char device driver");

module_init(s3c2410_buttons_init);
module_exit(s3c2410_buttons_exit);