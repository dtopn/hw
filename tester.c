#include <time.h>
clock_t start, stop; /* clock_t is a built-in type for processor time (ticks) */

#include <stdlib.h>
#include <stdio.h>

#include "searchAlgo.h"

int main(int argc, char *argv[]) {
	if (argc != 5) {
		printf("Usage: tester size target K func, \n\"size\" is the size of array, \n\"target\" is the number you want to find, \n\"K\" is the repetition index, \n\"func\" tells the program which function to call, feed the program with number 1 2 3 4\n");
		return 0;
	}
	int size = atoi(argv[1]);
	int array[10000];
	int i = 0;
	for ( ; i < size; i++) array[i] = i;
	for ( ; i < 10000; i++) array[i] = -1;
	int target = atoi(argv[2]);
	int K = atoi(argv[3]);
	char c = *(char *)argv[4];
	/* clock() returns the amount of processor time (ticks) that has elapsed
	since the program began running */
	start = clock(); /* records the ticks at the beginning of the function call */
	int ret;
	for (i = 0; i < K; i++) {
		switch (c) {
		case '1': ret = binaryRecur(array, 0, size, target); break;
		case '2': ret = sequenRecur(array, 0, size, target); break;
		case '3': ret = binaryItera(array, size, target); break;
		case '4': ret = sequenItera(array, size, target); break;
		}
	}
	stop = clock(); /* records the ticks at the end of the function call */
	double total = (double)(stop - start) / CLK_TCK;
	double duration = total / K; /* records the run time (seconds) of a function */
	/* CLK_TCK is a built-in constant = ticks per second */
	printf("%d\n%ld\n%.9lf\n%.9lf\n", K, stop - start, total, duration);
}
