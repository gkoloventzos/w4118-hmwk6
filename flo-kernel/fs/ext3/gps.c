#include "ext3.h"
#include <linux/gps.h>

/*
 * NOTE: Caller should hold the inode lock
 */
int ext3_set_gps_location(struct inode *inode)
{
	struct gps_location local;
	struct ext3_inode_info *ei;

	get_location(&local);

	ei = EXT3_I(inode);
	memcpy(&ei->i_latitude, &local.latitude, sizeof(unsigned long long));
	memcpy(&ei->i_longitude, &local.longitude, sizeof(unsigned long long));
	memcpy(&ei->i_accuracy, &local.accuracy, sizeof(unsigned int));

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
	memcpy(&local.latitude, &ei->i_latitude, sizeof(unsigned long long));
	memcpy(&local.longitude, &ei->i_longitude, sizeof(unsigned long long));
	memcpy(&local.accuracy, &ei->i_accuracy, sizeof(unsigned int));
	spin_unlock(&inode->i_lock);

	memcpy(location, &local, sizeof(local));

	return 0;
}
