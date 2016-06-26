/*  
 *  First Kernel Program
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

/* Initializations */
int init_module(void)
{
	printk(KERN_INFO "Saurabh Jagdhane\n");
	return 0;
}

/* Cleanup */
void cleanup_module(void)
{
	printk(KERN_INFO "\n");
}
