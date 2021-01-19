#pragma once
#pragma warning(disable : 4996)

#define MAX_COLOR_ARRAY_LENGTH 256	// max 256 color allowed
#define HEX_STR_LENGTH 7			// 6 for color hex chars, 1 for '\0' 

int CURRENT_HEX_ARRAY_LENGTH;


typedef struct RGB_Color {
	unsigned char r, g, b;
	bool errorFlag;
} RGB_Color;

//typedef char _hexString[7];

void freeHexArray(char** hexarray);
RGB_Color getRGBfromHexStr(char* hexStr);
char** getHexArrayFromFile(char* filename);
float getMappedValue(int _value, int _valuestart, int _valueend, int _mappedstart, int _mappedend);