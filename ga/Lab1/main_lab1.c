#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <sys/time.h>
#include <linux/ioctl.h>
#include <linux/rtc.h>*/
#include <time.h>

#include <pthread.h>

/* Intruoduce Token structure */
#include "Token.h"

void *thread_routine(void* arg) {
	printf("Inside newly created thread \n");
}

int main(int argc, char **argv)
{
	int fd1, fd2, res;
	char buff [1024];
	//TODO: remove string buff
	struct Token token_buff;
	int i = 0;

	if(strcmp("0", argv[1]) == 0){
		i=100/i;
	}else if(strcmp("1", argv[1]) == 0){
		;
	}

	/* open devices */
	fd1 = open("/dev/Squeue1", O_RDWR);
	fd2 = open("/dev/Squeue2", O_RDWR);
	if (fd1 < 0 || fd2 < 0){
		printf("Can not open device file.\n");		
		return 0;
	}else{
		if(strcmp("show", argv[1]) == 0){
			//memset(buff, 0, 1024);
			res = read(fd1, &token_buff, sizeof(struct Token));
			printf("'%d %d %s'\n", token_buff.in_stamp, token_buff.out_stamp, token_buff.msg);
			res = read(fd2, &token_buff, sizeof(struct Token));
			printf("'%d %d %s'\n", token_buff.in_stamp, token_buff.out_stamp, token_buff.msg);
			sleep(1);
			//ss
		}else if(strcmp("write1", argv[1]) == 0){
			memset(buff, 0, 1024);
			if(argc >= 3){
				sprintf(buff,"%s", argv[2]);
				for(i = 3; i < argc; i++)
					sprintf(buff,"%s %s",buff,argv[i]);
			}
			printf("'%s'\n", buff);
			res = write(fd1, buff, strlen(buff)+1);
			if(res == strlen(buff)+1){
				printf("Can not write to the device1 file.\n");		
				return 0;
			}	
		}else if(strcmp("write2", argv[1]) == 0){
			memset(buff, 0, 1024);
			if(argc >= 3){
				sprintf(buff,"%s", argv[2]);
				for(i = 3; i < argc; i++)
					sprintf(buff,"%s %s",buff,argv[i]);
			}
			printf("'%s'\n", buff);
			res = write(fd2, buff, strlen(buff)+1);
			if(res == strlen(buff)+1){
				printf("Can not write to the device2 file.\n");		
				return 0;
			}	
		}
		/* close devices */
		close(fd1);
		close(fd2);
	}
	return 0;
}
