/**
 * @file cdcm-driver-gen.c
 *
 * @brief VME-specific operations
 *
 * @author Copyright (C) 2009 CERN. Yury GEORGIEVSKIY <ygeorgie@cern.ch>
 *
 * @date Created on 08/06/2009
 *
 * @section license_sec License
 *          Released under the GPL
 */
#include "list_extra.h" /* for extra handy list operations */
#include "cdcmDrvr.h"
#include "cdcmLynxAPI.h"
#include "vmebus.h"
#define __CDCM__
#include "dg/ModuleHeader.h"	/* for DevInfo_t */


/* _GIOCTL_GET_DG_DEV_INFO ioctl argument
   Keep syncronized with one from user space! */
#define DDNMSZ    32   /* device/driver name size in bytes. Same as in Lynx */
struct dg_module_info {
	int dev_major;		/* device major number */
	int drvr_major;		/* driver major number */
	char name[DDNMSZ];	/* its name */
};

static DEFINE_MUTEX(mmutex);

/* external crap */
extern cdcmStatics_t cdcmStatT; /* CDCM statics table */
extern struct dldd entry_points; /* declared in the user driver part */
extern char* read_info_file(char*, int*);
extern struct class *cdcm_class;

/**
 * @brief driverGen devices
 *
 * list  -- linked list
 * major -- device major number
 * addr  -- statics table address
 * name  -- device name. String of DEVICE_NAME_FMT layout.
 *          Examples: sharedcibc_sim03.info, tric_drvr20.info, etc...
 */
struct dgd {
	struct list_head list;
	unsigned int major;
	char *addr;
	char *name;
};

/**
 * @brief list of device mapped memory
 *
 * list --
 * addr --
 */
struct dgmmap {
	struct list_head list;
	DevInfo_t *addr;
};

static LIST_HEAD(st_list); /* driverGen device linked list */
static LIST_HEAD(mmap_list); /* mapped memory list */

/**
 * @brief Get device info table from driver statics table.
 *
 * @param di -- device info
 *
 * @return device info table pointer
 */
static DevInfo_t* get_dit_from_statics(void *stat)
{
	/* all dg statics tables are of standard layout */
	struct static_layout {
		void *topology;
		DevInfo_t *info;
		void *usrst;
	} *lo;

	lo = (struct static_layout*)stat;
	return lo->info;
}

/**
 * @brief Add new statics table in the list
 *
 * @param major -- device major number
 * @param st    -- statics table to add
 * @param dname -- device name
 *
 * @return -ENOMEM - can't allocate new statics table descriptor
 * @return 0       - success
 */
static int dg_add_st(uint major, char *st, char *dname)
{
	struct dgd *new_st;

	new_st = kzalloc(sizeof(*new_st), GFP_KERNEL);
	if (!new_st)
		return -ENOMEM;

	new_st->name = kasprintf(GFP_KERNEL, "%s", dname);
	if (!new_st->name) {
		kfree(new_st);
		return -ENOMEM;
	}

	new_st->major = major;
	new_st->addr  = st;

	list_add_tail(&new_st->list, &st_list);

	return 0;
}

/**
 * @brief Removes deviced description from the list and frees resources.
 *
 * @param dgd -- what to find and remove.
 *
 * @return zero on success or a negative number on failure
 */
static int dg_rm_st(struct dgd *dgd)
{
	struct dgd *ptr, *tmp;
	list_for_each_entry_safe(ptr, tmp, &st_list, list) {
		if (ptr == dgd) {
			list_del(&ptr->list); /* exclude from linked list */
			kfree(ptr->name);
			kfree(ptr);	/* free mem */
			return 0;
		}
	}
	return -1;
}

/**
 * @brief Get dgd structure by statics table
 *
 * @param st -- statics table address
 *
 * @return NULL    - not found
 * @return pointer - found
 *
 */
static __attribute__((unused)) struct dgd* dg_get_dgd_by_st(char *st)
{
	struct dgd *ptr;
	list_for_each_entry(ptr, &st_list, list) {
		if (ptr->addr == st)
			return ptr;
	}
	return NULL;
}


/**
 * @brief Get dgd structure by major number
 *
 * @param major -- device major number to find
 *
 * @return NULL    - not found
 * @return pointer - found
 */
static struct dgd* dg_get_dgd_by_major(uint major)
{
	struct dgd *ptr;
	list_for_each_entry(ptr, &st_list, list) {
		if (ptr->major == major)
			return ptr;
	}
	return NULL;
}

/**
 * @brief Get statics table address by major number
 *
 * @param major -- device major number to find statics table
 *
 * <long-description>
 *
 * @return statics table address - if found
 * @return NULL                  - not found
 */
static char* dg_get_st_by_major(uint major)
{
	struct dgd *ptr;

	list_for_each_entry(ptr, &st_list, list) {
		if (ptr->major == major)
			return ptr->addr;
	}

	return NULL;
}

/**
 * @brief Tune mapped VME windows info (one you can see in /proc/vme/windows)
 *
 * @param st -- statics table
 *
 * Tune info bits in order to see useful info about vmebridge mapped memory.
 *
 * @e mapping  and @e vme_taskinfo are private && defined in vme_window.c
 *
 * Instead of storing pid number and task name, which in our case will always
 * be one of the @b modinst program, will store LUN and driver name.
 * This info is of more interest for the user.
 *
 * Tuning is done only in case of real driver, as simulator doesn't map any
 * physical memory.
 */
static void tune_vmebridge_proc_info(void *st)
{
	int i;
	DevInfo_t *info = get_dit_from_statics(st);
	AddrInfo_t *aip = &info->addr1;
	struct vme_mapping *vmep;
	struct mapping {
		struct list_head	list;
		struct vme_mapping	desc;
		struct vme_taskinfo {
			pid_t	pid_nr;
			char	name[MODULE_NAME_LEN];
		} client;
	} *m;

	if (info->mlun < 0)	/* don't do for driver simulator */
		return;

	/* pass through all possible address spaces */
	for (i = 0; i < ASA; i++) {
		vmep = find_vme_mapping_from_addr(aip->baseAddr);
		if (!vmep)
			continue;
		m = container_of(vmep, struct mapping, desc);
		m->client.pid_nr = info->mlun;
		strncpy(m->client.name, THIS_MODULE->name, MODULE_NAME_LEN);
		++aip;
	}
}

/**
 * @brief Information about all controlled devices
 *
 * @param arg -- user space buffer to put results.
 *
 * <long-description>
 *
 * @return zero on success or a negative number on failure
 */
int dg_get_dev_info(unsigned long arg)
{
	struct dgd *ptr;
	struct dg_module_info __user *buf = (struct dg_module_info __user *)arg;
	struct dg_module_info local;

	list_for_each_entry(ptr, &st_list, list) {
		local.dev_major = ptr->major;
		local.drvr_major = cdcmStatT.cdcm_major;
		strncpy(local.name, ptr->name, sizeof(local.name));
		if (copy_to_user(buf, &local, sizeof(local)))
			return -EFAULT;
		++buf;
	}

	return 0;
}

/**
 * @brief How many devices driver hosts.
 *
 * @param void - none
 *
 * Used by driver installation/deinstallation programs basically.
 *
 * @return device driver amount
 */
int dg_get_mod_am(void)
{
	return list_capacity(&st_list);
}

/**
 * @brief driverGen cdv_install
 *
 * @param name  -- info table file name
 * @param fops  -- file operations
 * @param dnm   -- driver name
 *
 * Registers new char device and insert it in driverGen device linked list.
 *
 * @return   major char device number - if success
 * @return < 0                        - failed
 */
int dg_cdv_install(char *name, struct file_operations *fops, char *dnm)
{
	unsigned int maj;     /* assigned major number */
	char *dname;	      /* device name */
	char *st;	      /* statics table */
	int cc, i;
	char *it;		/* module-specific info table */
	DevInfo_t *info = NULL;

	/* get info table */
	it = read_info_file(name, NULL);
	if (!it) {
		PRNT_ABS_ERR("Can't read info file.");
		return -EFAULT;
	}

	dname = strrchr(name, '/') + 1; /* device name is the info file name */

	st = entry_points.dldd_install((void*)it);
	if (st == (char*)SYSERR) {
		PRNT_ABS_ERR("Installation vector failed for %s device.",
			     dname);
		cc = -EAGAIN;
		goto out_err;
	}

	maj = register_chrdev(0, dname, fops);
	if (maj < 0) {
		PRNT_ABS_ERR("Can't register %s character device", dname);
		cc = maj;
		goto out_err;
	}

	/* we can get info _only_ after it came from dldd_install,
	   as it is initialized there */
	info = get_dit_from_statics(st);

	/* declare ourselfs in sysfs */
	/* NOTE! busubox mkdev will __not__ create /dev nodes with
	 * dots "." in their names */
	for (i = 0; i < info->chan; i++) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
		device_create(cdcm_class, NULL, MKDEV(maj, i+1),
			      "%s.l%02d.c%02d", dnm, abs(info->mlun), i);
#else
		device_create(cdcm_class, NULL, MKDEV(maj, i+1), NULL,
			      "%s.l%02d.c%02d", dnm, (int) abs(info->mlun), i);
#endif	/* 2.6.28 */
	}

	/* tune vmebridge proc info */
	tune_vmebridge_proc_info(st);

	/* save statics table */
	cc = dg_add_st(maj, st, dname);
	if (cc) {
		PRNT_ABS_ERR("Can't add %s device (maj %d) in the"
			     " device linked list.\n", dname, maj);

		/* remove ourselfs from sysfs */
		for (i = 0; i < info->chan; i++)
			device_destroy(cdcm_class, MKDEV(maj, i));

		unregister_chrdev(maj, dname);
	} else {
		/* all OK, return allocated major dev number */
		cc = maj;
	}

 out_err:
	kfree(it);
	return cc;

}

/**
 * @brief driverGen cdv_uninstall
 *
 * @param filp -- file struct pointer
 * @param arg  -- user args
 *
 * @return zero on success or a negative number on failure
 */
int dg_cdv_uninstall(struct file *filp, unsigned long arg)
{
	uint major; /* device major number */
	struct dgd *dgd; /* device info */
	DevInfo_t *info;
	int i;

	if (copy_from_user(&major, (void __user*)arg, sizeof(major)))
		return -EFAULT;

	dgd = dg_get_dgd_by_major(major);
	if (!dgd) /* should not happen */
		return -ENODEV;

	info = get_dit_from_statics(dgd->addr);

	/* remove ourselfs from sysfs */
	for (i = 0; i < info->chan; i++)
		device_destroy(cdcm_class, MKDEV(dgd->major, i+1));

	if (entry_points.dldd_uninstall(dgd->addr)) {
		PRNT_ABS_ERR("Uninstall vector failed.");
		return -EAGAIN;
	}

	/* remove from sysfs */
	unregister_chrdev(dgd->major, dgd->name);


	if (dg_rm_st(dgd))
		/* should not happen */
		return -ENODEV;

	return 0;
}

/**
 * @brief read EP
 *
 * @param filp -- file pointer
 * @param buf  -- kernel buffer, already allocated by CDCM
 * @param size -- buffer size
 * @param off  -- file offset
 *
 * @return number of bytes actually copied, including zero - if succeed.
 * @return SYSERR                                          - if fails.
 */
ssize_t dg_fop_read(struct file *filp, char *buf, size_t size,
		    loff_t *off)
{
	char *priv; /* statics table */

	/* not checking for NULL as it's done by open() */
	priv = dg_get_st_by_major(imajor(filp->f_dentry->d_inode));

	return entry_points.dldd_read(priv, (struct cdcm_file *)filp,
				      buf, size);
}

/**
 * @brief write EP
 *
 * @param filp -- file pointer
 * @param buf  -- kernel buffer, already allocated by CDCM
 * @param size -- buffer size
 * @param off  -- file offset
 *
 * @return number of bytes actually copied, including zero - if succeed.
 * @return SYSERR                                          - if fails.
 */
ssize_t dg_fop_write(struct file *filp, const char *buf,
		     size_t size, loff_t *off)
{
	char *priv; /* statics table */

	/* not checking for NULL as it's done by open() */
	priv = dg_get_st_by_major(imajor(filp->f_dentry->d_inode));

	return entry_points.dldd_write(priv, (struct cdcm_file *)filp,
				       (char *)buf, size);
}

unsigned int dg_fop_poll(struct file* filp, poll_table* wait)
{
	char *priv; /* statics table */

	/* not checking for NULL as it's done by open() */
	priv = dg_get_st_by_major(imajor(filp->f_dentry->d_inode));

	return entry_points.dldd_select(priv, (struct cdcm_file *)filp,
					0/*not vaild*/,
					(struct cdcm_sel *)wait);
}


long dg_fop_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	char *priv; /* statics table */

	/* not checking for NULL as it's done by open() */
	priv = dg_get_st_by_major(imajor(filp->f_dentry->d_inode));

	return entry_points.dldd_ioctl(priv, (struct cdcm_file *)filp,
				       cmd, (char *)arg);

}

/**
 * @brief
 *
 * @param filp -- file pointer
 * @param vma  --
 *
 * Mapping is based on the block index. So last mmap() parameter in user-space
 * (offset) should be offset in pagesize, multiplied on block index, that
 * should be mapped.
 *
 * @return
 */
int dg_fop_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int retval;
	/* statics table */
	/* not checking for NULL as it's done by open() */
	char *priv = dg_get_st_by_major(imajor(filp->f_dentry->d_inode));
	DevInfo_t *info = get_dit_from_statics(priv);
	AddrInfo_t *aip = &info->addr1;
	struct vme_mapping *vmep;
	int asi = vma->vm_pgoff; /* address space index */

	/* will map all the size user requested */
	unsigned long size = vma->vm_end - vma->vm_start;

	if (info->mlun < 0)
		return -ENOSYS;	/* not allowed for simulator */

	if (!(WITHIN_RANGE(0, asi, ASA-1))) {
		PRNT_ABS_ERR("mmap() offset must be within [0 - %d]\n", ASA-1);
		retval = -EINVAL;
		goto errout;
	}

	vmep = find_vme_mapping_from_addr(aip[asi].baseAddr);
	if (!vmep) {
		PRNT_ABS_ERR("Can't find mapping table for %d Address Space",
			     asi+1);
		retval = -EFAULT;
		goto errout;
	}

	vma->vm_flags |= VM_IO | VM_RESERVED | VM_DONTCOPY | VM_DONTEXPAND;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	return io_remap_pfn_range(vma,
				  vma->vm_start,
				  vmep->pci_addrl >> PAGE_SHIFT,
				  size,
				  vma->vm_page_prot);

 errout:
	return retval;
}

/**
 * @brief Open file
 *
 * @param inode -- device inode
 * @param filp  -- file pointer
 *
 * @return zero on success or a negative number on failure
 */
int dg_fop_open(struct inode *inode, struct file *filp)
{
	char *priv;  /* statics table */

	priv = dg_get_st_by_major(imajor(filp->f_dentry->d_inode));
	if (!priv) return -ENODEV;

	/* Not using filp->private_data to store priv,
	   so that Linux users can use it */

	return entry_points.dldd_open(priv, inode->i_rdev,
				      (struct cdcm_file *)filp);
}

/**
 * @brief Close file
 *
 * @param inode -- device inode
 * @param filp  -- file pointer
 *
 * <long-description>
 *
 * @return <ReturnValue>
 */
int dg_fop_release(struct inode *inode, struct file *filp)
{
	char *priv; /* statics table */

	/* not checking for NULL as it's done by open() */
	priv = dg_get_st_by_major(imajor(inode));

	return entry_points.dldd_close(priv, (struct cdcm_file *)filp);
}

