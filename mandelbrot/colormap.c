#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "colormap.h"

//#define LENGHTOF(x)  (sizeof(x) / sizeof((x)[0]))

//Stores the length of array obtained by getRGBArrayFromFile() or getHexArrayFromFile().
//Used to free memory
int CURRENT_HEX_ARRAY_LENGTH = 0;

//Converts hex string to RGB_Color
RGB_Color getRGBfromHexStr(char* hexStr) {
	RGB_Color color = { .r = 0, .g = 0, .b = 0, .errorFlag = false };
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

//Deprecated. Must be called after getHexArrayFromFile()
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

//Must be called after getRGBArrayFromFile()
void freeRGBArray(RGB_Color* rgbarray) {
	if (rgbarray != NULL) {
		free(rgbarray);
		rgbarray = NULL;
		printf("--> rgbarray freed\n");
	}
}

//Deprecated. Use 'getRGBArrayFromFile' instead.
//Reads gradient file and stores every line 
//(6 character color hex code ends with 'endl') in an array
//Must be freed! Call 'freeHexArray(char** hexarray)'
char** getHexArrayFromFile(char* filename) {
	//Converting hex to rgb in every mandelbrot iteration slows process.
	char** hexArray = malloc(MAX_COLOR_ARRAY_LENGTH * sizeof(char*));
	if (hexArray == NULL) {
		printf("Could not allocated memory for hexArray\n");
		return NULL;
	}
	char hexStr[8] = "";


	FILE* fileptr = fopen(filename, "r");
	if (fileptr == NULL) {
		printf("Could not open file: '%s'\n", filename);
		return NULL;
	}
	for (int i = 0; (i < MAX_COLOR_ARRAY_LENGTH) && (fgets(hexStr, 8, fileptr) != NULL); i++) {
		if (hexStr[6] != '\n' && strlen(hexStr) != 7) {
			printf("Could not create hexArray[%d]\n", i);
			printf("Gradient file corrupted. line: %d\n", i);
			printf("Every line must be 6 char long hex string and end with line terminator.\n");
			return NULL;
		}
		hexStr[6] = '\0';
		hexArray[i] = (char*)malloc(HEX_STR_LENGTH * sizeof(char));
		if (hexArray[i] == NULL) {
			printf("Could not allocate memory for hexArray[%d]\n", i);
			return NULL;
		}
		strncpy(hexArray[i], hexStr, HEX_STR_LENGTH * sizeof(char));
		printf("hexStr = %s\n", hexStr);
		printf("hexArray[%d] = %s\n", i, hexArray[i]);
		//strcpy(hexStr, "");
		CURRENT_HEX_ARRAY_LENGTH = i + 1;
	}
	fclose(fileptr);
	fileptr = NULL;
	return hexArray;
}

//Reads gradient file, converts every line (6 character color hex code ends with 'endl') 
//into 'RGB_Color' structure and stores them in an array
//Must be freed! Call 'freeRGBArray(RGB_Color* rgbarray)'
RGB_Color* getRGBArrayFromFile(char* filename) {
	RGB_Color* rgbArray = malloc(MAX_COLOR_ARRAY_LENGTH * sizeof(RGB_Color));
	if (rgbArray == NULL) {
		printf("Could not allocated memory for rgbArray\n");
		return NULL;
	}
	char hexStr[8] = "";
	RGB_Color temprgb = { .r = 0, .g = 0, .b = 0, .errorFlag = false };

	FILE* fileptr = fopen(filename, "r");
	if (fileptr == NULL) {
		printf("Could not open file: '%s'\n", filename);
		return NULL;
	}
	for (int i = 0; (i < MAX_COLOR_ARRAY_LENGTH) && (fgets(hexStr, 8, fileptr) != NULL); i++) {
		if (hexStr[6] != '\n' && strlen(hexStr) != 7) {
			printf("Could not create hexArray[%d]\n", i);
			printf("Gradient file corrupted. line: %d\n", i);
			printf("Every line must be 6 char long hex string and end with line terminator.\n");
			return NULL;
		}
		hexStr[6] = '\0';
		temprgb = getRGBfromHexStr(hexStr);
		if (temprgb.errorFlag == true) {
			printf("Could not convert hex string to rgb color. line: %d\n", i);
			return NULL;
		}
		rgbArray[i] = temprgb;
		
		printf("%s --> %d, %d, %d\n", hexStr, rgbArray[i].r, rgbArray[i].g, rgbArray[i].b);
		//strcpy(hexStr, "");
		CURRENT_HEX_ARRAY_LENGTH = i + 1;
	}
	fclose(fileptr);
	fileptr = NULL;
	return rgbArray;
}

//Maps a value between given limits. Example:
//0___4______10
//0_______X___________20
//X = getMappedValue(4, 0, 10, 0, 20)
//X -> 8
float getMappedValue(int _value, int _valuestart, int _valueend, int _mappedstart, int _mappedend) {
	float result = (_value - _valuestart) * (_mappedend - _mappedstart) / (float)(_valueend - _valuestart) + _mappedstart;
	return result;
}
