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

int poll_gps_data(void)
{
	return 0;
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
