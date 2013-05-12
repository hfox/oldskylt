/*
 * bs.c - a driver for the bs device.
 *
 * written 1997 by Stefan Persson.
 *
 * Revision history:
 *
 * 1997-09-22:	v0.1, initial revision; just some basic structures.
 * 1997-09-23:	added code for reading
 * 1997-09-29:	added code for writing
 * 1997-10-03:	verified the writing code, added experimental
 *		support for bank-switching.
 *		Increased the version number to 0.20, reflecting the
 *		possible actual usability of the driver.
 */

#include <linux/module.h>

#include <linux/config.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/malloc.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>

#include "bs.h"

#define BS_BANNER "Basskylt interface driver, version 0.20"

#define BS_MAJOR 120	/* 120 is reserved for local/experimental use */
#define DEF_BS_PORT 0x340
#define DEF_BS_ADDR 0xb0000
#define BS_MEM_SIZE 2048	/* 2KB on-board RAM */
#define BS_DEBUG 0

static int ioaddr = 0;
static int memaddr = 0;
static int debug_val = BS_DEBUG;

static long bs_write(struct inode * inode, struct file * file,
		     const char * buf, unsigned long count)
{
	unsigned long p = file->f_pos;

	if(p >= memaddr+BS_MEM_SIZE)
	    return 0;

	if(count > memaddr + BS_MEM_SIZE - p)
	    count = memaddr + BS_MEM_SIZE - p;

	if(debug_val > 1) {
		printk(KERN_DEBUG "writing %ld bytes to physical address 0x%lx"
		       " (virtual address 0x%lx)\n", count, p, (long)__va(p));
	}
	if(copy_from_user(__va(p), buf, count) < 0)
	    return -EFAULT;

 	return count;
}

static long long bs_lseek(struct inode * inode, struct file * file,
			  long long offset, int orig)
{
	switch(orig) {
		case 0: /* seek from start */
			if(offset > BS_MEM_SIZE)
				return -ESPIPE;
			file->f_pos = offset + memaddr;
			return offset;
		case 1:
			if(file->f_pos+offset > memaddr + BS_MEM_SIZE)
				return -ESPIPE;
			file->f_pos += offset;
			return file->f_pos - memaddr;
		default:
			return -EINVAL;
	}
	return -ESPIPE;
}

static long bs_read(struct inode * inode, struct file * file, 
		    char * buf, unsigned long count)
{
	unsigned long p = file->f_pos;
	unsigned long end_mem = memaddr + BS_MEM_SIZE;
	unsigned long read = 0;

	if(p >= end_mem)
		return 0;
	if(count > end_mem - p)
		count = end_mem - p;

	/*
	 * __va() translates the physical address to the
	 * kernel's address space
	 */
	if((copy_to_user(buf, __va(p), count)) < 0)
		return -EFAULT;

	read += count;
	file->f_pos += read;

	return read;
}


static int bs_open(struct inode * inode, struct file * file)
{
	unsigned int minor = MINOR(inode->i_rdev);

	if(minor > 0)
		return -ENODEV;

	MOD_INC_USE_COUNT;
	file->f_pos = memaddr;
	return 0;
}

static int bs_release(struct inode * inode, struct file * file)
{
	MOD_DEC_USE_COUNT;
	return 0;
}


static int bs_ioctl(struct inode *inode, struct file *file,
		    unsigned int cmd, unsigned long arg)
{
	unsigned int minor = MINOR(inode->i_rdev);
	int retval = 0;
	unsigned char newval, oldval;

	switch(cmd) {
		case BSRESET:
			printk(KERN_INFO "bs%d: restarting\n", minor);
			break;
			
		case BSSETBANK:
			newval = *((char *)arg);
			oldval = inb(ioaddr);
			newval = (newval & 3) | oldval;
			outb(newval, ioaddr);
			printk(KERN_DEBUG "bs%d: switching bank\n", minor);
			break;

		default:
			retval = -EINVAL;
	}
	return retval;
}


static struct file_operations bs_fops = {
	bs_lseek,
	bs_read,
	bs_write,
	NULL,		/* bs_readdir */
	NULL,		/* bs_poll */
	bs_ioctl,
	NULL,		/* bs_mmap */
	bs_open,
	bs_release
};

#ifdef MODULE
#define MAX_CARDS 1

static int io[MAX_CARDS] = { 0, };
static int mem[MAX_CARDS] = { 0, };
static int debug[MAX_CARDS] = { BS_DEBUG, };

MODULE_PARM(io, "1-" __MODULE_STRING(MAX_CARDS) "i");
MODULE_PARM(mem, "1-" __MODULE_STRING(MAX_CARDS) "i");
MODULE_PARM(debug, "1-" __MODULE_STRING(MAX_CARDS) "i");

int init_module(void)
{
	if (register_chrdev(BS_MAJOR, "bs", &bs_fops)) {
		printk("lp: unable to get major %d\n", LP_MAJOR);
		return -EIO;
	}

	if(io[0]) {
		/* 
		 * someone specified a port on the cmd-line;
		 * don't try any other ports.
		 */
		ioaddr = io[0];
	} else {
		/*
		 * should try to detect the board on all possible
		 * addresses here.
		 */
		ioaddr = DEF_BS_PORT;
	}
	debug_val = debug[0];
	memaddr = mem[0] ? mem[0] : DEF_BS_ADDR;

	printk(KERN_INFO "bs: %s\n", BS_BANNER);
	printk(KERN_INFO "bs: using memory address 0x%x , port 0x%x\n",
	       memaddr, ioaddr);
	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(BS_MAJOR, "bs");
}
#endif

