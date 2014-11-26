/*
 * kernel/gps.c
 *
 * Columbia University
 * COMS W4118 Fall 2014
 * Homework 6
 *
 * E. Atlidakis, G. Koloventzos, A. Papancea
 * UNI: ea2615, gk2409, alp2200
 * Last updated: 11/26/2014
 *
 */
#include <linux/gps.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>

/*
 * stores gps location
 * polled from daemon.
 */
static struct gps_location k_location={
	.latitude = 0,
	.longitude = 0,
	.accuracy = 0
};

static DEFINE_SPINLOCK(k_loc_lock);

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
	struct gps_location k_loc;

	if (current_uid() != 0)
		return -EACCES;
	if (loc == NULL)
		return -EINVAL;
	if (copy_from_user(&k_loc, loc, sizeof(k_loc)))
		return -EFAULT;

	spin_lock(&k_loc_lock);
	memcpy(&k_location, &k_loc, sizeof(k_loc));
	spin_unlock(&k_loc_lock);

	return 0;
}
