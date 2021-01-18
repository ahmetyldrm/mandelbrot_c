

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h> 
#include "colormap.h"
//#include "main.h"
//#include "mandelbrot.h"

//#define MAXITER 255

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define MAND_REAL_MIN -2.5
#define MAND_REAL_MAX  1.5

Uint32 MAND_MAX_ITER = 255;

double mandRealMin = MAND_REAL_MIN;
double mandRealMax = MAND_REAL_MAX;
double mandImagMin = -(double)SCREEN_HEIGHT / SCREEN_WIDTH * (MAND_REAL_MAX - MAND_REAL_MIN) / 2;
double mandImagMax = (double)SCREEN_HEIGHT / SCREEN_WIDTH * (MAND_REAL_MAX - MAND_REAL_MIN) / 2; 
//double mandImagMin = -1.265;
//double mandImagMax = 1.265;

SDL_Window*   sdlWindow   = NULL;
SDL_Renderer* sdlRenderer = NULL;
SDL_Texture*  sdlTexture  = NULL;

Uint32* sdlTexturePixels = NULL;
int sdlTexturePitch = 0; 
Uint32 sdlTextureFormat = 0;
int sdlTextureWidth  = 0;
int sdlTextureHeight = 0;

bool initSDL();
void closeSDL();
void updateTexturePixels();

int getMandelbrotIterCount(double real, double imag);
double getPrecision();
int getRealPointCount();
int getImagPointCount();
void zoomIn(int zoomfactor);
void zoomOut(int zoomfactor);
void slide(int x, int y);

//___________________________________
//Mandelbrot functions

//Retuns a value between 0 and MAND_MAX_ITER-1
int getMandelbrotIterCount(double real, double imag){
	double real0 = real;
	double imag0 = imag;
	Uint32 iter_count = 0;
	double real_temp = 0.0;

	while (iter_count < MAND_MAX_ITER) {
		if ((real * real) + (imag * imag) >= 4.0) {
			break;
		}
		iter_count++;
		real_temp = real;
		real = (real * real) - (imag * imag) + real0;
		imag = 2 * real_temp * imag + imag0;
	}
	return iter_count;
}
double getPrecision() {
	double screenScale = (double)SCREEN_WIDTH / SCREEN_HEIGHT;
	double mandelScale = (mandRealMax - mandRealMin) / (mandImagMax - mandImagMin);
	if (screenScale >= mandelScale) {
		return (mandImagMax - mandImagMin) / SCREEN_HEIGHT;
	}
	return (mandRealMax - mandRealMin) / SCREEN_WIDTH;
}
int getRealPointCount() {
	return (int)((mandRealMax - mandRealMin) / getPrecision());
}
int getImagPointCount() {
	return (int)((mandImagMax - mandImagMin) / getPrecision());
}
void zoomIn(int zoomfactor) {
	double precision = getPrecision();
	double mandelScale = (mandRealMax - mandRealMin) / (mandImagMax - mandImagMin);
	mandRealMin += zoomfactor * precision * mandelScale;
	mandRealMax -= zoomfactor * precision * mandelScale;
	mandImagMin += zoomfactor * precision;
	mandImagMax -= zoomfactor * precision;
}
void zoomOut(int zoomfactor) {
	double precision = getPrecision();
	double mandelScale = (mandRealMax - mandRealMin) / (mandImagMax - mandImagMin);
	mandRealMin -= zoomfactor * precision * mandelScale;
	mandRealMax += zoomfactor * precision * mandelScale;
	mandImagMin -= zoomfactor * precision;
	mandImagMax += zoomfactor * precision;
}
void slide(int x, int y) {
	double precision = getPrecision();
	mandRealMin += x * precision;
	mandRealMax += x * precision;
	mandImagMin += y * precision;
	mandImagMax += y * precision;
}

//___________________________________
//SDL functions

bool initSDL(){
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
			printf("Warning: Linear texture filtering not enabled!");
		}
		//Create window
		sdlWindow = SDL_CreateWindow("Mandelbrot Set", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (sdlWindow == NULL){
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else{
			//Create renderer for window
			sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (sdlRenderer == NULL){
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else{
				//Initialize renderer color
				SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 0xff);
				//Create blank streamable texture
				sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_GetWindowPixelFormat(sdlWindow), 
					SDL_TEXTUREACCESS_STREAMING, 
					getRealPointCount(), getImagPointCount());
				if (sdlTexture == NULL){
					printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
				}
				//else{
					//SDL_QueryTexture(sdlTexture, &sdlTextureFormat, NULL, &sdlTextureWidth, &sdlTextureHeight);
				//}
			}
		}
	}
	return success;
}

void closeSDL() {
	mandRealMin = 0;
	mandRealMax = 0;
	mandImagMax = 0;
	mandImagMin = 0;
	//Destroy texture
	if (sdlTexture != NULL) {
		SDL_DestroyTexture(sdlTexture);		
	}
	sdlTexture = NULL;
	sdlTexturePixels = NULL;
	sdlTextureFormat = 0;
	sdlTexturePitch = 0;
	sdlTextureWidth = 0;
	sdlTextureHeight = 0;
	//Destroy window and renderer	
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	sdlWindow = NULL;
	sdlRenderer = NULL;
}

void _printTextureSize() {
	printf("______________________________\n");
	printf("texture width = %d\n", sdlTextureWidth);
	printf("texture height = %d\n", sdlTextureHeight);
	printf("real point = %d\n", getRealPointCount());
	printf("imag point = %d\n", getImagPointCount());
	printf("______________________________\n");
}
void updateTexturePixels() {
	sdlTexturePixels = NULL;
	Uint8 colorValue[3] = { 0, 0, 0 };
	Uint32 iterCount = 0;

	//Get texture information
	SDL_QueryTexture(sdlTexture, &sdlTextureFormat, NULL, &sdlTextureWidth, &sdlTextureHeight);
	_printTextureSize();
	
	SDL_LockTexture(sdlTexture, NULL, &sdlTexturePixels, &sdlTexturePitch);

	SDL_PixelFormat* mappingFormat = SDL_AllocFormat(sdlTextureFormat);
	double mandPrecision = getPrecision();
	if (sdlTexturePixels) {
		for (Uint16 y = 0; y < sdlTextureHeight; y++) {
			for (Uint16 x = 0; x < sdlTextureWidth; x++) {
				//TODO Map colorValue				
				iterCount = getMandelbrotIterCount(mandRealMin + (x * mandPrecision), mandImagMax - (y * mandPrecision));
				if (iterCount == MAND_MAX_ITER) {
					colorValue[0] = 0;
					colorValue[1] = 0;
					colorValue[2] = 0;
				}
				else if (iterCount < MAND_MAX_ITER / 6) {
					/*colorValue[0] = iterCount * 255 / MAND_MAX_ITER;
					colorValue[1] = 255 - iterCount * 255 / MAND_MAX_ITER;
					colorValue[2] = iterCount * 255 / MAND_MAX_ITER;*/
					colorValue[0] = 0;
					colorValue[1] = 0;
					colorValue[2] = 0;
				}
				else if (iterCount < 2* MAND_MAX_ITER / 6) {
					colorValue[0] = 255 - iterCount * 255 / MAND_MAX_ITER;
					colorValue[1] = iterCount * 255 / MAND_MAX_ITER;
					colorValue[2] = iterCount * 255 / MAND_MAX_ITER;
					
				}
				else if (iterCount < 3 * MAND_MAX_ITER / 6) {
					colorValue[0] = iterCount * 255 / MAND_MAX_ITER;
					colorValue[1] = iterCount * 255 / MAND_MAX_ITER;
					colorValue[2] = 255 - iterCount * 255 / MAND_MAX_ITER;
				}
				else if (iterCount < 4 * MAND_MAX_ITER / 6) {
					colorValue[0] = iterCount * 255 / MAND_MAX_ITER;
					colorValue[1] = 255 - iterCount * 255 / MAND_MAX_ITER;
					colorValue[2] = 255 - iterCount * 255 / MAND_MAX_ITER;
				}
				else if (iterCount < 5 * MAND_MAX_ITER / 6) {
					colorValue[0] = 255 - iterCount * 255 / MAND_MAX_ITER;
					colorValue[1] = iterCount * 255 / MAND_MAX_ITER;
					colorValue[2] = 255 - iterCount * 255 / MAND_MAX_ITER;
				}
				else {
					colorValue[0] = 255 - iterCount * 255 / MAND_MAX_ITER;
					colorValue[1] = 255 - iterCount * 255 / MAND_MAX_ITER;
					colorValue[2] = iterCount * 255 / MAND_MAX_ITER;
				}
				//colorValue = colorValue * 255 / MAND_MAX_ITER;
				sdlTexturePixels[y * sdlTexturePitch / sizeof(int) + x] = SDL_MapRGB(mappingFormat, colorValue[0], colorValue[1], colorValue[2]);
			}
		}
	}
	mandPrecision = 0;
	iterCount = 0;
	*colorValue = NULL;
	SDL_UnlockTexture(sdlTexture);
	SDL_FreeFormat(mappingFormat);
}

void _printCoords() {
	printf("______________________________\n");
	printf("reel min = %.16lf\n", mandRealMin);
	printf("reel max = %.16lf\n", mandRealMax);
	printf("imag min = %.16lf\n", mandImagMin);
	printf("imag max = %.16lf\n", mandImagMax);
	printf("______________________________\n");
}

int main() 
{
	bool quit = false;
	//Start up SDL and create window
	if (!initSDL()){
		printf("Failed to initialize!\n");
	}
	else{
		updateTexturePixels();
		//Event handler
		SDL_Event event;
		
		//While application is running
		while (!quit){
			//Handle events on queue
			while (SDL_PollEvent(&event) != 0){
				//User requests quit
				if (event.type == SDL_QUIT){
					quit = true;
				}
				else if (event.type == SDL_KEYDOWN)
				{
					//Select surfaces based on key press
					switch (event.key.keysym.sym)
					{
					case SDLK_KP_PLUS:
						zoomIn(150);
						updateTexturePixels();
						printf("precision = %.16lf\n", getPrecision());
						break;

					case SDLK_KP_MINUS:
						zoomOut(150);
						updateTexturePixels();
						printf("precision = %.16lf\n", getPrecision());
						break;

					case SDLK_RIGHT:
						slide(25, 0);
						updateTexturePixels();
						_printCoords();
						break;

					case SDLK_LEFT:
						slide(-25, 0);
						updateTexturePixels();
						_printCoords();
						break;

					case SDLK_UP:
						slide(0, 25);
						updateTexturePixels();
						_printCoords();
						break;

					case SDLK_DOWN:
						slide(0, -25);
						updateTexturePixels();
						_printCoords();
						break;

					case SDLK_KP_0:
						if (MAND_MAX_ITER > 64) {
							MAND_MAX_ITER -= 64;
						}
						printf("max iterations = %u\n", MAND_MAX_ITER+1);
						break;
					
					case SDLK_KP_1:
						MAND_MAX_ITER += 64;
						printf("max iterations = %u\n", MAND_MAX_ITER+1);
						break;

					case SDLK_SPACE:
						updateTexturePixels();
						printf("updated\n");
						break;
					}
				}
			}
			//Clear screen
			SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(sdlRenderer);

			SDL_Rect centerQuad = {
				(SCREEN_WIDTH - sdlTextureWidth) / 2,
				(SCREEN_HEIGHT - sdlTextureHeight) / 2,
				sdlTextureWidth,
				sdlTextureHeight };

			SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &centerQuad);
			//SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);

			//Update screen
			SDL_RenderPresent(sdlRenderer);
		}
	}

	//Free resources and close SDL
	closeSDL();

	return 0;
}
