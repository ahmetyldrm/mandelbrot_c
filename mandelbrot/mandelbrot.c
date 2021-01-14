
#include "mandelbrot.h"

int mandelbrot(double real, double imag)
{
	double real0 = real;
	double imag0 = imag;
	int iter_count = 0;
	double real_temp = 0.0;

	while (iter_count < MAXITER) {
		if ((real * real) + (imag * imag) >= 4.0) {
			break;
		}
		iter_count++;
		real_temp = real;
		real = (real * real) - (imag * imag) + real0;
		imag = 2 * real_temp * imag + imag0;
	}
	return iter_count;
}

_Lcomplex addLComplex(_Lcomplex z1, _Lcomplex z2) {
	return _LCbuild(creall(z1) + creall(z2), cimagl(z1) + cimagl(z2));

}

int getMandelbrotIterCount(_Lcomplex c) {
	_C_ldouble_complex c0 = c;
	int iter_count = 0;
	while (iter_count < MAXITER) {
		if (cabsl(c) >= 2.0) {
			break;
		}
		iter_count++;
		c = addLComplex(_LCmulcc(c, c), c0);
	}
	return iter_count;
}