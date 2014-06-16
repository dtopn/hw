#include <stdio.h>
#include <math.h>
#define MAX_m 200
#define MAX_n 5

double f1(double x) {
	return sin(x);
}

double f2(double x) {
	return exp(x);
}

int OPA(double(*f)(double t), int m, double x[], double w[],
	double c[], double *eps);

void print_results(int n, double c[], double eps) {
	int i;

	printf("%d\n", n);
	for (i = 0; i <= n; i++)
		printf("%8.4e ", c[i]);
	printf("\n");
	printf("error = %6.2e\n", eps);
	printf("\n");
}

int main() {
	int m, i, n;
	double x[MAX_m], w[MAX_m], c[MAX_n + 1], eps;

	m = 90;
	for (i = 0; i<m; i++) {
		x[i] = 3.1415926535897932 * (double)(i + 1) / 180.0;
		w[i] = 1.0;
	}
	eps = 0.001;
	n = OPA(f1, m, x, w, c, &eps);
	print_results(n, c, eps);

	m = 200;
	for (i = 0; i<m; i++) {
		x[i] = 0.01*(double)i;
		w[i] = 1.0;
	}
	eps = 0.001;
	n = OPA(f2, m, x, w, c, &eps);
	print_results(n, c, eps);

	return 0;
}

/*class Phi_coeff;

class Array {
public:
	Array(int m = 0) : m(m) {
		for (int i = 0; i < m; i++) {
			var[m] = 0.0;
		}
	};
	template <typename T>
	Array(T& a) {
		m = a.m;
		for (int i = 0; i < m; i++) {
			var[i] = a.var[i];
		}
	}
	Array mul(double *x) {
		Array temp(m);
		for (int i = 0; i < m; i++) {
			temp[i] = var[i] * x[i];
		}
		return Array(temp);
	}
	Array mut(double &x) {
		Array temp(m);
		for (int i = 0; i < m; i++) {
			temp[i] = var[i] * x;
		}
		return temp;
	}
	Array powerUp() {
		Array temp(m);
		for (int i = 1; i < m; i++) {
			temp.var[i] = var[i - 1];
		}
		temp.var[0] = 0.0;
		return temp;
	}
	template <typename T> double operator* (T &b) {
		double sum;
		sum = .0;
		for (int i = 0; i < m; i++) {
			sum += this->var[i] * b[i];
		}
		return sum;
	}
	template <typename T> Array operator+ (T &b) {
		Array temp(m);
		for (int i = 0; i < m; i++) {
			temp[i] = this->var[i] + b[i];
		}
		return temp;
	}
	template <typename T> Array operator- (T b) {
		Array temp(m);
		for (int i = 0; i < m; i++) {
			temp[i] = this->var[i] - b[i];
		}
		return temp;
	}
	double& operator [](int i) {
		return var[i];
	}
	//template <typename T1, typename T2> friend Array operator-(T1&, T2&);
protected:
	int m;
	double var[MAX_m];
};


class Phi_eval: public Array {
public: 
	Phi_eval(int m = 0) : Array(m) {};
	Phi_eval(Array &a) : Array(a) {};
protected:
	//double *val = phi::var; // evaluated result of coefficient*x^i
};

class Phi_coeff: public Array{
public:
	Phi_coeff(int m = 0) : Array(m) { coefficient = Array::var; };
	Phi_coeff(Array &a) : Array(a) { coefficient = Array::var; };
	Phi_coeff(Array a) : Array(a) { coefficient = Array::var; };
	Phi_coeff powerUp() {
		Phi_coeff temp(m);
		for (int i = 1; i < m; i++) {
			temp.var[i] = var[i - 1];
		}
		temp.var[0] = 0.0;
		return temp;
	}
protected:
	double *coefficient; //coeffi of x^0 to x^n
};
template<typename T1, typename T2>
Array operator- (T1 &a, T2 &b) {
	int m = a.m;
	Array temp(m);
	for (int i = 0; i < m; i++) {
		temp[i] = a.var[i] - b.var[i];
	}
	return temp;
}
*/

double calc(double* a, double* b, int m) {
	double temp = 0.0;
	for (int i = 0; i < m; i++) {
		temp += a[i] * b[i];
	}
	return temp;
}

double* calc(double* a, double* temp, int m, double* mult) {
	for (int i = 0; i < m; i++) {
		temp[i] = a[i] * mult[i];
	}
	return temp;
}

double* calc(double* a, double* temp, int m, double mult) {
	for (int i = 0; i < m; i++) {
		temp[i] = a[i] * mult;
	}
	return temp;
}

#include <string.h>

int OPA(double(*f)(double t), int m, double* x, double w[],
	double c[], double *eps) {
	double a[MAX_n + 1], y[MAX_m + 1];
	double err = 0.0;
	double B, C;
	double TOL = *eps;
	double phi[3][MAX_m + 1];
	double coeff[3][MAX_m + 1];
	double temp[MAX_m + 1];

	for (int i = 0; i <= MAX_n; i++) {
		c[i] = 0.0;
	}
	for (int i = 0; i <= m; i++) {
		y[i] = f(x[i]);
	} // input

	/*for (int i = 0; i < 3; i++) {
		phi[i] = Array(m);
		coeff[i] = Array(m);
	}*/
	for (int i = 0; i <= m; i++) {
		phi[0][i] = 1.0;
		coeff[0][i] = 0 == i ? 1.0 : 0.0;
		coeff[1][i] = 0.0;
		phi[1][i] = 0.0;
		coeff[2][i] = 0.0;
		phi[2][i] = 0.0;
		//printf("xi %lf\n", x[i]);
	}
	a[0] = calc(phi[0], y, m) / calc(phi[0], phi[0], m);
	c[0] = a[0] * coeff[0][0];
	for (int i = 0; i < m; i++) {
		err += y[i] * y[i];
	}
	err -= calc(phi[0], y, m) * a[0];
	// step 1 done

	B = calc(calc(phi[0], temp, m, x), phi[0], m) / calc(phi[0], phi[0], m);
	
	for (int i = 0; i <= m; i++) {
		phi[1][i] = x[i] - B;
	}
	coeff[1][1] = 1.0; coeff[1][0] = -B;
	a[1] = calc(phi[1], y, m) / calc(phi[1], phi[1], m);
	c[1] += a[1] * coeff[1][1]; c[0] += a[1] * coeff[1][0];
	//printf("a1 %lf\n", a[1]);
	//printf("c1 %lf\n", c[1]);
	err -= a[1] * calc(phi[1], y, m);
	//printf("err %lf\n", err);
	//step 2 done
	int k = 1;
	while (k < MAX_n && fabs(err) > TOL) {
		k++; // step 5
		B = calc(calc(phi[1], temp, m, x), phi[1], m) / calc(phi[1], phi[1], m);
		C = calc(calc(phi[1], temp, m, x), phi[0], m) / calc(phi[0], phi[0], m);

		for (int i = 0; i <= m; i++) {
			phi[2][i] = phi[1][i] * (x[i] - B) - C * phi[0][i];
		}
		a[k] = calc(phi[2], y, m) / calc(phi[2], phi[2], m);
		//coeff[2] = coeff[1].powerUp() - coeff[1].mut(B) - coeff[0].mut(C);
		coeff[2][0] = - B * coeff[1][0] - C * coeff[0][0];
		for (int i = 1; i <= m; i++) {
			coeff[2][i] = coeff[1][i - 1] - B * coeff[1][i] - C * coeff[0][i];
		}
		for (int i = 0; i <= MAX_n; i++) {
			c[i] += a[k] * coeff[2][i];
		}
		err -= a[k] * calc(phi[2], y, m); //step 6
		memcpy(phi[0], phi[1], sizeof(phi[1]));
		memcpy(phi[1], phi[2], sizeof(phi[2]));
		memcpy(coeff[0], coeff[1], sizeof(coeff[1]));
		memcpy(coeff[1], coeff[2], sizeof(coeff[2]));
		//phi[0] = phi[1]; phi[1] = phi[2];
		//coeff[0] = coeff[1]; coeff[1] = coeff[2]; //step 7
	}
	*eps = err;
	return k;
}
