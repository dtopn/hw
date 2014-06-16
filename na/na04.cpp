#include <stdio.h>
#include <math.h>

#define MAX_SIZE 100
#define bound pow(2.0, 127)
#define ZERO 0.000000001 /* X is considered to be 0 if |X|<ZERO */
#define dbg1
int Jacobi(int n, double a[][MAX_SIZE], double b[], double x[],
	double TOL, int MAXN);

int Gauss_Seidel(int n, double a[][MAX_SIZE], double b[], double x[],
	double TOL, int MAXN);

int main() {
	freopen("in4.txt", "r", stdin);
	int n, MAXN, i, j, k;
	double a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE], x[MAX_SIZE];
	double TOL;
	/*
3
2   -1  1   -1
2   2   2   4
-1  -1  2   -5
0.000000001 1000
3
1   2   -2  7
1   1   1   2
2   2   1   5
0.000000001 1000
5
2 1 0 0 0 1
1 2 1 0 0 1
0 1 2 1 0 1
0 0 1 2 1 1
0 0 0 1 2 1
0.000000001 100
	*/
	while (scanf("%d", &n) != EOF) {
		for (i = 0; i<n; i++) {
			for (j = 0; j<n; j++)
				scanf("%lf", &a[i][j]);
			scanf("%lf", &b[i]);
		}
		scanf("%lf %d", &TOL, &MAXN);

		printf("Result of Jacobi method:\n");
		for (i = 0; i<n; i++)
			x[i] = 0.0;
		k = Jacobi(n, a, b, x, TOL, MAXN);
		switch (k) {
		case -2:
			printf("No convergence.\n");
			break;
		case -1:
			printf("Matrix has a zero column.  No unique solution exists.\n");
			break;
		case 0:
			printf("Maximum number of iterations exceeded.\n");
			break;
		default:
			printf("no_iteration = %d\n", k);
			for (j = 0; j<n; j++)
				printf("%lf\n", x[j]);
			break;
		}

		printf("Result of Gauss-Seidel method:\n");
		for (i = 0; i<n; i++)
			x[i] = 0.0;
		k = Gauss_Seidel(n, a, b, x, TOL, MAXN);
		switch (k) {
		case -2:
			printf("No convergence.\n");
			break;
		case -1:
			printf("Matrix has a zero column.  No unique solution exists.\n");
			break;
		case 0:
			printf("Maximum number of iterations exceeded.\n");
			break;
		default:
			printf("no_iteration = %d\n", k);
			for (j = 0; j<n; j++)
				printf("%.8lf\n", x[j]);
			break;
		}
		printf("\n");
	}
	return 0;
}

#include <stdlib.h>
#include <string.h>
int k_global;

int Rearrage(int n, double a[][MAX_SIZE], double* b) {
	size_t row_size = sizeof(double)* MAX_SIZE;
	double *temp = (double*)malloc(row_size);
	for (int i = 0; i < n; ++i) {//column[i] scan 
		int max = i;
		for (int j = i + 1; j < n; ++j) {
			if (fabs(a[j][i]) > fabs(a[max][i])) {
				max = j;
			}
		} // scan for max fabs in column i downward
		if (fabs(a[max][i]) - ZERO > 0) {
			//memcpy(temp, a[i], row_size); // save row[i] to temp
			//memcpy(a[i], a[max], row_size); //copy row[max] to row[i]
			//memcpy(a[max], temp,row_size);
			double b_temp;
			for (int k = 0; k < n; k++) {
				b_temp = a[i][k];
				a[i][k] = a[max][k];
				a[max][k] = b_temp;
			}
			b_temp = b[i];
			b[i] = b[max];
			b[max] = b_temp;
		}
		else { //no value >0 downward go upward
			if (0 == i) return -1;//cannot go upward
			max = i - 1;
			for (int j = i - 1; j >= 0; --j) {// go upward now
				if (fabs(a[j][i]) > fabs(a[max][i])) {
					max = j;
				}
			} // scan for max fabs in column i upward
			if (fabs(a[max][i]) - ZERO > 0) { //there is one upward >0
				b[i] += b[max];
				for (int k = 0; k < n; ++k) a[i][k] += a[max][k];
			}
			else return -1; //no entry >0 this column
		}
		#ifdef dbg
		for (int i = 0; i < n; i++) {
			for (int j = 0 ; j < n; j++) {
				printf("%.8lf ", a[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		#endif
	}
	//free(temp);
	#ifdef dbg
	for (int i = 0; i < n; i++) {
		for (int j = 0 ; j < n; j++) {
			printf("%.8lf ", a[i][j]);
		}
		printf("\n");
	}
	#endif
	return 0;
}

int Jacobi(int n, double a[][MAX_SIZE], double b[], double x[],
	double TOL, int MAXN) {
	if ((k_global = Rearrage(n, a, b))== -1) return -1;
	int k;
	double sum;
	double max_upsilon;
	//double *temp = (double*)malloc(sizeof(double) * MAX_SIZE);
	double temp[MAX_SIZE];
	k = 1;
	while (k <= MAXN) {
		max_upsilon = 0.0;
		for (int i = 0; i < n; ++i) {
			sum = 0.0;
			for (int j = 0; j <= i - 1; ++j) sum += a[i][j] * x[j];
			for (int j = i + 1 ; j < n; ++j) sum += a[i][j] * x[j];
			temp[i] = (b[i] - sum) / a[i][i];
			if (fabs(temp[i] - x[i]) > max_upsilon) max_upsilon = fabs(temp[i] - x[i]);
			
		}
		
		//memcpy(x, temp, sizeof(double) * MAX_SIZE);
		for (int i = 0; i < n; i++) {
			x[i] = temp[i];
		}
		if (max_upsilon < TOL) return k;
		k++;
	}
	for (int i = 0; i < n; i++)
		if (fabs(x[i]) > bound) return -2;
	//free(temp);
	return 0;
}

int Gauss_Seidel(int n, double a[][MAX_SIZE], double b[], double x[],
	double TOL, int MAXN) {
	//if (k_global == -1) return -1;
	int k = 1;
	double sum, x_temp;
	double max_upsilon;
	while (k <= MAXN) {
		max_upsilon = 0.0;
		for (int i = 0; i < n; ++i) {
			sum = 0.0;
			for (int j = 0; j <= i - 1; ++j) sum += a[i][j] * x[j];
			for (int j = i + 1 ; j < n; ++j) sum += a[i][j] * x[j];
			x_temp = (b[i] - sum) / a[i][i];
			if (fabs(x_temp - x[i]) > max_upsilon) max_upsilon = fabs(x_temp - x[i]);
			x[i] = x_temp;
			//if (fabs(x_temp) > bound) return -2;
		}
		if (max_upsilon < TOL) return k;
		k++;
	}
	for (int i = 0; i < n; i++)
		if (fabs(x[i]) > bound) return -2;
	return 0;
}