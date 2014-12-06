/*
 * userspace/gpsd/gpsd.c
 * User space daemon polling GPS location data
 *
 * Copyright (C) 2014 V. Atlidakis, G. Koloventzos, A. Papancea
 *
 * COMS W4118 Fall 2014, Columbia University
 *
 * Last updated: 11/26/2014
 */
#include "gpsd.h"
#include <time.h>


#ifdef _DEBUG
#define LOGFILE "/data/misc/gpstrace.log"
#define DBG(fmt, ...) fprintf(fp, fmt, ## __VA_ARGS__)
static FILE *fp;
#endif

/*
 * Turn calling process into a daemon
 */
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
	struct gps_location location;

	file = fopen(GPS_LOCATION_FILE, "r");
	if (file == NULL) {
		perror("fopen");
		goto exit;
	}

	if (fscanf(file, "%lf %lf %f", &location.latitude,
				       &location.longitude,
				       &location.accuracy) != 3) {
		perror("fscanf");
		goto close;
	}

	&location.coord_age = CURRENT_TIME_SEC.tv_sec;

#ifdef _DEBUG
	DBG("%u - lat: %f, lng: %f, accuracy: %f\n", (unsigned)time(NULL),
						     location.latitude,
						     location.longitude,
						     location.accuracy);
	if (set_gps_location(&location) < 0)
		DBG("failed to write gps data to kernel\n");
#else
	set_gps_location(&location);
#endif

close:	fclose(file);
exit:	return;
}

int main(int argc, char *argv[])
{
	/* turn me into daemon */
	daemonize();
#ifdef _DEBUG
	fp = fopen(LOGFILE, "w+");
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
#endif
	while (1) {
		poll_gps_data();
		usleep(200000);
	}
#ifdef _DEBUG
	fclose(fp);
#endif
	return 0;
}
