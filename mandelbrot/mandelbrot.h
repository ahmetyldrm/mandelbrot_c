#pragma once

//#include <complex.h>

#define MAND_MAX_ITER 255

int getMandelbrotIterCount(double real, double imag); // fast
//int getMandelbrotIterCount_LC(_Lcomplex c);  // slow - uses long double complex as argument