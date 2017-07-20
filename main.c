#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define MAX_KAYIT_SAYISI 381

struct Ogrenci{
    char isim[11];
    int numara;
};

int kayit_sayisi_bul(struct Ogrenci ogrenciler[100]);
void bolen_kalan_hash_ekle(struct Ogrenci ogrenciler[100]);
void bolen_kalan_hash_ara(int ogrenciler_hash[]);
void kare_ortasi_hash_ara();
void kare_ortasi_hash_ekle(struct Ogrenci ogrenciler[]);
void lineer_arama();
int basamak_sayisi_bul(uint64_t sayi);
float getdiff(struct timeval endtv, struct timeval starttv);

int ogrenciler_hash_bolen_kalan[MAX_KAYIT_SAYISI];
int ogrenciler_hash_kare[10000];


int main() {

    srand(time(NULL));

    FILE *fp_names;
    FILE *fp_numbers;

    fp_names = fopen("/Users/omrumbakitemiz/CLionProjects/ProLab2.1/names.txt", "r");
    fp_numbers = fopen("/Users/omrumbakitemiz/CLionProjects/ProLab2.1/numbers.txt", "r");

    struct Ogrenci ogrenciler[381];

    struct timeval starttv, endtv;
    struct timezone starttz, endtz;

    int kayit_sayisi = 0;

    //printf("Lüften oluşturulacak kayıt sayısını giriniz... ");
    //scanf("%d", &kayit_sayisi);

    for (int i = 0; i < 381 ; i++) {
        fscanf(fp_names, "%s", &ogrenciler[i].isim);
        fscanf(fp_numbers, "%d", &ogrenciler[i].numara);
    }

    bolen_kalan_hash_ekle(ogrenciler);
    bolen_kalan_hash_ara(ogrenciler_hash_bolen_kalan);
    kare_ortasi_hash_ekle(ogrenciler);
    kare_ortasi_hash_ara();
    lineer_arama();

    return 0;
}

void kare_ortasi_hash_ekle(struct Ogrenci ogrenciler[]){

    int ogrenciler_hash[10000];
    int kayit_sayisi = kayit_sayisi_bul(ogrenciler);
    int64_t hash;
    int basamaklar[18];
    int deger; // kare alındıktan sonra ortadan alınan 4 rakamlı sayı

    // Tüm kayıtlar boş hale getiriliyor...
    for (int j = 0; j < 10000 ; j++) {
        ogrenciler_hash[j] = -1;
    }

    for(int i=0; i < kayit_sayisi; i++){

        hash = (int64_t ) pow(ogrenciler[i].numara, 2);
        for (int j = 0; hash > 0; j++) {
            basamaklar[j] = hash % 10;
            hash = hash /10;
        }
        deger = basamaklar[7]*1000 + basamaklar[8]*100 + basamaklar[9]*10 + basamaklar[10];

        while(ogrenciler_hash[deger] != -1) {
            deger++;
            if(deger == MAX_KAYIT_SAYISI){
                deger = 0;
            }
        }

        ogrenciler_hash[deger] = ogrenciler[i].numara;
    }

    FILE *hash_yaz;
    hash_yaz = fopen("/Users/omrumbakitemiz/CLionProjects/ProLab2.1/hash_kare.txt", "w+");

    for (int i = 0; i < 10000; i++) {
        if(ogrenciler_hash[i] == -1){
            fputs("-1\n", hash_yaz);
            ogrenciler_hash_kare[i] = -1;
        } else{
            fprintf(hash_yaz, "%d", ogrenciler_hash[i]);
            fputs("\n", hash_yaz);
            ogrenciler_hash_kare[i] = ogrenciler_hash[i];
        }
    }
}

void kare_ortasi_hash_ara(){

    struct timeval starttv, endtv;
    struct timezone starttz, endtz;
    int sonuc = 0;

    int aranan_deger;
    int ogrenciler[10000];

    for (int i = 0; i < 381; i++) {
        ogrenciler[i] = -1;
    }

    FILE *fp_kare;
    fp_kare = fopen("/Users/omrumbakitemiz/CLionProjects/ProLab2.1/hash_kare.txt", "r");

    printf("aranacak değeri giriniz: ");
    scanf("%d", &aranan_deger);

    gettimeofday(&starttv, &starttz);

    for (int i = 0; i < 10000; i++) {
        fscanf(fp_kare, "%d", &ogrenciler[i]);

        if(ogrenciler[i] == aranan_deger){
            printf("aradığınız değer %d hash değeriyle bulundu\n", i);
            sonuc = 1;
            break;
        }
        sonuc = 0;
    }

    gettimeofday(&endtv, &endtz);

    if(sonuc == 0){
        printf("Aradığınız değer bulunamadı...");
    }
    else{
        float fark = getdiff(endtv, starttv);
        printf("Kare Ortası Arama Tekniği Hız %f\n\n", fark);
    }
}

float getdiff(struct timeval endtv, struct timeval starttv) {
    float diff=0;
    diff=(endtv.tv_sec-starttv.tv_sec) * 1000000 + (endtv.tv_usec-starttv.tv_usec);
    return diff/1000000;
}

void bolen_kalan_hash_ekle(struct Ogrenci ogrenciler[]){

    int ogrenciler_hash[MAX_KAYIT_SAYISI];
    int kayit_sayisi = kayit_sayisi_bul(ogrenciler);
    int hash = 0;

    // Tüm kayıtlar boş hale getiriliyor...
    for (int j = 0; j < MAX_KAYIT_SAYISI ; j++) {
        ogrenciler_hash[j] = -1;
    }

    for(int i=0; i < 381; i++){

        hash = ogrenciler[i].numara % MAX_KAYIT_SAYISI;

        while(ogrenciler_hash[hash] != -1) {
            hash++;
            if(hash == MAX_KAYIT_SAYISI){
                hash = 0;
            }
        }
        ogrenciler_hash[hash] = ogrenciler[i].numara;
    }

    FILE *hash_yaz;
    hash_yaz = fopen("/Users/omrumbakitemiz/CLionProjects/ProLab2.1/hash_bolen.txt", "w+");

    for (int i = 0; i < 381; i++) {
        if(ogrenciler_hash[i] == -1){
            fputs("-1\n", hash_yaz);
            ogrenciler_hash_bolen_kalan[i] = -1;
        } else{
            fprintf(hash_yaz, "%d", ogrenciler_hash[i]);
            fputs("\n", hash_yaz);
            ogrenciler_hash_bolen_kalan[i] = ogrenciler_hash[i];
        }
    }
}

void bolen_kalan_hash_ara(int ogrenciler_hash[]){

    struct timeval starttv, endtv;
    struct timezone starttz, endtz;

    int aranan_deger, hash = 0, numara;
    int ogrenciler[381];

    printf("aramak istenen değeri girin: ");
    scanf("%d", &aranan_deger);

    hash = aranan_deger % MAX_KAYIT_SAYISI;

    FILE *fp;
    fp = fopen("/Users/omrumbakitemiz/CLionProjects/ProLab2.1/hash_bolen.txt", "r");

    gettimeofday(&starttv, &starttz);

    for (int i = 0; i < MAX_KAYIT_SAYISI; i++) {
        fscanf(fp, "%d", &ogrenciler[i]);

        if(ogrenciler[i] == aranan_deger){
            printf("aradığınız değer %d hash değeriyle bulundu\n", i);
            break;
        }
    }

    gettimeofday(&endtv, &endtz);

    float fark = getdiff(endtv, starttv);

    printf("Bölen Kalan Tekniği Hızı : %f\n\n", fark);
}

void lineer_arama(){

    struct timeval starttv, endtv;
    struct timezone starttz, endtz;

    int aranan_deger, index = 0;
    int ogrenciler[381]; //TODO: dinamikleştirme

    FILE *fp_lineer;
    fp_lineer = fopen("/Users/omrumbakitemiz/CLionProjects/ProLab2.1/numbers.txt", "r");

    printf("aranacak değeri giriniz: ");
    scanf("%d", &aranan_deger);

    gettimeofday(&starttv, &starttz);

    for (int i = 0; i < 381; i++) { // TODO: döngü değişken dinamik yapılacak
        fscanf(fp_lineer, "%d", &ogrenciler[i]);

        if(ogrenciler[i] == aranan_deger){
            printf("aradığınız değer %d. indexte bulundu\n", i);
            break;
        }
    }

    gettimeofday(&endtv, &endtz);

    float fark = getdiff(endtv, starttv);
    printf("Lineer Arama Tekniği Hız %f\n\n", fark);
}

int basamak_sayisi_bul(uint64_t sayi){

    int basamak_sayisi = 0;

    while(sayi>0)
    {
        basamak_sayisi++;
        sayi/=10;
    }

    return basamak_sayisi;
}

int kayit_sayisi_bul(struct Ogrenci ogrenciler[]){

    int kayit_sayisi=0;

    while(ogrenciler[kayit_sayisi].isim[0] != '\0'){
        kayit_sayisi++;
    }

    return kayit_sayisi;
}