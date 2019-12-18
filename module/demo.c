#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>

#define BASEMINOR 0
#define COUNT 3
#define NAME "chardev_demo"

dev_t devno;
struct cdev* cdevp = NULL;

int demo_open(struct inode* inode, struct file* file)
{
	printk(KERN_INFO"[%s][%s][%d]\n", __FILE__, __func__, __LINE__);
	return 0;
}

int demo_release(struct inode* inode, struct file* file)
{
	printk(KERN_INFO"[%s][%s][%d]\n", __FILE__, __func__, __LINE__);
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.release = demo_release,
};

int __init demo_init(void)
{
	int ret = 0;
	ret = alloc_chrdev_region(&devno, BASEMINOR, COUNT, NAME);
	if(ret < 0){
		printk(KERN_ERR"alloc_chrdev_region failed.\n");
		goto err1;

	}
	printk(KERN_INFO"devno = %d\n", MAJOR(devno));

	cdevp = cdev_alloc();
	if(cdevp == NULL){
		printk(KERN_ERR"cdev_alloc failed.\n");
		ret = -ENOMEM;
		goto err2;
	}

	cdev_init(cdevp, &fops);

	ret = cdev_add(cdevp, devno, COUNT);
	if(ret < 0){
		printk(KERN_ERR"cdev_add failed.\n");
		goto err3;
	}

	return 0;
err3:
	kfree(cdevp);
	cdevp = NULL;
err2:
	unregister_chrdev_region(devno, COUNT);
err1:
	return ret;
}

void __exit demo_exit(void)
{
	cdev_del(cdevp);
	if(cdevp)kfree(cdevp);

	unregister_chrdev_region(devno, COUNT);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");

