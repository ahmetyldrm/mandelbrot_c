#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct RGB_Color {
	unsigned char r, g, b;
	bool errorFlag;
} RGB_Color;

//Converts hex string to RGB_Color
RGB_Color getRGBfromHexStr(char* hexStr) {
	RGB_Color color;
	char tempStr[3];

	if (strlen(hexStr) != 6) {
		color.errorFlag = true;
		return color;
	}
	for (int i = 0; i < strlen(hexStr); i += 2) {
		strncpy(tempStr, hexStr + i, 2);
		tempStr[2] = '\0';
		switch (i)
		{
		case 0:
			color.r = (unsigned char)strtoul(tempStr, NULL, 16);
			break;
		case 2:
			color.g = (unsigned char)strtoul(tempStr, NULL, 16);
			break;
		case 4:
			color.b = (unsigned char)strtoul(tempStr, NULL, 16);
			break;
		default:
			break;
		}
	}
	color.errorFlag = false;
	return color;
}
