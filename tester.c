#include <time.h>
clock_t start, stop; /* clock_t is a built-in type for processor time (ticks) */

#include <stdlib.h>
#include <stdio.h>

#include "searchAlgo.h"

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: tester size target K, size is the size of array, and target is the number you want to find, K is the repetition index.\n");
		return 0;
	}
	int size = atoi(argv[1]);
	int array[10000];
	int i = 0;
	for ( ; i < size; i++) array[i] = i;
	for ( ; i < 10000; i++) array[i] = -1;
	int target = atoi(argv[2]);
	int K = atoi(argv[3]);

	double duration; /* records the run time (seconds) of a function */
	/* clock() returns the amount of processor time (ticks) that has elapsed
	since the program began running */
	start = clock(); /* records the ticks at the beginning of the function call */
	//function(); /* run your function here */
	int ret;
	for (i = 0; i < K; i++) {
		ret = binaryRecur(array, 0, size, target);
//		ret = sequenRecur(array, 0, size, target);
//		ret = binaryItera(array, size, target);
//		ret = sequenItera(array, size, target);
	}
	stop = clock(); /* records the ticks at the end of the function call */
	duration = ((double)(stop - start))/CLK_TCK;
	/* CLK_TCK is a built-in constant = ticks per second */
	printf("%f\n", duration);
}
