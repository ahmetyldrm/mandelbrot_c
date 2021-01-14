#pragma once

#include <complex.h>

#define MAXITER 255

int getMandelbrotIterCount(double real, double imag); // fast
int getMandelbrotIterCount_LC(_Lcomplex c);  // slow - uses long double complex as argument