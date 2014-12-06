#ifndef _GPS_H
#define _GPS_H
/*
 * include/linux/gps.h
 *
 * Copyright (C) 2014 V. Atlidakis, G. Koloventzos, A. Papancea
 *
 * COMS W4118 Fall 2014, Columbia University
 *
 * Last updated: 11/26/2014
 */
struct gps_location {
	double	latitude;
	double	longitude;
	float	accuracy; /* in meters */
	int	coord_age; /* in seconds */
};

void get_location(struct gps_location *loc);
#endif
