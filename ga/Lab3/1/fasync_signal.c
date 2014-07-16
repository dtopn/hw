#include <stdio.h>
#include <fcntl.h>
#include <linux/hpet.h>
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

jmp_buf env;

int main () {
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
	unsigned long		freq;
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