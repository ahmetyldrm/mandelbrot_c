﻿

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h> 
//#include "main.h"
//#include "mandelbrot.h"

//#define MAXITER 255

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700

Uint32 MAND_MAX_ITER = 255;

double mandRealMin = -2.2;
double mandRealMax = 1.0;
double mandImagMin = -1.2;
double mandImagMax = 1.2;

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

int getMandelbrotIterCount(double real, double imag)
{
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
		sdlTexture = NULL;
	}
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
void updateTexturePixels() {
	sdlTexturePixels = NULL;
	int realPointCount = getRealPointCount();
	int imagPointCount = getImagPointCount();

	SDL_QueryTexture(sdlTexture, &sdlTextureFormat, NULL, &sdlTextureWidth, &sdlTextureHeight);
	
	SDL_LockTexture(sdlTexture, NULL, (void**) &sdlTexturePixels, &sdlTexturePitch);
	//SDL_PixelFormat* mappingFormat = SDL_AllocFormat(SDL_GetWindowPixelFormat(sdlWindow));
	SDL_PixelFormat* mappingFormat = SDL_AllocFormat(sdlTextureFormat);
	double mandPrecision = getPrecision();
	if (sdlTexturePixels) {
		for (int y = 0; y < imagPointCount; y++) {
			for (int x = 0; x < realPointCount; x++) {
				// todo Map colorValue
				Uint32 colorValue = getMandelbrotIterCount(mandRealMin + (x * mandPrecision), mandImagMax - (y * mandPrecision));
				if (colorValue == MAND_MAX_ITER) {
					colorValue = 0;
				}
				else {
					colorValue = colorValue * 255 / MAND_MAX_ITER;
				}
				sdlTexturePixels[y * sdlTexturePitch / sizeof(int) + x] = SDL_MapRGB(mappingFormat, colorValue, colorValue, colorValue);
			}
		}
	}
	realPointCount = 0;
	imagPointCount = 0;
	mandPrecision = 0;
	SDL_UnlockTexture(sdlTexture);
	SDL_FreeFormat(mappingFormat);
}

void printCoords() {
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
						printCoords();
						break;

					case SDLK_LEFT:
						slide(-25, 0);
						updateTexturePixels();
						printCoords();
						break;

					case SDLK_UP:
						slide(0, 25);
						updateTexturePixels();
						printCoords();
						break;

					case SDLK_DOWN:
						slide(0, -25);
						updateTexturePixels();
						printCoords();
						break;

					case SDLK_KP_0:
						if (MAND_MAX_ITER > 64) {
							MAND_MAX_ITER -= 64;
						}
						printf("max iteration = %u\n", MAND_MAX_ITER);
						break;
					
					case SDLK_KP_1:
						MAND_MAX_ITER += 64;
						printf("max iteration = %u\n", MAND_MAX_ITER);
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

			//Render stick figure
			//sdlTexture.render((SCREEN_WIDTH - sdlTexture.getWidth()) / 2, (SCREEN_HEIGHT - sdlTexture.getHeight()) / 2);
			/*SDL_Rect centerQuad = { 
				(SCREEN_WIDTH - getRealPointCount()) / 2, 
				(SCREEN_HEIGHT - getImagPointCount()) / 2,  
				getRealPointCount(), 
				getImagPointCount() };*/
			SDL_Rect centerQuad = {
				(SCREEN_WIDTH - sdlTextureWidth) / 2,
				(SCREEN_HEIGHT - sdlTextureHeight) / 2,
				sdlTextureWidth,
				sdlTextureHeight };
			//SDL_Rect centerQuad = { 0,0,sdlTextureWidth,sdlTextureHeight };
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


//int main()
//{
//
//	SDL_Window* window = NULL;
//	SDL_Surface* screenSurface = NULL;
//
//	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//	}
//	else {
//		window = SDL_CreateWindow("Mandelbrot Set",
//			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//			SCREEN_WIDTH, SCREEN_HEIGHT,
//			SDL_WINDOW_SHOWN);
//		if (window == NULL) {
//			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
//		}
//		else {
//			screenSurface = SDL_GetWindowSurface(window);
//			//SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
//			Uint32* pixels = (Uint32*)screenSurface->pixels;
//			//pixels[640 * 10 + 10] = SDL_MapRGB(screenSurface->format, 255, 0, 255);
//
//			double realBorders[2] = { -2.0, 1.0 };
//			double imagBorders[2] = { -1.5, 1.5 };
//
//			for (int y = 0; y < SCREEN_HEIGHT; y++) {
//				for (int x = 0; x < SCREEN_WIDTH; x++) {
//					Uint8 colorValue = getMandelbrotIterCount(x / (SCREEN_WIDTH  / (realBorders[1] - realBorders[0])) + realBorders[0],
//												y / (SCREEN_HEIGHT / (imagBorders[1] - imagBorders[0])) + imagBorders[0]);
//					/*Uint8 colorValue = getMandelbrotIterCount(_LCbuild(x / (SCREEN_WIDTH / (realBorders[1] - realBorders[0])) + realBorders[0],
//																	y / (SCREEN_HEIGHT / (imagBorders[1] - imagBorders[0])) + imagBorders[0]));*/
//					pixels[y * SCREEN_WIDTH + x] = SDL_MapRGB(screenSurface->format, colorValue, colorValue, colorValue);
//				}
//			}
//
//			SDL_Event event;
//			bool running = true;
//			while (running) {
//				while (SDL_PollEvent(&event) != 0)
//				{
//					//User requests quit
//					if (event.type == SDL_QUIT)
//					{
//						running = false;
//					}
//				}
//				SDL_UpdateWindowSurface(window);
//				SDL_Delay(50);
//			}
//			
//			SDL_DestroyWindow(window);
//			pixels = NULL;
//			SDL_Quit();
//
//			return 0;
//		}
//	}
//}
//int main(int argc, char* args[])
//{
//	//Start up SDL and create window
//	if (!init())
//	{
//		printf("Failed to initialize!\n");
//	}
//	else
//	{
//		//Load media
//		if (!loadMedia())
//		{
//			printf("Failed to load media!\n");
//		}
//		else
//		{
//			//Main loop flag
//			bool quit = false;
//
//			//Event handler
//			SDL_Event e;
//
//			//While application is running
//			while (!quit)
//			{
//				//Handle events on queue
//				while (SDL_PollEvent(&e) != 0)
//				{
//					//User requests quit
//					if (e.type == SDL_QUIT)
//					{
//						quit = true;
//					}
//				}
//
//				//Clear screen
//				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
//				SDL_RenderClear(gRenderer);
//
//				//Copy frame from buffer
//				gStreamingTexture.lockTexture();
//				gStreamingTexture.copyPixels(gDataStream.getBuffer());
//				gStreamingTexture.unlockTexture();
//
//				//Render frame
//				gStreamingTexture.render((SCREEN_WIDTH - gStreamingTexture.getWidth()) / 2, (SCREEN_HEIGHT - gStreamingTexture.getHeight()) / 2);
//
//				//Update screen
//				SDL_RenderPresent(gRenderer);
//			}
//		}
//	}
//
//	//Free resources and close SDL
//	close();
//
//	return 0;
//}
// Programı çalıştır: Ctrl + F5 veya Hata Ayıkla > Hata Ayıklamadan Başlat menüsü
// Programda hata ayıkla: F5 veya Hata Ayıkla > Hata Ayıklamayı Başlat menüsü

// Kullanmaya Başlama İpuçları: 
//   1. Dosyaları eklemek/yönetmek için Çözüm Gezgini penceresini kullanın
//   2. Kaynak denetimine bağlanmak için Takım Gezgini penceresini kullanın
//   3. Derleme çıktısını ve diğer iletileri görmek için Çıktı penceresini kullanın
//   4. Hataları görüntülemek için Hata Listesi penceresini kullanın
//   5. Yeni kod dosyaları oluşturmak için Projeye Git > Yeni Öğe ekle veya varolan kod dosyalarını projeye eklemek için Proje > Var Olan Öğeyi Ekle adımlarını izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantılı dosyayı seçin
