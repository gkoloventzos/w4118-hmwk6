/*
 * include/linux/gps.h
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
struct gps_location {
	double latitude;
	double longitude;
	float  accuracy;  /* in meters */
};
