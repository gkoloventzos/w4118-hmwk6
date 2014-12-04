#include "ext3.h"
#include <linux/gps.h>

int ext3_set_gps_location(struct inode *inode)
{
	int rval;
	struct gps_location local;

	get_location(&local);

	inode->latitude = local.latitude;
	inode->longitude = local.longitude;
	inode->accuracy = local.accuracy;

	return 0;
}

int ext3_get_gps_location(struct inode *inode, struct gps_location *location)
{
	struct gps_location local;

	spin_lock(&inode->i_lock);
	local.latitude = inode->latitude;
	local.longitude = inode->longitude;
	local.accuracy = inode->accuracy;
	spin_unlock(&inode->i_lock);

	memcpy(&location, &local, sizeof(local));

	return 0;
}
