/*
 * flo-kernel/fs/ext3/gps.c
 * Backend ext3 GPS-location operations.
 *
 * Copyright (C) 2014 V. Atlidakis, G. Koloventzos, A. Papancea
 *
 * COMS W4118 Fall 2014, Columbia University
 */
#include <linux/time.h>
#include <linux/gps.h>
#include "ext3.h"

/*
 * ext3_set_gps_location: Update inode's GPS inforation.
 *
 * @inode: The inode whose GPS info we update.
 *
 * NOTE: Caller must hold i_lock.
 */
int ext3_set_gps_location(struct inode *inode)
{
	long coord_age;
	long gps_location_ts;
	struct gps_location local;
	struct ext3_inode_info *ei = EXT3_I(inode);

	BUG_ON(!ei);

	/*
	 * Read device's current location using getter
	 * exposed from /kernel/gps.c layer.
	 */
	gps_location_ts = get_location(&local);

	coord_age = CURRENT_TIME_SEC.tv_sec - gps_location_ts;

	memcpy(&ei->i_latitude, &local.latitude, sizeof(unsigned long long));
	memcpy(&ei->i_longitude, &local.longitude, sizeof(unsigned long long));
	memcpy(&ei->i_accuracy, &local.accuracy, sizeof(unsigned int));
	memcpy(&ei->i_coord_age, &coord_age, sizeof(unsigned int));

	return 0;
}

/*
 * ext3_get_gps_location: Read inode's GPS information.
 *
 * @inode: The inode in whose GPS info we are interested.
 * @location: The sturct to return GPS info read.
 *
 * NOTE: The caller must hold i_lock.
 */
int ext3_get_gps_location(struct inode *inode, struct gps_location *location)
{
	struct gps_location local;
	struct ext3_inode_info *ei = EXT3_I(inode);

	BUG_ON(!ei);

	memcpy(&local.latitude, &ei->i_latitude, sizeof(unsigned long long));
	memcpy(&local.longitude, &ei->i_longitude, sizeof(unsigned long long));
	memcpy(&local.accuracy, &ei->i_accuracy, sizeof(unsigned int));
	memcpy(location, &local, sizeof(local));

	return *(int *) &ei->i_coord_age;
}
