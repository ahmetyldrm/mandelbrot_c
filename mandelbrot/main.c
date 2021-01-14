// mandelbrot.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

#include <SDL.h> 
#include <stdio.h>
#include <stdbool.h>
#include "mandelbrot.h"

//#define MAXITER 255



int main2()
{
	float list[1000];
	int index = 0;
	for (int i = -200; i <= 100; i ++) {
		float fl = i / 100.0f;
		list[index] = fl;
		index++;
		printf("%i\n", mandelbrot(fl, 0.0f));
	}
	printf("Liste boyutu '%zu' byte\n", sizeof(list));
	return 0;
}


int main()
{
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 1000;

	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		window = SDL_CreateWindow("Mandelbrot Set",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			screenSurface = SDL_GetWindowSurface(window);
			//SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));
			Uint32* pixels = (Uint32*)screenSurface->pixels;
			//pixels[640 * 10 + 10] = SDL_MapRGB(screenSurface->format, 255, 0, 255);

			double realBorders[2] = { -2.0, 1.0 };
			double imagBorders[2] = { -1.5, 1.5 };

			for (int y = 0; y < SCREEN_HEIGHT; y++) {
				for (int x = 0; x < SCREEN_WIDTH; x++) {
					Uint8 colorValue = mandelbrot(x / (SCREEN_WIDTH  / (realBorders[1] - realBorders[0])) + realBorders[0], 
												y / (SCREEN_HEIGHT / (imagBorders[1] - imagBorders[0])) + imagBorders[0]);
					/*Uint8 colorValue = getMandelbrotIterCount(_LCbuild(x / (SCREEN_WIDTH / (realBorders[1] - realBorders[0])) + realBorders[0],
																	y / (SCREEN_HEIGHT / (imagBorders[1] - imagBorders[0])) + imagBorders[0]));*/
					pixels[y * SCREEN_WIDTH + x] = SDL_MapRGB(screenSurface->format, colorValue, colorValue, colorValue);
				}
			}

			SDL_Event event;
			bool running = true;
			while (running) {
				while (SDL_PollEvent(&event) != 0)
				{
					//User requests quit
					if (event.type == SDL_QUIT)
					{
						running = false;
					}
				}
				SDL_UpdateWindowSurface(window);
				SDL_Delay(50);
			}
			
			SDL_DestroyWindow(window);
			pixels = NULL;
			SDL_Quit();

			return 0;
		}
	}
}
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
