/*
 * flo-kernel/kernel/gps.c
 *
 * Copyright (C) 2014 V. Atlidakis, G. Koloventzos, A. Papancea
 *
 * COMS W4118 Fall 2014, Columbia University
 *
 * Last updated: 11/26/2014
 */
#include <linux/gps.h>
#include <linux/slab.h>
#include <linux/namei.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>

/*
 * store gps location polled from daemon
 * and protect access to it with a spinlock.
 */
static struct gps_location location = {
	.latitude = 0,
	.longitude = 0,
	.accuracy = 0
};

static DEFINE_SPINLOCK(location_lock);

void get_location(struct gps_location *loc)
{
	spin_lock(&location_lock);
	memcpy(loc, &location, sizeof(location));
	spin_unlock(&location_lock);
}

/*
 * Updates the kernel with the device's current location
 */
SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, u_location)
{
	int rval;
	struct gps_location k_location;

	if (current_uid() != 0)
		return -EACCES;

	if (u_location == NULL)
		return -EINVAL;

	rval = copy_from_user(&k_location, u_location, sizeof(k_location));
	if (rval < 0)
		return -EFAULT;

	spin_lock(&location_lock);
	memcpy(&location, &k_location, sizeof(k_location));
	spin_unlock(&location_lock);

	return 0;
}

/*
 * Retrive the given file's current location
 */
SYSCALL_DEFINE2(get_gps_location, const char __user *, pathname,
				  struct gps_location __user *, u_location)
{
	int rval;
	int errno;
	struct path path;
	struct inode *inode;
	struct gps_location k_location;

	rval = user_path(pathname, &path);
	if (rval) {
		errno = rval;
		goto error;
	}
	inode = path.dentry->d_inode;

	/*
	 * TODO
	 * check permmisions and stuff...
	 * check return value of vfs_get...
	 */
	vfs_get_gps_location(inode, &k_location);

	rval = copy_to_user(u_location, &k_location, sizeof(k_location));
	if (rval < 0) {
		errno = -EFAULT;
		goto error;
	}

	return 0;
error:
	return errno;
}
