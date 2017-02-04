#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include "ioctl-drv.h"
#define DRIVER_NAME	"ioctl"
static	int	device_major_number =	0;


/* This function handles ioctl for the character device */
static int chrdev_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	/* See which ioctl we are doing */
	switch (cmd) {
	case IOCTL_TEST1:
		printk(KERN_NOTICE "Test 1 \n");
		break;
	case IOCTL_TEST2:
		printk(KERN_NOTICE "Test 2 \n");
		break;
	default:
		 printk(KERN_NOTICE "Test noting \n");
		break;
	}
	return 0;
}

/* This function handles open for the character device */

static int ioctl_chrdev_open(struct inode *inode, struct file *file)
{
	printk(KERN_NOTICE "The character device is opened! \n");
	return 0;
}

/* File operations struct for character device */

static const struct file_operations ioctl_fops = {
	.owner		= THIS_MODULE,
	.ioctl		= chrdev_ioctl,
	.open		= ioctl_chrdev_open,
	.release	= NULL,
};

static int __init ioctl_init (void)
{
	int Rv = 0;
	device_major_number = register_chrdev(0, DRIVER_NAME, &ioctl_fops);
	printk(KERN_ALERT "The ioctl device major number is %d ! \n",device_major_number);
	return Rv;
}

static void __exit ioctl_cleanup( void )
{
 	unregister_chrdev( device_major_number, DRIVER_NAME );
}

module_init (ioctl_init);
module_exit (ioctl_cleanup);
