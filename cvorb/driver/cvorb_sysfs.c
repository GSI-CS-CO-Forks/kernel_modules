/*
 * cvorb_sysfs.c
 * sysfs management for cvorb
 *
 * Copyright (c) 2012 Samuel Iglesias Gonsalvez <siglesia@cern.ch>
 *		      Michel Arruat <michel.arruat@cern.ch>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/ctype.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/slab.h>
#include "cvorb.h"
#include "cvorb_priv.h"

#ifdef CONFIG_SYSFS

/*
 * Note that the read-only parameters' handlers do not take the card's lock.
 * This is because the information they export is not supposed to change
 * once the device has been loaded.
 */

static int cvorb_get_int(const char *buf, size_t count, int32_t * val)
{
	unsigned long value;
	char *endp;

	if (!isdigit(buf[0]))
		return -EINVAL;
	value = simple_strtoul(buf, &endp, 0);
	/* add one to endp to allow a trailing '\n' */
	if (endp + 1 < buf + count)
		return -EINVAL;
	*val = value;
	return 0;
}

/* ====================================================================================
 * Channel attributes
 * ====================================================================================
 */
#define to_cvorb_channel(k) container_of(k, struct cvorb_channel, channels_dir)
#define to_cvorb_channel_attr(a) container_of(a, struct cvorb_channel_attribute, attr)

#define CVORB_CHANNEL_ATTR(_name,_mode,_show,_store)          \
struct cvorb_channel_attribute channel_attr_##_name = {               \
	.attr = {.name = __stringify(_name), .mode = _mode }, \
	.show   = _show,                                      \
	.store  = _store,                                     \
};

/* cvorb channels specific attribute structure */
struct cvorb_channel_attribute {
	struct attribute attr;
	 ssize_t(*show) (struct cvorb_channel *, char *);
	 ssize_t(*store) (struct cvorb_channel *, const char *, size_t);
};

static ssize_t cvorb_show_enable_fcn_mask(struct cvorb_channel *ch,
					  char *buf)
{
	return snprintf(buf, PAGE_SIZE, "0x%llx\n", ch->enable_fcn_mask);
}

CVORB_CHANNEL_ATTR(enable_fcn_mask, S_IRUGO, cvorb_show_enable_fcn_mask,
		   NULL);

static ssize_t cvorb_store_enable_fcn(struct cvorb_channel *ch,
				      const char *buf, size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_enable_fcn(ch, value);
	return count;
}

CVORB_CHANNEL_ATTR(enable_fcn, S_IWUSR, NULL, cvorb_store_enable_fcn);

static ssize_t cvorb_store_disable_fcn(struct cvorb_channel *ch,
				       const char *buf, size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_disable_fcn(ch, value);
	return count;
}

CVORB_CHANNEL_ATTR(disable_fcn, S_IWUSR, NULL, cvorb_store_disable_fcn);

static ssize_t cvorb_store_repeat_count(struct cvorb_channel *ch,
					const char *buf, size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_ch_repeat_count(ch, value);
	return count;
}

static ssize_t cvorb_show_repeat_count(struct cvorb_channel *ch, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", ch->repeat_count);
}

CVORB_CHANNEL_ATTR(repeat_count, S_IWUSR | S_IRUGO,
		   cvorb_show_repeat_count, cvorb_store_repeat_count);

static ssize_t cvorb_store_select_fcn(struct cvorb_channel *ch,
				      const char *buf, size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_select_fcn(ch, value);
	return count;
}

static ssize_t cvorb_show_select_fcn(struct cvorb_channel *ch, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", ch->selected_fcn);
}

CVORB_CHANNEL_ATTR(select_fcn, S_IWUSR | S_IRUGO, cvorb_show_select_fcn,
		   cvorb_store_select_fcn);

static ssize_t cvorb_show_status(struct cvorb_channel *ch, char *buf)
{
	uint32_t status;

	cvorb_ch_status(ch, &status);
	return snprintf(buf, PAGE_SIZE, "0x%08x\n", status);
}

CVORB_CHANNEL_ATTR(status, S_IRUGO, cvorb_show_status, NULL);

static ssize_t cvorb_store_enable(struct cvorb_channel *ch,
				  const char *buf, size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_ch_enable(ch, value);
	return count;
}

static ssize_t cvorb_show_enable(struct cvorb_channel *ch, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", ch->enable);
}

CVORB_CHANNEL_ATTR(enable, S_IWUSR | S_IRUGO, cvorb_show_enable,
		   cvorb_store_enable);

/* Set of show/store higher level functions for default cvorb attributes */
static ssize_t cvorb_channel_show(struct kobject *channels_dir,
				  struct attribute *attr, char *buffer)
{
	struct cvorb_channel *channel = to_cvorb_channel(channels_dir);
	struct cvorb_channel_attribute *cvorb_channel_attr =
	    to_cvorb_channel_attr(attr);

	if (cvorb_channel_attr->show)
		return cvorb_channel_attr->show(channel, buffer);
	return -EIO;
}

static ssize_t cvorb_channel_store(struct kobject *channels_dir,
				   struct attribute *attr,
				   const char *buffer, size_t count)
{
	struct cvorb_channel *channel = to_cvorb_channel(channels_dir);
	struct cvorb_channel_attribute *cvorb_channel_attr =
	    to_cvorb_channel_attr(attr);

	if (cvorb_channel_attr->store)
		return cvorb_channel_attr->store(channel, buffer, count);
	return -EIO;
}

static struct sysfs_ops cvorb_channel_fs_ops = {
	.show = cvorb_channel_show,
	.store = cvorb_channel_store
};

/* default attributes of the CSROW<id> object */
static struct cvorb_channel_attribute *default_cvorb_channel_attr[] = {
	&channel_attr_status,
	&channel_attr_enable,
	&channel_attr_repeat_count,
	&channel_attr_select_fcn,
	&channel_attr_enable_fcn,
	&channel_attr_disable_fcn,
	&channel_attr_enable_fcn_mask,
	NULL,
};

/* fcn attribute name is formated as "fcn_xx" with xx in the range [0,64] */
#define CCVORB_FCN_ATTR_NAME_SIZE 7
static struct bin_attribute **cvorb_fcn_bin_attrs = NULL;
static int is_sysfs_fcn_enabled = 0;

static void cvorb_channel_instance_release(struct kobject *kobj)
{
	unsigned int fcnIdx;

	if (is_sysfs_fcn_enabled) {
		for (fcnIdx = 0; fcnIdx < CVORB_MAX_FCT_NR; ++fcnIdx) {
			sysfs_remove_bin_file(kobj,
					      cvorb_fcn_bin_attrs[fcnIdx]);
		}
	}
}

/* the kobj_type instance for a CSROW */
static struct kobj_type ktype_cvorb_channel = {
	.release = cvorb_channel_instance_release,
	.sysfs_ops = &cvorb_channel_fs_ops,
	.default_attrs = (struct attribute **) default_cvorb_channel_attr,
};

/* Channel binary attribute fcn*/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
static ssize_t cvorb_store_fcn(struct file *file,
			       struct kobject *channels_dir,
			       struct bin_attribute *bin_attr,
			       char *buffer, loff_t off, size_t count)
#else
static ssize_t cvorb_store_fcn(struct kobject *channels_dir,
			       struct bin_attribute *bin_attr,
			       char *buffer, loff_t off, size_t count)
#endif
{
	static const int FCN_ATTR_HEADER_SIZE = 4;
	uint32_t fcn_nr;
	int ret;
	struct cvorb_channel *channel = to_cvorb_channel(channels_dir);

	/* Extract the value as an int from the sysfs file */
	/* attribute name is formatted this way fcn_x with xx = function number */
	ret = cvorb_get_int(bin_attr->attr.name + FCN_ATTR_HEADER_SIZE,	/* points to the number */
			    strlen(bin_attr->attr.name) - FCN_ATTR_HEADER_SIZE,	/* number of chars */
			    &fcn_nr);
	if (ret)
		return ret;
	ret = cvorb_sysfs_set_fcn(channel, fcn_nr, (void *) buffer, count);
	return (ret) ? ret : count;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
static ssize_t cvorb_read_fcn(struct file *file,
			      struct kobject *channels_dir,
			      struct bin_attribute *bin_attr, char *buffer,
			      loff_t off, size_t count)
#else
static ssize_t cvorb_read_fcn(struct kobject *channels_dir,
			      struct bin_attribute *bin_attr,
			      char *buffer, loff_t off, size_t count)
#endif
{
	static const int FCN_ATTR_HEADER_SIZE = 4;
	uint32_t fcn_nr;
	int ret;
	ssize_t ret_count;
	struct cvorb_channel *channel = to_cvorb_channel(channels_dir);

	/* Extract the value as an int from the sysfs file */
	/* attribute name is formatted this way fcn_x with xx = function number */
	ret = cvorb_get_int(bin_attr->attr.name + FCN_ATTR_HEADER_SIZE,	/* points to the number */
			    strlen(bin_attr->attr.name) - FCN_ATTR_HEADER_SIZE,	/* number of chars */
			    &fcn_nr);
	if (ret)
		return ret;
	ret =
	    cvorb_sysfs_get_fcn(channel, fcn_nr, (void *) buffer,
				&ret_count);
	return (ret) ? ret : ret_count;
}

/*end of Channel attributes */

/* ====================================================================================
 * SubModule attributes
 * ====================================================================================
 */
#define to_cvorb_submodule(k) container_of(k, struct cvorb_submodule, submodules_dir)
#define to_cvorb_submodule_attr(a) container_of(a, struct cvorb_submodule_attribute, attr)

#define CVORB_SUBMODULE_ATTR(_name,_mode,_show,_store)          \
struct cvorb_submodule_attribute submodule_attr_##_name = {               \
	.attr = {.name = __stringify(_name), .mode = _mode },   \
	.show   = _show,                                        \
	.store  = _store,                                       \
};

/* cvorb specific attribute structure */
struct cvorb_submodule_attribute {
	struct attribute attr;
	 ssize_t(*show) (struct cvorb_submodule *, char *);
	 ssize_t(*store) (struct cvorb_submodule *, const char *, size_t);
};

static ssize_t cvorb_store_submodule_reset(struct cvorb_submodule
					   *submodule, const char *buf,
					   size_t count)
{
	cvorb_submodule_reset(submodule);
	return count;
}

CVORB_SUBMODULE_ATTR(reset, S_IWUSR, NULL, cvorb_store_submodule_reset);

static ssize_t cvorb_store_submodule_start(struct cvorb_submodule
					   *submodule, const char *buf,
					   size_t count)
{
	int ret = cvorb_submodule_trigger(submodule, CVORB_START);
	return (ret) ? ret : count;
}

CVORB_SUBMODULE_ATTR(start, S_IWUSR, NULL, cvorb_store_submodule_start);

static ssize_t cvorb_store_submodule_stop(struct cvorb_submodule
					  *submodule, const char *buf,
					  size_t count)
{
	int ret = cvorb_submodule_trigger(submodule, CVORB_STOP);
	return (ret) ? ret : count;
}

CVORB_SUBMODULE_ATTR(stop, S_IWUSR, NULL, cvorb_store_submodule_stop);

static ssize_t cvorb_store_submodule_event_start(struct cvorb_submodule
						 *submodule,
						 const char *buf,
						 size_t count)
{
	int ret = cvorb_submodule_trigger(submodule, CVORB_EVT_START);
	return (ret) ? ret : count;
}

CVORB_SUBMODULE_ATTR(event_start, S_IWUSR, NULL,
		     cvorb_store_submodule_event_start);

static ssize_t cvorb_store_submodule_event_stop(struct cvorb_submodule
						*submodule,
						const char *buf,
						size_t count)
{
	int ret = cvorb_submodule_trigger(submodule, CVORB_EVT_STOP);
	return (ret) ? ret : count;
}

CVORB_SUBMODULE_ATTR(event_stop, S_IWUSR, NULL,
		     cvorb_store_submodule_event_stop);

static ssize_t cvorb_show_input_polarity(struct cvorb_submodule *submodule,
					 char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", submodule->inpol);
}

static ssize_t cvorb_store_input_polarity(struct cvorb_submodule
					  *submodule, const char *buf,
					  size_t count)
{
	uint32_t value;
	int ret;

	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	ret = cvorb_submodule_inpol(submodule, value);
	return (ret) ? ret : count;
}

CVORB_SUBMODULE_ATTR(input_polarity, S_IWUSR | S_IRUGO,
		     cvorb_show_input_polarity,
		     cvorb_store_input_polarity);

static ssize_t cvorb_show_submodule_status(struct cvorb_submodule
					   *submodule, char *buf)
{
	uint32_t status;

	/* read status register */
	cvorb_submodule_status(submodule, &status);
	return snprintf(buf, PAGE_SIZE, "0x%08x\n", status);
}

CVORB_SUBMODULE_ATTR(status, S_IRUGO, cvorb_show_submodule_status, NULL);

static ssize_t cvorb_show_submodule_dac_source(struct cvorb_submodule
					       *submodule, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", submodule->dac_source);
}

static ssize_t cvorb_store_submodule_dac_source(struct cvorb_submodule
						*submodule,
						const char *buf,
						size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_submodule_dacsource(submodule, value);
	return count;
}

CVORB_SUBMODULE_ATTR(dac_source, S_IWUSR | S_IRUGO,
		     cvorb_show_submodule_dac_source,
		     cvorb_store_submodule_dac_source);

static ssize_t cvorb_show_submodule_optical_source(struct cvorb_submodule
						   *submodule, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", submodule->optical_source);
}

static ssize_t cvorb_store_submodule_optical_source(struct cvorb_submodule
						    *submodule,
						    const char *buf,
						    size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_submodule_opticalsource(submodule, value);
	return count;
}

CVORB_SUBMODULE_ATTR(optical_source, S_IWUSR | S_IRUGO,
		     cvorb_show_submodule_optical_source,
		     cvorb_store_submodule_optical_source);

static ssize_t cvorb_show_submodule_led_source(struct cvorb_submodule
					       *submodule, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", submodule->led_source);
}

static ssize_t cvorb_store_submodule_led_source(struct cvorb_submodule
						*submodule,
						const char *buf,
						size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_submodule_ledsource(submodule, value);
	return count;
}

CVORB_SUBMODULE_ATTR(led_source, S_IWUSR | S_IRUGO,
		     cvorb_show_submodule_led_source,
		     cvorb_store_submodule_led_source);

static ssize_t cvorb_show_submodule_enable_optical_output(struct
							  cvorb_submodule
							  *submodule,
							  char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n",
			submodule->optical_out_enabled);
}

static ssize_t cvorb_store_submodule_enable_optical_output(struct
							   cvorb_submodule
							   *submodule,
							   const char *buf,
							   size_t count)
{
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	cvorb_submodule_enable_opticalout(submodule, value);
	return count;
}

CVORB_SUBMODULE_ATTR(enable_optical_output, S_IWUSR | S_IRUGO,
		     cvorb_show_submodule_enable_optical_output,
		     cvorb_store_submodule_enable_optical_output);

/* Set of show/store higher level functions for default cvorb attributes */
static ssize_t cvorb_submodule_show(struct kobject *submodules_dir,
				    struct attribute *attr, char *buffer)
{
	struct cvorb_submodule *submodule =
	    to_cvorb_submodule(submodules_dir);
	struct cvorb_submodule_attribute *cvorb_submodule_attr =
	    to_cvorb_submodule_attr(attr);

	if (cvorb_submodule_attr->show)
		return cvorb_submodule_attr->show(submodule, buffer);
	return -EIO;
}

static ssize_t cvorb_submodule_store(struct kobject *submodules_dir,
				     struct attribute *attr,
				     const char *buffer, size_t count)
{
	struct cvorb_submodule *submodule =
	    to_cvorb_submodule(submodules_dir);
	struct cvorb_submodule_attribute *cvorb_submodule_attr =
	    to_cvorb_submodule_attr(attr);

	if (cvorb_submodule_attr->store)
		return cvorb_submodule_attr->store(submodule, buffer,
						   count);
	return -EIO;
}

static struct sysfs_ops cvorb_submodule_fs_ops = {
	.show = cvorb_submodule_show,
	.store = cvorb_submodule_store
};

/* default attributes of the CSROW<id> object */
static struct cvorb_submodule_attribute *default_cvorb_submodule_attr[] = {
	&submodule_attr_reset,
	&submodule_attr_start,
	&submodule_attr_stop,
	&submodule_attr_event_start,
	&submodule_attr_event_stop,
	&submodule_attr_input_polarity,
	&submodule_attr_status,
	&submodule_attr_dac_source,
	&submodule_attr_optical_source,
	&submodule_attr_led_source,
	&submodule_attr_enable_optical_output,
	NULL,
};

static void cvorb_submodule_instance_release(struct kobject *kobj)
{
}

/* the kobj_type instance for a CSROW */
static struct kobj_type ktype_cvorb_submodule = {
	.release = cvorb_submodule_instance_release,
	.sysfs_ops = &cvorb_submodule_fs_ops,
	.default_attrs =
	    (struct attribute **) default_cvorb_submodule_attr,
};

/* End of submodule attributes */

/* ====================================================================================
 * Board attributes
 * ====================================================================================
 */
static ssize_t cvorb_show_hw_version(struct device *pdev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct cvorb_dev *card = dev_get_drvdata(pdev);

	return snprintf(buf, PAGE_SIZE, "%s\n", card->hw_rev);
}

static DEVICE_ATTR(hw_version, S_IRUGO, cvorb_show_hw_version, NULL);

static ssize_t cvorb_show_temperature(struct device *pdev,
				      struct device_attribute *attr,
				      char *buf)
{
	struct cvorb_dev *card = dev_get_drvdata(pdev);
	uint32_t temperature;

	cvorb_get_temperature(card, &temperature);
	return snprintf(buf, PAGE_SIZE, "%d\n", temperature);
}

static DEVICE_ATTR(temperature, S_IRUGO, cvorb_show_temperature, NULL);

static ssize_t cvorb_show_pcb_id(struct device *pdev,
				 struct device_attribute *attr, char *buf)
{
	struct cvorb_dev *card = dev_get_drvdata(pdev);
	uint64_t pcb_id;

	cvorb_get_pcb_id(card, &pcb_id);
	return snprintf(buf, PAGE_SIZE, "0x%llx\n", pcb_id);
}

static DEVICE_ATTR(pcb_id, S_IRUGO, cvorb_show_pcb_id, NULL);

static ssize_t cvorb_show_description(struct device *pdev,
				      struct device_attribute *attr,
				      char *buf)
{
	struct cvorb_dev *card = dev_get_drvdata(pdev);

	return snprintf(buf, PAGE_SIZE, "CVORB at VME-A32 0x%08x\n",
			card->vme_base);
}

static DEVICE_ATTR(description, S_IRUGO, cvorb_show_description, NULL);

static ssize_t cvorb_store_reset(struct device *pdev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	struct cvorb_dev *card = dev_get_drvdata(pdev);
	cvorb_reset(card);
	return count;
}

static DEVICE_ATTR(reset, S_IWUSR, NULL, cvorb_store_reset);

static ssize_t cvorb_show_sysfs_fcn_enabled(struct device *pdev,
					    struct device_attribute *attr,
					    char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", is_sysfs_fcn_enabled);
}

static DEVICE_ATTR(sysfs_fcn_enabled, S_IRUGO,
		   cvorb_show_sysfs_fcn_enabled, NULL);

static ssize_t cvorb_show_debug_level(struct device *pdev,
					    struct device_attribute *attr,
					    char *buf)
{
	struct cvorb_dev *card = dev_get_drvdata(pdev);
	return snprintf(buf, PAGE_SIZE, "%d\n", card->dbglevel);
}

static ssize_t cvorb_store_debug_level(struct device *pdev,
				 	struct device_attribute *attr,
				 	const char *buf, size_t count)
{
	struct cvorb_dev *card = dev_get_drvdata(pdev);
	uint32_t value;
	int ret;

	/* Extract the value as an int from the sysfs file */
	ret = cvorb_get_int(buf, count, &value);
	if (ret)
		return ret;

	card->dbglevel = value; 
	return count;
}

static DEVICE_ATTR(debug_level, S_IRUGO | S_IWUSR,
		   cvorb_show_debug_level, cvorb_store_debug_level);

static struct attribute *cvorb_attrs[] = {
	&dev_attr_hw_version.attr,
	&dev_attr_temperature.attr,
	&dev_attr_pcb_id.attr,
	&dev_attr_description.attr,
	&dev_attr_reset.attr,
	&dev_attr_sysfs_fcn_enabled.attr,
	&dev_attr_debug_level.attr,
	NULL,
};

static struct attribute_group cvorb_attr_group = {
	.attrs = cvorb_attrs,
};

/* End of Board attributes */

/* ===========================================================================================
 * Build the directories and files structure under sysfs
 * module/cvorb/cvorb.0/
 *                     reset
 *                     hw_version
 *                     pcb_id
 *                     temperature
 *                     submodule.0/ <== 2 directories for submodule's files: submodule.0 and submodule.1
 *                                reset
 *                                status
 *                                trigger
 *                                ...
 *                                channel.0/ <== 8 directories for channel's files: channel.0 up to channel.7
 *                                         status
 *                                         load_fcn
 *                                         select_fcn
 *                                         ...
 *                                channel.1/
 *                                         status
 *                                         ...
 *                     submodule.1/
 *                                reset
 *                                ...
 * ==========================================================================================
 */
static int cvorb_dev_add_attributes(struct cvorb_dev *card)
{
	int ret;
	volatile int submoduleIdx = 0, channelIdx = 0, fcnIdx = 0;
	struct cvorb_submodule *submodule = NULL;
	struct cvorb_channel *channel = NULL;

	for (submoduleIdx = 0; submoduleIdx < CVORB_SUBMODULES;
	     ++submoduleIdx) {
		submodule = &card->submodules[submoduleIdx];
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
		ret =
		    kobject_init_and_add(&submodule->submodules_dir,
					 &ktype_cvorb_submodule,
					 &card->dev->kobj, "submodule.%d",
					 submoduleIdx);
#else
		kobject_init(&submodule->submodules_dir);
		kobject_set_name(&submodule->submodules_dir,
				 "submodule.%d", submoduleIdx);
		submodule->submodules_dir.parent = &card->dev->kobj;
		submodule->submodules_dir.ktype = &ktype_cvorb_submodule;
		ret = kobject_register(&submodule->submodules_dir);
#endif
		if (ret) {
			--submoduleIdx;	/* set properly the index for the removal */
			goto unregister_kobjects;
		}
		for (channelIdx = 0; channelIdx < CVORB_CHANNELS;
		     ++channelIdx) {
			channel = &submodule->channels[channelIdx];
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
			ret =
			    kobject_init_and_add(&channel->channels_dir,
						 &ktype_cvorb_channel,
						 &submodule->
						 submodules_dir,
						 "channel.%d", channelIdx);
#else
			kobject_init(&channel->channels_dir);
			kobject_set_name(&channel->channels_dir,
					 "channel.%d", channelIdx);
			channel->channels_dir.parent =
			    &submodule->submodules_dir;
			channel->channels_dir.ktype = &ktype_cvorb_channel;
			ret = kobject_register(&channel->channels_dir);
#endif
			if (ret) {
				--channelIdx;	/* set properly the index for the removal */
				goto unregister_kobjects;
			}
			if (is_sysfs_fcn_enabled) {
				for (fcnIdx = 0; fcnIdx < CVORB_MAX_FCT_NR;
				     ++fcnIdx) {
					ret =
					    sysfs_create_bin_file
					    (&channel->channels_dir,
					     cvorb_fcn_bin_attrs[fcnIdx]);
					if (ret) {
						goto unregister_kobjects;
						--fcnIdx;	/* set properly the index for the removal */
					}
				}
			}
		}
	}
	return 0;

unregister_kobjects:
	/* Rollback: unregister any kobject previously registered successfully */
	/* Done by stepping_down the various indexes */
	while (submoduleIdx >= 0) {
		while (channelIdx >= 0) {
			channel = &submodule->channels[channelIdx];
			if (is_sysfs_fcn_enabled) {
				while (fcnIdx >= 0) {
					sysfs_remove_bin_file(&channel->
							      channels_dir,
							      cvorb_fcn_bin_attrs
							      [fcnIdx]);
					--fcnIdx;
				}
				fcnIdx = CVORB_MAX_FCT_NR - 1;
			}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
			kobject_del(&channel->channels_dir);
#else
			kobject_unregister(&channel->channels_dir);
#endif
			--channelIdx;
		}
		channelIdx = CVORB_CHANNELS - 1;
		submodule = &card->submodules[submoduleIdx];
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
		kobject_del(&submodule->submodules_dir);
#else
		kobject_unregister(&submodule->submodules_dir);
#endif
		--submoduleIdx;
	}
	return ret;
}

int cvorb_create_sysfs_files(struct cvorb_dev *card)
{
	int error = 0;

	error = sysfs_create_group(&card->dev->kobj, &cvorb_attr_group);
	if (error)
		return error;
	error = cvorb_dev_add_attributes(card);
	if (error)
		goto add_attributes_failed;
	return 0;

add_attributes_failed:
	sysfs_remove_group(&card->dev->kobj, &cvorb_attr_group);
	return error;
}

static void cvorb_dev_del_attributes(struct cvorb_dev *card)
{
	struct cvorb_submodule *sm;
	struct cvorb_channel *ch;
	unsigned int smIdx, chIdx = 0;

	for (smIdx = 0; smIdx < CVORB_SUBMODULES; ++smIdx) {
		sm = &card->submodules[smIdx];
		for (chIdx = 0; chIdx < CVORB_CHANNELS; ++chIdx) {
			ch = &sm->channels[chIdx];
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
			kobject_del(&ch->channels_dir);
#else
			kobject_unregister(&ch->channels_dir);
#endif
		}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
		kobject_del(&sm->submodules_dir);
#else
		kobject_unregister(&sm->submodules_dir);
#endif
	}
}

void cvorb_remove_sysfs_files(struct cvorb_dev *card)
{
	printk(KERN_INFO PFX
	       "cvorb_remove_sysfs_files board with lun %d.\n", card->lun);
	cvorb_dev_del_attributes(card);
	sysfs_remove_group(&card->dev->kobj, &cvorb_attr_group);
}

/* 
 * Implementing get/set cvorb function via sysfs requires that the
 * PAGE_SIZE (size of the buffer used with sysfs) is at least equal
 * to the maximum function size defined by the hardware which is 4kb 
 */
int cvorb_sysfs_init_module(void)
{
	int fcnIdx;
	char *attr_name;
	struct bin_attribute *fcn_bin_attr;

	/* check first if teh size of teh PAGE_SIZE is big enough */
	/* compare to teh cvorb function size in bytes */
	if (PAGE_SIZE < CVORB_FCN_MAX_BYTE_SIZE) {
		printk(KERN_INFO PFX
		       "PAGE_SIZE is not big enough to support get/set function via sysfs.\n"
		       "Please use ioctl calls (PAGE_SIZE: %ld CVORB_FCN_MAX_SIZE:4096).\n",
		       PAGE_SIZE);
		is_sysfs_fcn_enabled = 0;
		return 0;
	}

	cvorb_fcn_bin_attrs =
	    kzalloc(sizeof(struct bin_attribute *) * CVORB_MAX_FCT_NR,
		    GFP_KERNEL);
	if (cvorb_fcn_bin_attrs == NULL)
		return -ENOMEM;	/* Out of memory */
	for (fcnIdx = 0; fcnIdx < CVORB_MAX_FCT_NR; ++fcnIdx) {
		fcn_bin_attr =
		    kzalloc(sizeof(struct bin_attribute), GFP_KERNEL);
		if (fcn_bin_attr == NULL)
			goto error1;
		attr_name = kzalloc(CCVORB_FCN_ATTR_NAME_SIZE, GFP_KERNEL);
		if (attr_name == NULL)
			goto error2;
		snprintf(attr_name, CCVORB_FCN_ATTR_NAME_SIZE, "fcn_%d",
			 fcnIdx);
		fcn_bin_attr->attr.name = attr_name;
		fcn_bin_attr->attr.mode = S_IWUSR | S_IRUGO;
		/* fcn size is dynamic tehrefore no point to set it */
		fcn_bin_attr->size = 0;
		fcn_bin_attr->read = cvorb_read_fcn;
		fcn_bin_attr->write = cvorb_store_fcn;
		cvorb_fcn_bin_attrs[fcnIdx] = fcn_bin_attr;
	}
	is_sysfs_fcn_enabled = 1;	/* get/set fcn via sysfs is enabled */
	return 0;

error2:
	kfree(fcn_bin_attr);	/* This one is not registered in the array */
error1:
	while (--fcnIdx) {	/* Rollback: step_down the idx */
		kfree(cvorb_fcn_bin_attrs[fcnIdx]->attr.name);
		kfree(cvorb_fcn_bin_attrs[fcnIdx]);
	}
	kfree(cvorb_fcn_bin_attrs);
	return -ENOMEM;		/* Out of memory */
}

void cvorb_sysfs_exit_module(void)
{
	int fcnIdx;
	if (is_sysfs_fcn_enabled) {
		/* All the cvorb devices have been uninstalled */
		/* It's time to clean dynamic memory allocated to build dynamic attributes(fcn) */
		/* before leaving the module itself */
		for (fcnIdx = 0; fcnIdx < CVORB_MAX_FCT_NR; ++fcnIdx) {
			kfree(cvorb_fcn_bin_attrs[fcnIdx]->attr.name);
			kfree(cvorb_fcn_bin_attrs[fcnIdx]);
		}
		kfree(cvorb_fcn_bin_attrs);
	}
}

#else

int cvorb_create_sysfs_files(struct cvorb_dev *card)
{
	return 0;
}

void cvorb_remove_sysfs_files(struct cvorb_dev *card)
{
}

#endif				/* CONFIG_SYSFS */
