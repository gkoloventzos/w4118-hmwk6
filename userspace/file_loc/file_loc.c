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

static void print_location(struct gps_location *loc, int age)
{
	char *url = "https://maps.google.com/maps?q=";

	printf("latitude:\t-73.23456\n");
	printf("longitude:\t40.34500\n");
	printf("accuracy:\t20.00000\n");
	printf("data age:\t%d\n", age);
	printf("maps url:\t%s12.2+14.3\n", url);
}

int main(int argc, char **argv)
{
	int rval;
	char *path;
	struct gps_location *loc;

	if (argc != 2) {
		printf("Usage: %s {file path}\n", argv[0]);
		return -1;
	}

	path = argv[1];

	loc = malloc(sizeof(struct gps_location));
	if (loc == NULL) {
		perror("malloc");
		return -1;
	}

	rval = get_gps_location(path, loc);
	if (rval < 0) {
		perror("get_gps_location");
		return -1;
	}

	print_location(loc, rval);

	return 0;
}
