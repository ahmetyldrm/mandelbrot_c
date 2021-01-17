#include "pch.h"

//#include "../mandelbrot/mandelbrot.h"
//#include "../mandelbrot/mandelbrot.c"
//#include "../mandelbrot/test.c"



TEST(TestCaseName, TestName) {
	char hexCodeStr[] = "ffd2fc";
	unsigned long hexCode;
	hexCode = strtoul(hexCodeStr, NULL, 16);
	EXPECT_EQ(hexCode, 16765692);
}

TEST(EqualityTest, IsEqual) {
	//EXPECT_EQ(, ) << "Incorrect return value";
}