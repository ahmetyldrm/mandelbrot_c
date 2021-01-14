#include "pch.h"

//#include "../mandelbrot/mandelbrot.h"
#include "../mandelbrot/mandelbrot.c"
#include "../mandelbrot/test.c"



TEST(TestCaseName, TestName) {
	for (int i = -10; i < 10; i++) {
		EXPECT_EQ(getMandelbrotIterCount_LC(_LCbuild(0.0, i / 10.0)), getMandelbrotIterCount(0.0, i / 10.0));
	}
}

TEST(EqualityTest, IsEqual) {
	EXPECT_EQ(getMandelbrotIterCount_LC(_LCbuild(-1.0, 0.0)), getMandelbrotIterCount(-1.0, 0.0)) << "Incorrect return value";
}