#include "mandelbrot.h"
#include <stdio.h>
#include <stdlib.h>

int* ptr;

int main__() {
	ptr = (int*)malloc(6 * sizeof(int));
	if (ptr) {
		ptr[1] = 16;
		ptr[3] = 189;
		ptr[5] = 128;
		for (int i = 0; i < 6; i++) {
			printf("%d ", ptr[i]);
		}
	}
	free(ptr);
	return 0;
}