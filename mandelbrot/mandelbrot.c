// mandelbrot.cpp : Bu dosya 'main' işlevi içeriyor. Program yürütme orada başlayıp biter.
//

# include <stdio.h>
# define MAXITER 128

int mandelbrot(float real, float imag)
{
	float real0 = real;
	float imag0 = imag;
	int iter_count = 0;

	while (iter_count < MAXITER) {
		if ((real * real) + (imag * imag) >= 4.0f) {
			break;
		}
		iter_count++;
		real = (real * real) - (imag * imag) + real0;
		imag = 2 * real * imag + imag0;
	}
	return iter_count;
}

int main()
{
	float list[1000];
	int index = 0;
	for (int i = -200; i <= 100; i ++) {
		float fl = i / 100.0f;
		list[index] = fl;
		index++;
		printf("%i\n", mandelbrot(fl, -0.2f));
	}
	printf("Liste boyutu '%zu' byte\n", sizeof(list));
}

// Programı çalıştır: Ctrl + F5 veya Hata Ayıkla > Hata Ayıklamadan Başlat menüsü
// Programda hata ayıkla: F5 veya Hata Ayıkla > Hata Ayıklamayı Başlat menüsü

// Kullanmaya Başlama İpuçları: 
//   1. Dosyaları eklemek/yönetmek için Çözüm Gezgini penceresini kullanın
//   2. Kaynak denetimine bağlanmak için Takım Gezgini penceresini kullanın
//   3. Derleme çıktısını ve diğer iletileri görmek için Çıktı penceresini kullanın
//   4. Hataları görüntülemek için Hata Listesi penceresini kullanın
//   5. Yeni kod dosyaları oluşturmak için Projeye Git > Yeni Öğe ekle veya varolan kod dosyalarını projeye eklemek için Proje > Var Olan Öğeyi Ekle adımlarını izleyin
//   6. Bu projeyi daha sonra yeniden açmak için Dosya > Aç > Proje'ye gidip .sln uzantılı dosyayı seçin
