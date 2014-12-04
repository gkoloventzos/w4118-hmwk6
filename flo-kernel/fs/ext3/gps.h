#include <linux/fs.h>

int set_gps_location(struct inode *);
int get_gps_location(struct inode *, struct gps_location *);
