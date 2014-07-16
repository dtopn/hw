#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

int i, j; 
long T0; 
jmp_buf env;

void alarm_hdlr(int x) {
 	long t1 = time(0) - T0;
 	printf("%ld sec%s passed: j=%d.i=%d\n",t1, (t1==1)?"":"s",j,i);
 	if (t1 >= 8) { 
		printf("Giving up\n"); 
		longjmp(env,1); 
	}
 	alarm(1);
}

int main() {
 	signal(SIGALRM, alarm_hdlr); alarm(1); 
 	if (setjmp(env)!=0) {
 		printf("Gave up: j=%d,i=%d\n",j,i); exit(1);
 	} 
 	T0 = time(0); 
 	for (j = 0; j < 10000; j++) for (i = 0; i < 1000000; i++);
}

//(from https://www.cs.purdue.edu/homes/cs240/lectures/Lecture-19.pdf)
