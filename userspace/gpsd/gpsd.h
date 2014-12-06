#ifndef _GPSD_H_
#define _GPSD_H_
/*
 * userspace/gpsd/gpsd.h
 * User space daemon polling GPS location data header file
 *
 * Copyright (C) 2014 V. Atlidakis, G. Koloventzos, A. Papancea
 *
 * COMS W4118 Fall 2014, Columbia University
 *
 * Last updated: 12/05/2014
 */
#include <errno.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/syscall.h>

struct gps_location {
	double	latitude;
	double	longitude;
	float	accuracy;  /* accuracy in meters */
};

/* Use this file to access the most recent gps location
 * Provides in separate lines: latitude, longitude, accuracy */
#define GPS_LOCATION_FILE "/data/media/0/gps_location.txt"

static inline int set_gps_location(struct gps_location *loc)
{
	return syscall(__NR_set_gps_location, loc);
}

#endif
