#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/cdev.h>


/*******************Glb Data*******************/
int button_major;
int button_minor;
int button_dev_num=1;


//key button character device data struct
struct keybutton
{
  
  struct cdev cdev;
}

/*********************************************/

static int button_init(void)
{
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
  

  


}


static void button_exit(void)
{
}

MODULE_AUTHOR("Faithere");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Keybutton driver for FS2410");
module_init(button_init);
module_exit(button_exit);

