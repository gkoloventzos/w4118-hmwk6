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
 * Retrive the device's current location
 */
SYSCALL_DEFINE2(get_gps_location, const char __user *, pathname,
				  struct gps_location __user *, loc)
{
	int errno;

	if (copy_to_user(loc, &location, sizeof(location)) < 0) {
		errno = -EFAULT;
		goto error;
	}

	return 0;

error:
	return errno;
}
