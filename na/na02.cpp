#include <stdio.h>
#include <math.h>

#define ZERO 0.000000001 /* X is considered to be 0 if |X|<ZERO */
#define MAXN 13   /* Max Polynomial Degree + 1 */


double Polynomial_Root(int n, double c[], double a, double b, double EPS);

int main() {
	freopen("in2.txt", "r", stdin);
	int n;
	double c[MAXN], a, b;
	double EPS = 0.00005;
	int i;

	while (scanf("%d", &n) != EOF) {
		for (i = n; i >= 0; i--)
			scanf("%lf", &c[i]);
		scanf("%lf %lf", &a, &b);
		printf("%.4lf\n", Polynomial_Root(n, c, a, b, EPS));
	}
	return 0;
}


#define MAX_I 10
#define NEAR_ZERO 2
#define OK 1
#define FAIL 0;

double f(int n, double c[], double x) {
	double temp = 0.0;
	for (int i = n; i >= 0; i--) {
		temp = temp * x + c[i];
	}
	return temp;
}

int Newton(double *root, int n, double c[], double cd1[], double cd2[], double p0, double TOL, int mod, double a, double b) {
	int i = 1 << MAX_I;
	double p;
	double temp0, temp1, temp2;
	double temp;
	while (i--) {
		if (!mod) {
			temp0 = f(n, c, p0);
			temp1 = f(n - 1, cd1, p0);
			if (fabs(temp1) < ZERO && fabs(temp0) > 1.0) {
				*root = p;
				return NEAR_ZERO;
			}
			p = p0 - f(n, c, p0) / f(n - 1, cd1, p0);
		}
		else {
			temp0 = f(n, c, p0);
			temp1 = f(n - 1, cd1, p0);
			temp2 = f(n - 2, cd2, p0);
			temp = temp1 * temp1 - temp0 * temp2;
			if (fabs(temp) < ZERO && fabs(temp0) < ZERO) {
				if (p < a || p > b) return FAIL;
				*root = p;
				return OK;
			}
			p = p0 - temp0 * temp1 / temp;
			//p = temp0 /(temp1 - temp0 * temp2 / temp1) ;
		}
		if (fabs(p - p0) < ZERO) {
			if (p < a || p > b) return FAIL;
			*root = p;
			return OK;
		}
		p0 = p;
	}
	return FAIL;
}

#define RESUSITATE_NEWTON_SAMA Newton(&root, n, c, cd1, cd2, p0, EPS, mod, a, b)

double Polynomial_Root(int n, double c[], double a, double b, double EPS) {
	//if (n == 1) return -c[1] / c[0];
	double cd1[MAXN], cd2[MAXN]; // coefficients of derivative
	for (int i = 0; i < n; i++) {
		cd1[i] = c[i + 1] * (1.0 + i);
	}
	for (int i = 0; i < n; i++) {
		cd2[i] = cd1[i + 1] * (1.0 + i);
	}
	double temp;
	if (a > b) {
		temp = a;
		a = b;
		b = temp;
	}
	int mod, status;

	double root, p0;
	while (1) {
		mod = 0;
		if (n >= 3) mod = 1; // final shoot
		p0 = a;
		if (RESUSITATE_NEWTON_SAMA == 1) break;
		p0 = b;
		if (RESUSITATE_NEWTON_SAMA == 1) break;
		p0 = (a + b) * 0.5;
		if (RESUSITATE_NEWTON_SAMA == 1) break;

		break;
	}
	

	if (fabs(root) <= EPS) root = fabs(root);
	return root;
}