/*
 * flo-kernel/kernel/gps.c
 *
 * Copyright (C) 2014 V. Atlidakis, G. Koloventzos, A. Papancea
 *
 * COMS W4118 Fall 2014, Columbia University
 */
#include <linux/fs.h>
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
	.accuracy = 0,
};

/*
 * Timestamp of the latest gps location update
 */
static long gps_location_ts = 0;

static DEFINE_RWLOCK(location_lock);

/*
 * Accessor exposed to the rest of the kernel
 */
long get_location(struct gps_location *loc)
{
	long local_ts;

	read_lock(&location_lock);
	memcpy(loc, &location, sizeof(location));
	local_ts = gps_location_ts;
	read_unlock(&location_lock);

	return local_ts;
}

/*
 * set_gps_location:  Updates the kernel with the device's current location.
 *
 * @u_location: Current device's location given from userspace.
 */
SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, u_location)
{
	int rval;
	int errno;
	struct gps_location k_location;

	if (current_uid() != 0) {
		errno = -EACCES;
		goto out;
	}

	if (u_location == NULL) {
		errno = -EINVAL;
		goto out;
	}

	rval = copy_from_user(&k_location, u_location, sizeof(k_location));
	if (rval < 0) {
		errno = -EFAULT;
		goto out;
	}

	write_lock(&location_lock);
	memcpy(&location, &k_location, sizeof(k_location));
	gps_location_ts = CURRENT_TIME_SEC.tv_sec;
	write_unlock(&location_lock);

	errno = 0;
out:
	return errno;
}

/*
 * get_gps_location: Retrive the given file's current location.
 *
 * @pathname: Path of the file whose GPS info we will try to retrieve.
 * @u_location: Userspace buffer to fill GPS info into.
 */
SYSCALL_DEFINE2(get_gps_location, const char __user *, pathname,
				  struct gps_location __user *, u_location)
{
	int rval;
	int errno;
	int lookup_flags;
	struct path path;
	int gps_coord_age;
	struct inode *inode;
	struct gps_location k_location;


	lookup_flags = 0;
	lookup_flags |= (AT_SYMLINK_FOLLOW | !LOOKUP_FOLLOW);
	rval = user_path_at(AT_FDCWD, pathname,  lookup_flags, &path);
	if (rval) {
		errno = rval;
		goto out;
	}
	inode = path.dentry->d_inode;

	/*
	 * TODO
	 * check permmisions and stuff...
	 * check return value of vfs_get...
	 */
	rval = vfs_get_gps_location(inode, &k_location);
	if (rval < 0) {
		errno = rval;
		goto path_put_out;
	}
	gps_coord_age = gps_location_ts == 0 ? -ENODEV : rval;

	rval = copy_to_user(u_location, &k_location, sizeof(k_location));
	if (rval < 0) {
		errno = -EFAULT;
		goto path_put_out;
	}

	errno = gps_coord_age;
path_put_out:
	path_put(&path);
out:
	return errno;
}
