#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "colormap.h"

//#define LENGHTOF(x)  (sizeof(x) / sizeof((x)[0]))

CURRENT_HEX_ARRAY_LENGTH = 0;

//Converts hex string to RGB_Color
RGB_Color getRGBfromHexStr(char* hexStr) {
	RGB_Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.errorFlag = false;
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

void freeHexArray(char** hexarray) {
	for (int i = 0; i < CURRENT_HEX_ARRAY_LENGTH; i++) {
		if (hexarray[i] != NULL) {
			free(hexarray[i]);
			hexarray[i] = NULL;
			printf("hexarray[%d] freed\n", i);
		}
	}
	if (hexarray != NULL) {
		free(hexarray);
		hexarray = NULL;
		printf("--> hexarray freed\n");
	}
}

char** getHexArrayFromFile(char* filename) {
	//Must be freed! Call 'freeHexArray(char** hexarray)'

	char** hexArray = malloc(MAX_COLOR_ARRAY_LENGTH * sizeof(char*));
	if (hexArray == NULL) {
		printf("Could not allocated hexArray\n");
		return NULL;
	}
	char hexStr[8] = "";


	FILE* fileptr = fopen(filename, "r");
	if (fileptr == NULL) {
		printf("Could not open file: '%s'\n", filename);
		return NULL;
	}
	for (int i = 0; (i < MAX_COLOR_ARRAY_LENGTH) && (fgets(hexStr, 8, fileptr) != NULL); i++) {
		hexStr[6] = '\0';
		hexArray[i] = (char*)malloc(HEX_STR_LENGTH * sizeof(char));
		if (hexArray[i] == NULL) {
			printf("Could not allocated hexArray[%d]\n", i);
			return NULL;
		}
		strncpy(hexArray[i], hexStr, HEX_STR_LENGTH * sizeof(char));
		printf("hexStr = %s\n", hexStr);
		printf("hexArray[%d] = %s\n", i, hexArray[i]);
		*hexStr = "";
		CURRENT_HEX_ARRAY_LENGTH = i + 1;
	}
	fclose(fileptr);
	return hexArray;
}

float getMappedValue(int _value, int _valuestart, int _valueend, int _mappedstart, int _mappedend) {
	float result = (_value - _valuestart) * (_mappedend - _mappedstart) / (float)(_valueend - _valuestart) + _mappedstart;
	return result;
}
