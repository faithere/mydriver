/*
 * File Name: hello.c
 *
 * Descriptions: 
 *		The simple example of kernel module code.
 *
 * Author: 
 *		Mike Lee
 * Kernel Version: 2.6.20
 *
 * Update:
 * 		-	2007.07.13	Mike Lee	 Creat this file
 */

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
char *who="world";

static int hello_init(void)
{
        printk(KERN_ALERT "Hello, %s!\n", who);
        return 0;
}

static void hello_exit(void)
{
        printk(KERN_ALERT "Goodbye, %s!\n",who);
}

module_init(hello_init);
module_exit(hello_exit);