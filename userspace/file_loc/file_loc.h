#ifndef _FILE_LOC_H_
#define _FILE_LOC_H_
/*
 * file_loc.h
 *
 * Columbia University
 * COMS W4118 Fall 2014
 * Homework 6
 *
 * E. Atlidakis, G. Koloventzos, A. Papancea
 * UNI: ea2615, gk2409, alp2200
 * Last updated: 12/03/2014
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
	double 	latitude;
	double 	longitude;
	float  	accuracy;
};

static inline int get_gps_location(const char *pathname,
				   struct gps_location *loc)
{
	return syscall(__NR_get_gps_location, pathname, loc);
}

#endif

