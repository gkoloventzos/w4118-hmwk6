/*
 * gpsd.c
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
#include "gpsd.h"

void daemonize(void)
{
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid > 0)
		exit(EXIT_SUCCESS);

	if (setsid() < 0) {
		perror("setsid");
		exit(EXIT_FAILURE);
	}

	close(0);
	close(1);
	close(2);
	chdir("/data/misc/");
	umask(0);
}

void poll_gps_data(void)
{
	FILE *file;
	int rval, i;
	struct gps_location *loc;

	loc = malloc(sizeof(struct gps_location));
	if (loc == NULL) {
		perror("malloc");
		goto exit;
	}

	file = fopen(GPS_LOCATION_FILE, "r");
	if (file != 0) {
		perror("fopen");
		goto free;
	}

	for (i = 0; i < 3; i++) {
		double d;
		float f;

		if (i < 2)
			rval = fscanf(file, "%lf", &d);
		else
			rval = fscanf(file, "%f", &f);

		if (rval < 1) {
			perror("fscanf");
			goto close;
		}

		if (i == 0)
			loc->latitude = d;
		else if (i == 1)
			loc->longitude = d;
		else
			loc->accuracy = f;
	}

	rval = set_gps_location(loc);
	if (rval != 0)
		perror("set_gps_location");

close:	fclose(file);
free:	free(loc);
exit:	return;
}

int main(int argc, char *argv[])
{
	/* turn me into daemon */
	daemonize();

	printf("daemon: start polling for gps data\n");

	while (1) {
		poll_gps_data();
		usleep(200000);
	}

	return 0;
}
