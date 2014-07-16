#include <stdio.h>
#include <fcntl.h>
#include <linux/hpet.h>
#include <linux/input.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <setjmp.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>

#include <pthread.h>

extern void hpet_fasync();
#define TIMEOUT 20
#define CLICK_INTERVAL 200000
jmp_buf env;

#define MOUSEFILE "/dev/input/event2"
void *detect (void *arg) {
	int fd = *(int *)arg;
	struct input_event ie;
	struct timeval tv;
	fd_set readfds;
	int ret;

	/* Wait on mouse event for input. */
	FD_ZERO(&readfds);		// Clear fds
	
	struct timeval prv_click;
	prv_click.tv_sec = 0;
	prv_click.tv_usec = 0;

	while (1) {
		FD_SET(fd, &readfds);	// Put fd into fds

		tv.tv_sec = TIMEOUT;	// Set timeout value
		tv.tv_usec = 0;

		ret = select (fd + 1,	//max fd to monitor
					&readfds,
					NULL,
					NULL,
					&tv);

		if (ret == -1) {
			// select error occured
			perror ("Cannot select");
			return NULL;
		} else if (!ret) {
			// return 0 no event
			printf ("%d seconds elapsed. Nothing happend\n", TIMEOUT);
			close(fd);
			return NULL;
		}

		// At least one event can be read from device without blocking
		read(fd, &ie, sizeof(struct input_event));
		if (1 == ie.type && 273 == ie.code && 0 == ie.value) {
			//printf("time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n",
			//	ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);
			if ( (ie.time.tv_sec - prv_click.tv_sec) == 0 && ie.time.tv_usec - prv_click.tv_usec < CLICK_INTERVAL ||
				(ie.time.tv_sec - prv_click.tv_sec) == 1 && ie.time.tv_usec - prv_click.tv_usec + 1000000L < CLICK_INTERVAL
				) {
				//printf("--time %ld.%06ld\n", prv_click.tv_sec, prv_click.tv_usec);
				//printf("%s\n", "Double click");

				// Reset for next double click detection
				prv_click.tv_sec = 0;
				prv_click.tv_usec = 0;

				close(fd);
				// Perform async jump
				longjmp(env ,1);
			}
			else {
				prv_click.tv_sec = ie.time.tv_sec;
				prv_click.tv_usec = ie.time.tv_usec;
			}
		}
	}
	close(fd);
	return NULL;
}

pthread_t tid;

int main (int argc, const char** argv) {

	int i, j, ret;

	// Handle program arguments
	char evdev_file[80] = MOUSEFILE;
	if (1 == argc) {
		printf("Using default mouse event file %s\n", evdev_file);
		printf("If the program misbehaves, try ./this_execuatable other_evdev_path\n");
	} 
	else if (2 == argc) {
		printf("Using custom mouse event file: %s\n", argv[1]);
		sprintf(evdev_file, "%s", argv[1]);
	}


	/* Open evdev file */
	int fd;
	if((fd = open(evdev_file, O_RDONLY)) == -1) {
		printf("Error opening device\n");
		exit(EXIT_FAILURE);
	}

	// Create thread
	ret = pthread_create(&tid, NULL, detect, (void *)&fd);
	if (-1 == ret) {
		printf("Cannot create thread\n");
		return 1;
	}

	//Main Computation here
 	if (setjmp(env)!=0) {
 		printf("Right button double clicked: j=%d,i=%d\n",j,i);
 		return 1;
 	} 
 	for (j = 0; j < 10000; j++) for (i = 0; i < 1000000; i++);

	return 0;
}
