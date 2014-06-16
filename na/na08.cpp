#include <stdio.h>
#include <math.h>
double f0(double x, double l, double t) {
	return sqrt(1.0 + l*l*t*t*cos(t*x)*cos(t*x));
}

double Integral(double a, double b, double(*f)(double x, double y, double z),
	double eps, double l, double t);

int main() {
	freopen("in8.txt", "r", stdin);
	double a = 0.0, b, eps = 0.005, l, t;

	while (scanf("%lf %lf %lf", &l, &b, &t) != EOF)
		printf("%.2f\n", Integral(a, b, f0, eps, l, t));
	return 0;
}

#include <string.h>
#include <stdlib.h>
#define MAX_I 17 // maximum 17 iterations

double Romberg(double a, double b, double(*f)(double x, double y, double z),
	double eps, double l, double t);
//see http://www.cc98.org/dispbbs.asp?BoardID=47&id=3493935
//see http://www.cc98.org/dispbbs.asp?boardID=47&ID=3832766
double Integral(double a, double b, double(*f)(double x, double y, double z),
	double eps, double l, double t) {
	double R[3][MAX_I + 1];
	double sum;
	bool go = false;
	double pi = 3.1415926535897932;
	double T = 2.0 * pi / t;
	int T_count = int(floor(b / T));
	double b_temp = b - (double)T_count * T;
	double cycleLength = Romberg(0, T, f0, eps, l, t);
	double tempLength = Romberg(0, b_temp, f0, eps, l, t);
	return (cycleLength * T_count + tempLength) / 100;
}

double Romberg(double a, double b, double(*f)(double x, double y, double z),
	double eps, double l, double t) {
	double R[3][MAX_I + 1];
	double sum;
	bool go = false;

	double h = b - a;
	R[1][1] = h / 2.0 * (f(a, l, t) + f(b, l, t)); // step 1
	// empty // step 2
	for (int i = 2; i <= MAX_I; i++) { // step 3 (4-8loop)
		sum = 0.0;
		for (int k = 1; k <= 1 << (i - 2); k++) {
			sum += f((a + ((double)k - 0.5) * h), l, t);
		}
		R[2][1] = 0.5 * (R[1][1] + h * sum); // step 4
		for (int j = 2; j <= i; j++) {
			R[2][j] = R[2][j - 1] + (R[2][j - 1] - R[1][j - 1]) / (double)(1 << (2 * j - 2) - 1);
		} // step 5
		// empty // step 6
		h = h / 2.0;
		/*double err = fabs(R[2][i] - R[1][i - 1]);
		if (err < eps) {
			if (go) return R[2][i];
			else go = true;
		}
		else go = false;
		*/
		memcpy(R[1], R[2], sizeof(R[2]));
	}
	return R[1][MAX_I];
}