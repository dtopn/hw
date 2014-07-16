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


extern void hpet_fasync();
#define TIMEOUT 5
#define CLICK_INTERVAL 200000
jmp_buf env;

#define MOUSEFILE "/dev/input/event3"
int main () {
	int fd;
	struct input_event ie;
	struct timeval tv;
	fd_set readfds;
	int ret;

	/* Open evdev file */
	if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
		perror("opening device");
		exit(EXIT_FAILURE);
	}
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
			return 1;
		} else if (!ret) {
			// return 0 no event
			printf ("%d seconds elapsed. Nothing happend\n", TIMEOUT);
			return 0;
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

			}
			else {
				prv_click.tv_sec = ie.time.tv_sec;
				prv_click.tv_usec = ie.time.tv_usec;
			}
		}
	}


	return 0;
	hpet_fasync();
}

static int hpet_sigio_count;

static void
hpet_sigio(int val)
{
	printf("Signal handler called\n"); 
	longjmp(env,1); 
}

void
hpet_fasync()
{
	unsigned long		freq = 1;
	int			fd, value;
	sig_t			oldsig;
	struct hpet_info	info;
	int i, j;

	hpet_sigio_count = 0;
	fd = -1;

	if ((oldsig = signal(SIGIO, hpet_sigio)) == SIG_ERR) {
		fprintf(stderr, "hpet_fasync: failed to set signal handler\n");
		return;
	}

	// Open h High Precision E Timer
	fd = open("/dev/hpet", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "hpet_fasync: failed to open hpet\n");
		return;
	}
	
	// Set Device Control
	if ((fcntl(fd, F_SETOWN, getpid()) == 1) ||
		((value = fcntl(fd, F_GETFL)) == 1) ||
		(fcntl(fd, F_SETFL, value | O_ASYNC) == 1)) {
		fprintf(stderr, "hpet_fasync: fcntl failed\n");
		goto out;
	}

	freq = 1;
	if (ioctl(fd, HPET_IRQFREQ, freq) < 0) {
		fprintf(stderr, "hpet_fasync: HPET_IRQFREQ failed\n");
		goto out;
	}

	if (ioctl(fd, HPET_INFO, &info) < 0) {
		fprintf(stderr, "hpet_fasync: failed to get info\n");
		goto out;
	}

	//fprintf(stderr, "hpet_fasync: info.hi_flags 0x%lx\n", info.hi_flags);

	if (info.hi_flags && (ioctl(fd, HPET_EPI, 0) < 0)) {
		fprintf(stderr, "hpet_fasync: HPET_EPI failed\n");
		goto out;
	}

	if (ioctl(fd, HPET_IE_ON, 0) < 0) {
		fprintf(stderr, "hpet_fasync, HPET_IE_ON failed\n");
		goto out;
	}
	
	//Main Computation here
 	if (setjmp(env)!=0) {
 		printf("One second passed: j=%d,i=%d\n",j,i); exit(1);
 	} 
 	for (j = 0; j < 10000; j++) for (i = 0; i < 1000000; i++);

out:
	signal(SIGIO, oldsig);

	if (fd >= 0)
		close(fd);

	return;
}