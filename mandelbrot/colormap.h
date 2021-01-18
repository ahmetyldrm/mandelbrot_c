#pragma once

typedef struct RGB_Color {
	unsigned char r, g, b;
	bool errorFlag;
} RGB_Color;

typedef char _hexString[7];

RGB_Color getRGBfromHexStr(char* hexStr);
_hexString* getHexArrayFromFile(char* filename);