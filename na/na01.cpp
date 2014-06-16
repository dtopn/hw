#include <stdio.h>

void Series_Sum(double sum[]);

int main() {
	int i;
	double x, sum[3001];

	Series_Sum(sum);

	x = 0.0;
	for (i = 0; i<3001; i++)
		printf("%6.2lf %16.10lf\n", x + (double)i * 0.10, sum[i]);
	/*
  0.00   1.6449340668 48
  0.10   1.5346072449 04
...
  1.00   1.0000000000 00
...
  2.00   0.7500000000 00
...
300.00   0.0209422129 34
	*/
	return 0;
}

#include <math.h>

void Series_Sum(double *sum) {
	int i;
	double current_sum = 0.0;
	double stop = 0.0;
	double k = 1.0;
	double x = 0.0;
	for (i = 0; i <= 10; ++i) {
		x = i * 0.1;
		stop = sqrt(10e10 / 2.0 * fabs((double)1 - x));
		current_sum = 0.0;
		for (k = 1.0; k <= stop; k += 1.0) {
		//for (k = 1.0; k <= 100000; k += 1.0) {
			current_sum += 1.0 / (k*(k + 1.0)*(k + x));
		}
		*(sum + i) = 1.0 + current_sum * (1.0 - x);// +0.5 * 10e-10;
	}
	for (int i = 10; i < 3001; i++) {
		sum[i] = sum[i - 10] * ((double)i / 10.0  - 1.0) / (i / 10.0) + 1.0 / (i / 10.0) / (i / 10.0);
	}
	/*
	for ( ; i <= 3000; ++i) {
	x = i * 0.1;
	stop = sqrt(10e10 / 2.0);
	current_sum = 0.0;
	for (k = 1.0; k <= stop; k += 1.0) {
	//for (k = 1.0; k <= 100000; k += 1.0) {
	current_sum += (1.0 - x) / (k*(k + 1.0)*(k + x));
	}
	*(sum + i) = 1.0 + current_sum;// +0.5 * 10e-10;
	}
	*/
	return;
}