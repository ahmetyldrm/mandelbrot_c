#include "mandelbrot.h"
#include <stdio.h>

int main4() {
	for (int i = -10; i < 10; i++) {
		printf("%d ", getMandelbrotIterCount(0.0, i / 10.0));
	}
	return 0;
}