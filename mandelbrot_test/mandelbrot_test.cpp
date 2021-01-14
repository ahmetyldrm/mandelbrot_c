
#include "pch.h"
#include "CppUnitTest.h"
extern "C" {
#include "main.h"
}
//extern "C" int mandelbrot(double real, double imag);
//for (int i = -10; i < 10; i++) {
//	printf("%d ", mandelbrot(0.0, i / 10.0));
//}

int main() {
	mandelbrot(-1.0, 0.0);
}