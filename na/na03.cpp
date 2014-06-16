#include <stdio.h>
#define Max_size 10000 /* max number of dishes */
void Price(int n, double p[]);

int main() {
	int n, i;
	double p[Max_size] = { 32.11,40.06,52.99,23.64, 17.39, 12.77, 16.62, 10.67, 14.85, 12.68, 26.90, 28.30, 15.59, 37.99, 23.18 };;
	//n = 3;
	//Price(n, p);

	while (scanf("%d", &n) != EOF) {
		for (i = 0; i<n; i++)
			scanf("%lf", &p[i]);
		Price(n, p);
		for (i = 0; i<n; i++)
			printf("%.2f ", p[i]);
		printf("\n");
	}

	return 0;
}

/*
Sample Input
3 32.11 40.06 52.99
12 23.64 17.39 12.77 16.62 10.67 14.85 12.68 26.90 28.30 15.59 37.99 23.18

Sample Output
7.50 12.80 21.42
9.20 5.58 3.24 7.00 1.99 6.36 2.25 10.01 11.52 0.50 17.65 4.88
*/

void Price(int n, double p[]) {
	double l[Max_size + 1], sigma[Max_size + 1]; //L matrix
	double u[Max_size + 1], rou[Max_size + 1]; //U matrix
	double a, b, c;
	a = c = 0.5;
	b = 2.0;
	u[1] = b;
	rou[1] = a;
	sigma[1] = c / u[1];
	for (int i = 2; i <= n - 1; ++i) {
		l[i] = a / u[i - 1];
		u[i] = b - l[i] * c;
		rou[i] = -l[i] * rou[i - 1];
		sigma[i] = -sigma[i - 1] * c / u[i];
	}
	l[n] = a / u[n - 1];
	double temp;
	temp = 0.0;
	for (int i = 1; i <= n - 2; ++i) {
		temp += rou[i] * sigma[i];
	}
	u[n] = b - (sigma[n - 1] + l[n])*(c + rou[n - 1]) - temp;
	//LU done
	double y[Max_size + 1];
	y[1] = p[0];
	for (int i = 2; i <= n - 1; ++i) {
		y[i] = p[i - 1] - l[i] * y[i - 1];
	}
	temp = 0.0;
	for (int i = 1; i <= n - 2; ++i) {
		temp += sigma[i] * y[i];
	}
	y[n] = p[n - 1] - (l[n] + sigma[n - 1]) * y[n - 1] - temp;
	//y done
	p[n - 1] = y[n] / u[n];
	for (int i = n - 1; i >= 1; --i) {
		p[i - 1] = (y[i] - c * p[i] - rou[i] * p[n - 1]) / u[i];
	}
	//price done
}