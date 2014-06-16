#include <stdio.h>
#define MAX_SIZE 100

int EigenV(int n, double a[][MAX_SIZE], double *lambda, double v[],
	double TOL, int MAXN);

int main() {
	freopen("in5.txt", "r", stdin);
	int n, MAXN, m, i, j, k;
	double a[MAX_SIZE][MAX_SIZE], v[MAX_SIZE];
	double lambda, TOL;

	while (scanf("%d", &n) != EOF) {
		for (i = 0; i<n; i++)
		for (j = 0; j<n; j++)
			scanf("%lf", &a[i][j]);
		scanf("%lf %d", &TOL, &MAXN);
		scanf("%d", &m);
		for (i = 0; i<m; i++) {
			scanf("%lf", &lambda);
			for (j = 0; j<n; j++)
				scanf("%lf", &v[j]);
			switch (EigenV(n, a, &lambda, v, TOL, MAXN)) {
			case -1:
				printf("%12.8f is an eigenvalue.\n", lambda);
				break;
			case 0:
				printf("Maximum number of iterations exceeded.\n");
				break;
			case 1:
				printf("%12.8f\n", lambda);
				for (k = 0; k<n; k++)
					printf("%12.8f ", v[k]);
				printf("\n");
				break;
			}
		}
		printf("\n");
	}

	return 0;
}

#include <math.h>
#define ZERO 0.0000000001

int solve(int n, double aa[][MAX_SIZE], double *lambda, double* x, double* y) {
	double l[MAX_SIZE][MAX_SIZE], u[MAX_SIZE][MAX_SIZE];
	double a[MAX_SIZE][MAX_SIZE];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			a[i][j] = aa[i][j];
		}
	}
	for (int i = 0; i<n; i++) {
		l[i][i] = 1.0;
		a[i][i] -= *lambda;
	}
	double temp;
	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			temp = a[i][j];
			for (int k = 0; k < i; k++) temp -= l[i][k] * u[k][j];
			u[i][j] = temp;
		}
		for (int j = i + 1; j < n; j++) {
			temp = a[j][i];
			for (int k = 0; k < i; k++) temp -= l[j][k] * u[k][i];
			l[j][i] = temp / u[i][i];
		}
		if (fabs(u[i][i]) < ZERO) {
			return -1;
		}
	}
	for (int i = 0; i < n; i++) {
		temp = 0.0;
		for (int j = 0; j < i; j++) temp += l[i][j] * y[j];
		y[i] = x[i] - temp;
	}
	for (int i = n - 1; i >= 0; i--) {
		temp = 0.0;
		for (int j = i + 1; j < n; j++) temp += u[i][j] * y[j];
		y[i] = (y[i] - temp) / u[i][i];
	}
	return 0;
}

int EigenV(int n, double a[][MAX_SIZE], double *lambda, double x[],
	double TOL, int MAXN) {
	double q = *lambda;
	double y[MAX_SIZE];
	int k = 1;
	int p = 0;
	double err = 0.0;
	double miu;
	for (int i = 1; i < n; ++i) {
		if (fabs(x[i]) > fabs(x[p])) p = i;
	} //find max
	for (int i = 0; i < n; ++i) {
		x[i] /= x[p];
	} // normalize
	while (k <= MAXN) {
		if (solve(n, a, lambda, x, y) != 0) return -1;
		miu = y[p];
		p = 0;
		for (int i = 1; i < n; ++i) {
			if (fabs(y[i]) > fabs(y[p])) p = i;
		} //find max
		err = 0.0;
		for (int i = 0; i < n; i++) {
			double temp = fabs(x[i] - y[i] / y[p]);
			if (temp > err) err = temp;
			x[i] = y[i] / y[p];
		}
		if (err < TOL) {
			miu = 1.0 / miu + q;
			*lambda = miu;
			return 1;
		}
		k++;
	}
	/*
	//USE INVERSE POWER METHOD INSTEAD..
	//DEAR ME....
	//TAT..
	double x_temp[MAX_SIZE];
	while (k <= MAXN) {
		for (int i = 0; i < n; ++i) {
			x_temp[i] = 0.0;
			for (int j = 0; j < n; ++j) {
				x_temp[i] += v[j] * a[i][j];
			}
		} // iteration calc x_k+1 = A * x_k
		*lambda = x_temp[max_i];
		max_i = 0.0;
		for (int i = 0; i < n; ++i) {
			//v[i] = x_temp[i];
			if (fabs(x_temp[i]) > fabs(x_temp[max_i])) max_i = i;
		} // find max fabs
		if (fabs(x_temp[max_i]) < ZERO) return -1;
		err = 0.0;
		for (int i = 0; i < n; i++) {
			if (fabs(v[i] - x_temp[i] / x_temp[max_i]) > err) err = v[i] - x_temp[i] / x_temp[max_i];
			v[i] = x_temp[i] / x_temp[max_i];
		}
		if (fabs(err) < TOL) return 1;
		k++;
	}
	*/
	return 0;
}