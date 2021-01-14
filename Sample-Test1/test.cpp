#include "pch.h"

//#include "../mandelbrot/mandelbrot.h"
#include "../mandelbrot/mandelbrot.c"
#include "../mandelbrot/test.c"



TEST(TestCaseName, TestName) {
	for (int i = -10; i < 10; i++) {
		EXPECT_EQ(getMandelbrotIterCount(_LCbuild(0.0, i / 10.0)), mandelbrot(0.0, i / 10.0));
	}
}

TEST(EqualityTest, IsEqual) {
	EXPECT_EQ(getMandelbrotIterCount(_LCbuild(-1.0, 0.0)), mandelbrot(-1.0, 0.0)) << "Incorrect return value";
}