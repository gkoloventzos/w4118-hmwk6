/*
 * file_loc.c
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
#include "file_loc.h"

static void print_location(struct gps_location loc, int coord_age)
{
	/* Google Maps base URL */
	char *url = "https://maps.google.com/maps?q=";

	printf("latitude:\t%f\n", loc.latitude);
	printf("longitude:\t%f\n", loc.longitude);
	printf("accuracy:\t%f\n", loc.accuracy);
	printf("data age:\t%d\n", coord_age);
	printf("maps url:\t%s%f+%f\n", url, loc.latitude, loc.longitude);
}

int main(int argc, char **argv)
{
	int rval;
	char *path;
	struct gps_location loc;

	if (argc != 2) {
		printf("Usage: %s {file path}\n", argv[0]);
		return -1;
	}

	path = argv[1];

	rval = get_gps_location(path, &loc);
	if (rval < 0) {
		perror("get_gps_location");
		return -1;
	}

	print_location(loc, rval);

	return 0;
}
