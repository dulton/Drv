
/**
  * This is a driver for mcp3002 device .
  * File:			mcp3002.c
  * Description:		Install all needed software from ubuntu software center.
  * Created Date:	May 25th. 2011
  * Last Modified:	Mayl 25th. 2011
  * Author:	Wu DaoGuang  <wdgvip@gmail.com>
  * Copywrite (C)	 GPL/GPL2 
  *
  *
  */


#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/version.h>
 
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/ioport.h>

#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/serial_reg.h>
#include <linux/circ_buf.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kthread.h>

#include <linux/workqueue.h>

#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/irq.h>
#include <asm/system.h>

#include <asm/uaccess.h>

#include <linux/moduleparam.h>

#include <asm/uaccess.h>

#include <asm/arch/gpio.h>
#include <asm/arch/platform.h>
#include <asm/arch/yx_gpiodrv.h>
#include<asm/arch/yx_iomultplex.h>




#include <linux/i2c.h>
#include <linux/i2c-id.h>




#undef PDEBUG 
#define HI3511_SPI_DEBUG   1
#ifdef    HI3511_SPI_DEBUG
	#define PDEBUG(fmt, args...)		printk( KERN_INFO "mcp3002-- " fmt, ##args)
#else
	#define PDEBUG(fmt, args...)    
#endif




#define HI3511_MCP3002_MAJOR   235

static int mcp3002_major = HI3511_MCP3002_MAJOR;



struct hi3511_mcp3002_dev
{
	struct cdev cdev;
	int irq;
	
	u8  *tx;
	u8  *rx;
	u32 chan_id;
	
};

struct hi3511_mcp3002_dev *hi3511_mcp3002_devp;




int hi3511_mcp3002_open(struct inode *inode, struct file *filp)
{

return 0;
}

int hi3511_mcp3002_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static int hi3511_mcp3002_ioctl(struct inode *inodep, struct file *filp, unsigned int cmd, unsigned long arg)
{

return 0;
}

static ssize_t hi3511_mcp3002_read(struct file *filp, char __user  *buf, size_t size, loff_t *ppos)
{
	return 0;
}

static ssize_t hi3511_mcp3002_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{

return 0;
}
static const struct file_operations hi3511_mcp3002_fops ={
	.owner	= THIS_MODULE,
	.read	= hi3511_mcp3002_read,
	.write 	= hi3511_mcp3002_write,
	.ioctl 	= hi3511_mcp3002_ioctl,
	.open 	= hi3511_mcp3002_open,
	.release 	= hi3511_mcp3002_release,
};


static void hi3511_mcp3002_setup_cdev(struct hi3511_mcp3002_dev *dev, int index)

{
	int err;
	int devno = MKDEV(mcp3002_major, index);
	cdev_init(&dev->cdev, &hi3511_mcp3002_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops      = &hi3511_mcp3002_fops;
	err = cdev_add(&dev->cdev, devno, 1);
	if(err)
		{
			PDEBUG("err in cdev add !\n");
		}
}



static int hi3511_mcp3002_init(void)
{
	int result ;
	int value =0;
	unsigned char temp,temp1;
	dev_t devno = MKDEV(mcp3002_major,0);

	if(mcp3002_major)
		result = register_chrdev_region(devno, 1, "hi3511_mcp3002");
	else
		{
			result = alloc_chrdev_region(&devno, 0, 1, "hi3511_mcp3002");
			mcp3002_major = MAJOR(devno);
		}
	hi3511_mcp3002_devp = kmalloc(sizeof(struct hi3511_mcp3002_dev), GFP_KERNEL);
	
	if(!hi3511_mcp3002_devp)
		{
			result = - ENOMEM;
			goto fail_malloc;
		}
	memset(hi3511_mcp3002_devp, 0, sizeof(struct hi3511_mcp3002_dev));

	hi3511_mcp3002_setup_cdev(hi3511_mcp3002_devp, 0);

	while(1)
		{
	SPISendByte(0x60, 1);
	mdelay(1000);
	temp = SPIRecvByte(1);
	temp1 =SPIRecvByte(1);
	PDEBUG("temp is :%02x,temp1 is :%02x\n",temp,temp1);
		}
	
	PDEBUG("the spi read value is :%8x\n",value);
	return 0;
	
	fail_malloc:

		unregister_chrdev_region(MKDEV(mcp3002_major, 0), 1);
	
	return result;
}



static void hi3511_mcp3002_exit(void)
{
	cdev_del(&hi3511_mcp3002_devp->cdev);
	kfree(hi3511_mcp3002_devp);
	unregister_chrdev_region(MKDEV(mcp3002_major, 0), 1);
	
	PDEBUG("xxx driver exit !\n");
}

module_init(hi3511_mcp3002_init);
module_exit(hi3511_mcp3002_exit);

MODULE_AUTHOR("Wu DaoGuang");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Mcp3002  driver !");
MODULE_ALIAS("mcp3002 driver ");









