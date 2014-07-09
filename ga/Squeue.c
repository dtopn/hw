/* ----------------------------------------------- DRIVER Squue --------------------------------------------------

 ----------------------------------------------------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/jiffies.h>

#include <linux/init.h>
#include <linux/moduleparam.h>

//#include <linux/hpet.h>

#define DEVICE_NAME                 "Squeue"  // device name to be created and registered
#define QUEUE_LENGTH 10

#include "Token.h"

/* per device structure */
struct Squeue_dev {
	struct cdev cdev;               /* The cdev structure */
	char name[20];                  /* Name of device*/
	struct Token token_queue[QUEUE_LENGTH];		/* Token storage */
	//TODO: change to pointer
	int mutex_lock; 		/* Mutex lock */
	//TODO: use kernel mux
	int queue_size;			/* Size of the queue */
	char in_string[256];
	int current_write_pointer;
	int current_read_pointer;
} *Squeue_devp1, *Squeue_devp2;

static dev_t Squeue_dev_number;      /* Allotted device number */
struct class *Squeue_dev_class;          /* Tie with the device model */
static struct device *Squeue_dev_device;

static char *user_name = "Yuanzhang Hao";

module_param(user_name,charp,0000);	//to get parameter from load.sh script to greet the user

/*
* Open Squeue driver
*/
int Squeue_driver_open(struct inode *inode, struct file *file)
{
	struct Squeue_dev *Squeue_devp;

	/* Get the per-device structure that contains this cdev */
	Squeue_devp = container_of(inode->i_cdev, struct Squeue_dev, cdev);


	/* Easy access to cmos_devp from rest of the entry points */
	file->private_data = Squeue_devp;
	printk("\n%s openning \n", Squeue_devp->name);
	return 0;
}

/*
 * Release Squeue driver
 */
int Squeue_driver_release(struct inode *inode, struct file *file)
{
	struct Squeue_dev *Squeue_devp = file->private_data;
	
	printk("\n%s closing\n", Squeue_devp->name);
	
	return 0;
}

/*
 * Write to Squeue driver
 */
ssize_t Squeue_driver_write(struct file *file, const char *buf,
           size_t count, loff_t *ppos)
{
	int bytes_fail = 0;
	struct Squeue_dev *Squeue_devp = file->private_data;
	struct Token *tokenp;
	int *mutex_lockp = &(Squeue_devp->mutex_lock); /* Get the lock for the specific device */

	while (*mutex_lockp); /* Wait until the lock is available */


	*mutex_lockp = 1; /* Assert lock */
	if (QUEUE_LENGTH == Squeue_devp->queue_size) {
		/* Queue is full */
		printk("Queue is full"); //TODO: add identifier
		return -1;
	}
	else {
		tokenp = &(Squeue_devp->token_queue[Squeue_devp->current_write_pointer]);
	}
	/*while (count) {		
		get_user(Squeue_devp->in_string[Squeue_devp->current_write_pointer], buf++);
		count--;
		if(count){
			Squeue_devp->current_write_pointer++;
			if( Squeue_devp->current_write_pointer == 256)
				Squeue_devp->current_write_pointer = 0;
		}
	}*/
	/* Truncate size to 80 */
	if (count > 80) count = 80;
	bytes_fail = copy_from_user(tokenp->msg, buf, count);
	tokenp->msg[count] = '\0'; /* Terminate string */
	//TODO: add time


	printk("Written -- %d %s \n", Squeue_devp->current_write_pointer, tokenp->msg);
	/* Increase pointer, counter */
	Squeue_devp->queue_size++;
	if (++Squeue_devp->current_write_pointer == QUEUE_LENGTH) {
		Squeue_devp->current_write_pointer = 0;
	}
	*mutex_lockp = 0; /* Release lock */

	return bytes_fail;
}
/*
 * Read from Squeue driver
 */
ssize_t Squeue_driver_read(struct file *file, char *buf,
           size_t count, loff_t *ppos)
{
	int bytes_read, bytes_fail;
	struct Squeue_dev *Squeue_devp = file->private_data;
	struct Token *tokenp;
	/* If the queue is empty return -1 signifying enpty queue */
	if (0 == Squeue_devp->queue_size) {
		/* Queue is empty */
		printk("Queue is empty"); //TODO: add identifier
		return -1;
	}
	/* 
	 * Actually put the data into the buffer 
	 */
	/*while (count && Squeue_devp->in_string[bytes_read]) {
		put_user(Squeue_devp->in_string[bytes_read], buf++);
		count--;
		bytes_read++;
	}*/
	tokenp = Squeue_devp->token_queue + Squeue_devp->current_read_pointer;
	bytes_fail = copy_to_user(buf, tokenp, count);

	printk("Read from %s -- '%s'\n", Squeue_devp->name, tokenp->msg);
		if (++Squeue_devp->current_read_pointer == QUEUE_LENGTH) {
		Squeue_devp->current_read_pointer = 0;
	}
	Squeue_devp->queue_size--;

	/* 
	 * Most read functions return the number of bytes put into the buffer
	 */
	bytes_read = count - bytes_fail;
	return bytes_read;

}

/* File operations structure. Defined in linux/fs.h */
static struct file_operations Squeue_fops = {
    .owner		= THIS_MODULE,           /* Owner */
    .open		= Squeue_driver_open,        /* Open method */
    .release		= Squeue_driver_release,     /* Release method */
    .write		= Squeue_driver_write,       /* Write method */
    .read		= Squeue_driver_read,        /* Read method */
};

/*
 * Driver Initialization
 */
int __init Squeue_driver_init(void)
{
	int ret;
	int time_since_boot;

	/* Request dynamic allocation of a device major number */
	if (alloc_chrdev_region(&Squeue_dev_number, 1, 2, DEVICE_NAME) < 0) {
			printk(KERN_DEBUG "Can't register device\n"); return -1;
	}

	/* Populate sysfs entries */
	Squeue_dev_class = class_create(THIS_MODULE, DEVICE_NAME);

	/* Allocate memory for the per-device structure */
	Squeue_devp1 = kmalloc(sizeof(struct Squeue_dev), GFP_KERNEL);
	Squeue_devp2 = kmalloc(sizeof(struct Squeue_dev), GFP_KERNEL);
		
	if (!Squeue_devp1 || !Squeue_devp2) {
		printk("Bad Kmalloc\n"); return -ENOMEM;
	}

	/* Request I/O region */
	sprintf(Squeue_devp1->name, "%s1", DEVICE_NAME);
	sprintf(Squeue_devp2->name, "%s2", DEVICE_NAME);

	/* Connect the file operations with the cdev */
	cdev_init(&Squeue_devp1->cdev, &Squeue_fops);
	cdev_init(&Squeue_devp2->cdev, &Squeue_fops);
	Squeue_devp1->cdev.owner = THIS_MODULE;
	Squeue_devp2->cdev.owner = THIS_MODULE;

	/* Connect the major/minor number to the cdev */
	//ret  = cdev_add(&Squeue_devp1->cdev, (Squeue_dev_number), 1);
	ret  = cdev_add(&Squeue_devp1->cdev, MKDEV(MAJOR(Squeue_dev_number), 1), 1);
	ret |= cdev_add(&Squeue_devp2->cdev, MKDEV(MAJOR(Squeue_dev_number), 2), 1);

	if (ret) {
		printk("Bad cdev\n");
		return ret;
	}

	/* Send uevents to udev, so it'll create /dev nodes */
	Squeue_dev_device = device_create(Squeue_dev_class, NULL, MKDEV(MAJOR(Squeue_dev_number), 1), NULL, Squeue_devp1->name);
	Squeue_dev_device = device_create(Squeue_dev_class, NULL, MKDEV(MAJOR(Squeue_dev_number), 2), NULL, Squeue_devp2->name);	
	// device_create_file(Squeue_dev_device, &dev_attr_xxx);

	memset(Squeue_devp1->in_string, 0, 256);
	memset(Squeue_devp2->in_string, 0, 256);
	
	time_since_boot=(jiffies-INITIAL_JIFFIES)/HZ;//since on some systems jiffies is a very huge uninitialized value at boot and saved.
	//printk(Squeue_devp->in_string, "Hi %s, this machine has been on for %d seconds", user_name, time_since_boot);
	
	Squeue_devp1->current_write_pointer = 0;
	Squeue_devp2->current_write_pointer = 0;
	Squeue_devp1->current_read_pointer = 0;
	Squeue_devp2->current_read_pointer = 0;
	Squeue_devp1->mutex_lock = 0;
	Squeue_devp2->mutex_lock = 0;
	Squeue_devp1->queue_size = 0;
	Squeue_devp2->queue_size = 0;

	printk("Squeue driver initialized.\n");// '%s'\n",Squeue_devp->in_string);

	//printk("Address Of Squeue_driver_read %p\n", &Squeue_driver_read);
	//printk("Address Of Squeue_fops %p\n", &Squeue_fops);
	//printk("Address Of is_hpet_enabled %p\n", &is_hpet_enabled);
	//printk("Address Of jiffies %p\n", &jiffies);
	//printk("Address Of jiffies_64 %p\n", &jiffies_64);
	return 0;
}
/* Driver Exit */
void __exit Squeue_driver_exit(void)
{
	// device_remove_file(Squeue_dev_device, &dev_attr_xxx);
	/* Release the major number */
	unregister_chrdev_region((Squeue_dev_number), 2);

	/* Destroy device */
	device_destroy (Squeue_dev_class, MKDEV(MAJOR(Squeue_dev_number), 1));
	device_destroy (Squeue_dev_class, MKDEV(MAJOR(Squeue_dev_number), 2));
	cdev_del(&Squeue_devp1->cdev);
	cdev_del(&Squeue_devp2->cdev);
	kfree(Squeue_devp1);
	kfree(Squeue_devp2);
	
	/* Destroy driver_class */
	class_destroy(Squeue_dev_class);

	printk("Squeue driver removed.\n");
}

module_init(Squeue_driver_init);
module_exit(Squeue_driver_exit);
MODULE_LICENSE("GPL v2");
