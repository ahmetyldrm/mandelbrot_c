#include "pch.h"
#include "..\mandelbrot\colormap.c"

//#include "../mandelbrot/mandelbrot.h"
//#include "../mandelbrot/mandelbrot.c"
//#include "../mandelbrot/test.c"



TEST(TestCaseName, TestName) {
	char hexCodeStr[] = "ffd2fc";
	unsigned long hexCode;
	hexCode = strtoul(hexCodeStr, NULL, 16);
	EXPECT_EQ(hexCode, 16765692);
}

TEST(RGBTest, IsEqual) {
	/*char* hexString = "D95750";
	RGB_Color color;
	color = getRGBfromHexStr(hexString);
	EXPECT_EQ(color.r, 217) << "Incorrect red value";
	EXPECT_EQ(color.g, 87) << "Incorrect green value";
	EXPECT_EQ(color.b, 80) << "Incorrect blue value";
	EXPECT_EQ(color.errorFlag, false) << "Length of hex string not right";*/
}

TEST(MapTest, IsEqual) {
	EXPECT_FLOAT_EQ(getMappedValueF(0.9f, 0.0f, 1.0f, 0.0f, 100.0f), 90.0f);
}