#ifndef _GPSD_H_
#define _GPSD_H_
/*
 * gpsd.h
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
	float	accuracy;	/* accuracy in meters */
};

/* Use this file to access the most recent gps location
 * Provides in separate lines: latitude, longitude, accuracy */
#define GPS_LOCATION_FILE "/data/media/0/gps_location.txt"

static inline int set_gps_location(struct gps_location *loc)
{
	return syscall(__NR_set_gps_location, loc);
}

#endif
