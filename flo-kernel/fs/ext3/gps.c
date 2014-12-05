#include "ext3.h"
#include <linux/gps.h>

/*
 *
 * NOTE: Caller should hold the inode lock
 */
int ext3_set_gps_location(struct inode *inode)
{
	struct gps_location local;
	struct ext3_inode_info *ei;

	get_location(&local);

	printk(KERN_ERR "\n\nFUCKING SERIOUS??????\n\n\n");

	ei = EXT3_I(inode);
	ei->i_latitude = *((unsigned long long *) &local.latitude);
	ei->i_longitude = *((unsigned long long *) &local.longitude);
	ei->i_accuracy = *((unsigned long *) &local.accuracy);

	printk(KERN_ERR "storing lat=%ld, lng=%ld, acc=%ld\n",
			*((long int *)&ei->i_latitude),
			*((long int *)&ei->i_longitude),
			*((long int *)&ei->i_accuracy));

	return 0;
}

/*
 *
 *NOTE: Make sure caller does not hold any  lock?
 */
int ext3_get_gps_location(struct inode *inode, struct gps_location *location)
{
	struct gps_location local;
	struct ext3_inode_info *ei;

	spin_lock(&inode->i_lock);
	ei = EXT3_I(inode);
	local.latitude = *((double *)&ei->i_latitude);
	local.longitude = *((double *)&ei->i_longitude);
	local.accuracy = *((float *)&ei->i_accuracy);
	spin_unlock(&inode->i_lock);

	memcpy(&location, &local, sizeof(local));

	return 0;
}
