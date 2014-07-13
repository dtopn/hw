#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <sys/time.h>
#include <linux/ioctl.h>
#include <linux/rtc.h>*/
#include <time.h>
#include <unistd.h>

#include <signal.h>
#include <pthread.h>
#include <errno.h>
/* Intruoduce Token structure */
#include "Token.h"


pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

void *send_msg(void* arg) {
	int fd = *(int*)arg;
	int cnt, i, res;
	int nap_time;
	int buff_len;
	char buff [81];

	cnt = 0;
	do {
		if (pthread_mutex_trylock (&queue_mutex) != EBUSY) {
			buff_len = rand() % 71 + 10;
			for (i = 0; i < buff_len; ++i) {
				switch (rand() % 3) {
					case 0: buff[i] = rand() % 26 + 'A'; break;
					case 1: buff[i] = rand() % 26 + 'a'; break;
					case 2: buff[i] = rand() % 10 + '0'; break;
				}
			}
			buff[buff_len] = '\0';
			res = write(fd, buff, strlen(buff)+1);
			cnt++;
			pthread_mutex_unlock (&queue_mutex);
		}
		/* I'd like to take a nap */
		usleep(rand() % 9 + 1);
	} while (cnt < 100);
	return 0;
}

struct fd_pack
{
	int fd1;
	int fd2;
};

void *read_msg(void* arg) {
	
	int cnt, i, res;
	int nap_time;
	int buff_len;
	char buff [81];
	struct Token token_buff;

	/* Pass file descripter into fd1, fd2 */
	struct fd_pack *fdp = (struct fd_pack*)arg;
	int fd1, fd2;
	fd1 = fdp->fd1;
	fd2 = fdp->fd2;

	int old_cancel_state;
	while(1) {
		pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);
		res = 0;
		while (1) {
			res = read(fd1, &token_buff, sizeof(struct Token));
			/* Read until empty */
			if (res != -1) {
				printf("'dev1 %d %lld %lld %s'\n", token_buff.id, 
					token_buff.in_stamp, token_buff.out_stamp, token_buff.msg);
			}
			else break;
		}
		res = 0;
		while (1) {
			res = read(fd2, &token_buff, sizeof(struct Token));
			/* Read until empty */
			if (res != -1) {
				printf("'dev2 %d %lld %lld %s'\n", token_buff.id, 
					token_buff.in_stamp, token_buff.out_stamp, token_buff.msg);
			}
			else break;
			//printf("'%d %d %d %d %s'\n", token_buff.in_stamp_h, token_buff.in_stamp_l,
			//							 token_buff.out_stamp_h, token_buff.out_stamp_l, token_buff.msg);
			//if there is no byte read		
		}
		/* I'd like to take a nap */
		usleep(rand() % 9 + 1);
		pthread_setcancelstate (old_cancel_state, NULL); // Safer than ENABLE
		pthread_testcancel();
	}
}


int main(int argc, char **argv)
{
	int fd1, fd2, res;
	char buff [1024];
	//TODO: remove string buff
	struct Token token_buff;
	int i = 0;
	srand (time(NULL));

	/* open devices */
	fd1 = open("/dev/Squeue1", O_RDWR);
	fd2 = open("/dev/Squeue2", O_RDWR);
	if (fd1 < 0 || fd2 < 0){
		printf("Can not open device file.\n");		
		return 0;
	}
	else {
		if (argc >= 2) {
			if(strcmp("show", argv[1]) == 0) {
				//memset(buff, 0, 1024);
				res = 0;
				while (1) {
					res = read(fd1, &token_buff, sizeof(struct Token));
					if (res != -1) {
						printf("'dev1 %lld %lld %s'\n", token_buff.in_stamp, token_buff.out_stamp, token_buff.msg);
					}
					else break;
				}
				res = 0;
				while (1) {
					res = read(fd2, &token_buff, sizeof(struct Token));
					if (res != -1) {
						printf("'dev2 %lld %lld %s'\n", token_buff.in_stamp, token_buff.out_stamp, token_buff.msg);
					}
					else break;
					//printf("'%d %d %d %d %s'\n", token_buff.in_stamp_h, token_buff.in_stamp_l,
					//							 token_buff.out_stamp_h, token_buff.out_stamp_l, token_buff.msg);
					//if there is no byte read		
				}
			}
			else if(strcmp("write1", argv[1]) == 0) {
				memset(buff, 0, 1024);
				if(argc >= 3){
					sprintf(buff,"%s", argv[2]);
					for(i = 3; i < argc; i++)
						sprintf(buff,"%s %s",buff,argv[i]);
				}

				int buff_len = rand() % 71 + 10;
				for (i = 0; i < buff_len; ++i) {
					buff[i] = rand() % 94 + 32;
				}
				buff[buff_len] = '\0';
				printf("'%s'\n", buff);
				res = write(fd1, buff, strlen(buff)+1);
				if(res == strlen(buff)+1){
					printf("Can not write to the device1 file.\n");		
					return 0;
				}	
			}
			else if(strcmp("write2", argv[1]) == 0) {
				memset(buff, 0, 1024);
				if(argc >= 3){
					sprintf(buff,"%s", argv[2]);
					for(i = 3; i < argc; i++)
						sprintf(buff,"%s %s",buff,argv[i]);
				}

				int buff_len = rand() % 71 + 10;
				for (i = 0; i < buff_len; ++i) {
					buff[i] = rand() % 94 + 32;
				}
				buff[buff_len] = '\0';
				printf("'%s'\n", buff);
				res = write(fd2, buff, strlen(buff)+1);
				if(res == strlen(buff)+1){
					printf("Can not write to the device2 file.\n");		
					return 0;
				}	
			}
		}
		else {
			pthread_mutex_t mutex;
			pthread_mutex_init (&mutex, NULL);

			pthread_t thread1_id;
			pthread_t thread2_id;
			pthread_t thread3_id;
			pthread_t thread4_id;
			pthread_t thread5_id;
			pthread_t thread6_id;
			pthread_create (&thread1_id, NULL, &send_msg, &fd1);
			pthread_create (&thread2_id, NULL, &send_msg, &fd1);
			pthread_create (&thread3_id, NULL, &send_msg, &fd1);
			pthread_create (&thread4_id, NULL, &send_msg, &fd2);
			pthread_create (&thread5_id, NULL, &send_msg, &fd2);
			pthread_create (&thread6_id, NULL, &send_msg, &fd2);

			struct fd_pack fds = {fd1, fd2};
			pthread_t thread0_id;
			pthread_create (&thread0_id, NULL, &read_msg, &fds);

			pthread_join (thread1_id, NULL);
			pthread_join (thread2_id, NULL);
			pthread_join (thread3_id, NULL);
			pthread_join (thread4_id, NULL);
			pthread_join (thread5_id, NULL);
			pthread_join (thread6_id, NULL);
		}
		/* close devices */
		close(fd1);
		close(fd2);
	}
	return 0;
}
