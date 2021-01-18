#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "colormap.h"

#define LENGHTOF(x)  (sizeof(x) / sizeof((x)[0]))



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

_hexString* _allocateHexArray(size_t arraysize) {
	//_hexString _tempstr = "";
	_hexString* _hexArray = (_hexString*)malloc(arraysize * sizeof(_hexString));
	if (_hexArray == NULL) {
		printf("Could not allocate memory for _hexArray\n");
		return NULL;
	}	
	return _hexArray;	
}

_hexString* _reallocateHexArray(_hexString* _hexarray, size_t arraysize) {
	//_hexString _tempstr = "";
	_hexString* _newhexarray = (_hexString*)realloc(_hexarray, arraysize * sizeof(_hexString));
	if (_newhexarray == NULL) {
		printf("Could not reallocate memory for _newhexarray\n");
		return NULL;
	}
	_hexarray = NULL;
	return _newhexarray;
}

void _freeHexArray(_hexString* _hexarray) {
	if (_hexarray != NULL) {
		free(_hexarray);
	}
}

_hexString* getHexArrayFromFile(char* filename) {
	//Must be freed!
	_hexString* hexArray;
	_hexString hexStr = "";
	size_t _arraysize = 1;

	hexArray = _allocateHexArray(_arraysize);
	
	if (hexArray == NULL) {
		printf("Could not initialize hexArray\n");
		return NULL;
	}

	FILE* fileptr = fopen(filename, "r");
	if (fileptr == NULL) {
		printf("Could not open file: '%s'\n", filename);
		return NULL;
	}
	for (int i = 0; fgets(hexStr, 6, fileptr) != NULL; i++) {
		hexStr[6] = '\0';
		if (i >= _arraysize) {
			hexArray = _reallocateHexArray(hexArray, i);
			_arraysize = i;
		}
		*(hexArray[i]) = hexStr;
		printf("hexStr = %s\n", hexStr);
	}
	return hexArray;
}

float getMappedValue(int _value, int _valuestart, int _valueend, int _mappedstart, int _mappedend) {
	float result = (_value - _valuestart) * (_mappedend - _mappedstart) / (float)(_valueend - _valuestart) + _mappedstart;
	return result;
}


//TODO gradient.gradient dosyasýndan rgb verileri çýkart