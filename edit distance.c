#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define M 1009
#define STRSIZE 40
#define STRSIZEPLUS 250

struct hash{				// Sozluk tablosunun struct yapisi.
	char word[STRSIZE];
}; 

struct wHash{				// Yanlis kelimelerin tutuldugu tablonun struct yapisi.
	char word[STRSIZE];
	char correctWord[STRSIZE];
};

struct editDistance{		// Secili kelimeye olan uzakliklari olculen kelimelerin tutuldugu linkli listenin struct yapisi.
	char word[STRSIZE];
	int distance;
	struct editDistance* next;
};

struct hash *dictionaryTable;
struct wHash *wrongWordTable;

void fileToHash(FILE*,struct hash*);					// Dosyadaki kelimeleri tek tek gezen ve hash tablosuna yazmak uzere insertIntoHash fonksiyonuna gonderen fonksiyon (2. Odev)
void insertIntoHash(char*, struct hash*);				// Gonderilen kelimeleri hash tablosuna yazan fonksiyon (2. Odev)
void insertIntoWHash(char*, char*, struct wHash*);		// Gonderilen kelimeleri wHash tablosuna yazan fonksiyon (hatali kelime tablosu)
int horner(char*);										// Horner fonksiyonu (2. Odev)
void checkSentence(char*);								// Cumleyi kelimelere bolup controlWord fonksiyonuna gonderen fonksiyon.
void controlWord(char*);								// Kelimeyle ilgili sorgulari yapip sozlukte olup olmadigini kontrol eden fonksiyon.
char* wrongWord(char*);									// Sozlukte bulunmayan kelimeyle ilgili sorgulari yapan fonksiyon.
int searchInWrongWord(char*);							// Kelimenin hatali kelime tablosunda olup olmadigini kontrol eden fonksiyon.
char* findCorrectWord(char*);							// Hatali kelime tablosunda bulunmayan kelimeye getEditDistance fonksiyonuna gonderip en uygun kelimeleri secen fonksiyon.
int getEditDistance(char*, char*);						// Gonderilen kelimeler arasindaki edit distance mesafesini bulup degeri donduren fonksiyon.
int min3(int, int, int);								// Gonderilen 3 sayi arasindaki en kucuk degeri donduren fonksiyon.
int minArray(int*, int);								// Gonderilen dizideki en kucuk elemani bulup donduren fonksiyon.

int main(){

	int i;
	char fileName[STRSIZE];			// Sozluk dosyasinin adini tutan degisken.
	char inputStr[STRSIZEPLUS];		// Girilen cumleyi tutan degisken.
	FILE *fp;
	
	dictionaryTable = (struct hash*) calloc(M, sizeof(struct hash));	// Sozluk tablosu icin bellekte yer ayirma.
	wrongWordTable = (struct wHash*) calloc(M, sizeof(struct wHash));	// Yanlis kelimelerin tablosu icin bellekte yer ayirma.
	
	if(dictionaryTable == NULL){
		printf("Bellek ayrilamadi.");
		return 1;
	}
	
	printf("Sozuk dosyasinin adini girin: ");
	
	fgets(fileName, STRSIZE, stdin);									// Sozluk dosyasinin adi kullanicidan alinir. (Ilerde cumle girdisi icin bosluklu string alinacagindan ve herhangi bir yerdeki scanf fgets'in calismasini engellediginden kodda scanf kullanilmadi.)
	if ((strlen(fileName) > 0) && (fileName[strlen (fileName) - 1] == '\n'))
        fileName[strlen (fileName) - 1] = '\0';
	
	fp = fopen(fileName, "r");											// Sozluk dosyasi okuma formatinda acilir.
	
	if(fp == NULL){
		fprintf(stderr, "Sozluk dosyasi acilamadi.");
		exit(1);
	}
	
	fileToHash(fp, dictionaryTable);									// Sozluk dosyasi dictionaryTable'a eklenir.
	fclose(fp);
	printf("%s dosyasi okunup sozluk hash tablosuna eklendi.\n", fileName);
	
	while(1){															// Sonsuz dongude kullanicidan cumleler alinip bunlara gore kullaniciya cikti verilir.
		
		printf("Sorgulanacak cumleyi ya da cikmak icin ! karakterini girin: ");			// Kullanicidan cumle alinir.

		fgets(inputStr, STRSIZEPLUS, stdin);
		if ((strlen(inputStr) > 0) && (inputStr[strlen (inputStr) - 1] == '\n'))
	        inputStr[strlen (inputStr) - 1] = '\0';
		
		if(strcmp(inputStr, "!") == 0){									// Bellek temizlenip cikis yapilir.
			
			free(dictionaryTable);
			free(wrongWordTable);
			
			printf("Hafiza temizlendi.");
			
			return 0;
		}
		
		checkSentence(inputStr);										// Cumle checkSentence fonksiyonuna gonderilir.
	}
	return 0;
}

void fileToHash(FILE *fp,struct hash* table){			// Onceki odevde bulunan ve dosyasindaki kelimeleri tek tek okuyup hash tablosuna ekleyen fonksiyon.
	
	int i, j = 0;
	char tmpC;
	char newWord[STRSIZE] = {0};
	
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	for(i=0; i<fileSize; i++){			
		tmpC = fgetc(fp);
		
		if(tmpC == ' ' || feof(fp)){
			
			insertIntoHash(newWord, table);
			memset(newWord, 0, j);
			j = 0;
			
		}else{
			newWord[j] = tmpC;
			j++;
		}
	}
}

void insertIntoHash(char *word, struct hash* table){					// Onceki odevde bulunan ve gonderilen kelimeyi hash tablosuna ekleyen fonksiyon.
	
	int i = 0;														
	int key = horner(word);											
	int emptyAddressFound = 0;										
	int address;
	
	while(emptyAddressFound == 0 && i < M){
		
		address = h(key, i);										
		
		if(strlen(table[address].word) == 0){					
			emptyAddressFound = 1;
		}else{				
			i++;
		}
	}
	
	if(emptyAddressFound == 1){
		strcpy(table[address].word, word);
	}else{
		printf("Hash tablosu dolu\n");
	}	
}

void insertIntoWHash(char *word,char *word2, struct wHash* table){ 		// Bir onceki fonksiyonun WHash (hatali kelime sozlugu) tablosu icin yazilmis hali.
	
	int i = 0;														
	int key = horner(word);										
	int emptyAddressFound = 0;									
	int address;
	
	while(emptyAddressFound == 0 && i < M){
		
		address = h(key, i);										
		
		if(strlen(table[address].word) == 0){				
			emptyAddressFound = 1;
		}else{				
			i++;
		}
	}
	
	if(emptyAddressFound == 1){
		strcpy(table[address].word, word);
		strcpy(table[address].correctWord, word2);
	}else{
		printf("WHash tablosu dolu\n");
	}	
}

int horner(char *word){													// Onceki odevde bulunan horner fonksiyonu.											
																		
	int i;								
	int key = 0;
	int r = 3;
	
	for(i=0; i<strlen(word); i++){
		char c = word[i];
		if(c>96)														
			c-=32;
		key += pow(r,(strlen(word) - i - 1)) * (c - 'A' + 1);
	}	
	return key;	
}

int h(int key, int i){													// Onceki odevde bulunan double hashing h fonksiyonu.
	return (h1(key) + i * h2(key)) % M;
}

int h1(int key){														// Onceki odevde bulunan double hashing h1 fonksiyonu.
	return key % M;
}

int h2(int key){														// Onceki odevde bulunan double hashing h2 fonksiyonu.
	return 1 + (key % (M - 1));
}

void checkSentence(char* sentence){										// Cumleyi kelime kelime sorgulayip her kelimeyi controlWord fonksiyonuna gonderen fonksiyon.
	
	int i, j = 0;
	int strSize = strlen(sentence);
	char tmpC;
	char newWord[STRSIZE] = {0};
	
	for(i=0; i<strSize; i++){											
		
		tmpC = sentence[i];
		if(tmpC == ' '){												// Bosluk karakteri geldiyse yeni kelime baslayacaktir bu yuzden o anki kelime controlWord fonksiyonuna gonderilir.
			
			controlWord(newWord);
			memset(newWord, 0, j);
			j = 0;
			
		}else{
			newWord[j] = tmpC;
			j++;
		}
	}

	controlWord(newWord);												// Dongu bittiginde son kelime gonderilmemis olacagindan o anki son kelime controlWord fonksiyonuna gonderilir.
}

void controlWord(char* word){											// Arguman aldigi kelimeyle ilgili sorgulari yapan fonksiyon.
	
	int i = 0;
	int fin = 0;
	int key = horner(word);
	
	while(i<M && fin == 0){
		
		int address = h(key, i);
		
		if(strlen(dictionaryTable[address].word) == 0){					// Bos bir adrese gelindiyse kelime sozlukte yoktur bu yuzden kelime wrongWord fonksiyonuna gonderilir. 
			fin = -1;
		}else if(strcmp(dictionaryTable[address].word, word) == 0){		// Kelime sozlukte bulunduysa hicbir islem yapilmadan devam edilir.
			fin = 1;
		}else{
			i++;
		}
	}
	
	if(fin != 1){	
		
		printf("%s --> %s\n",word, wrongWord(word));					// Kelimenin dogrusunun ne olacagina wrongWord fonksiyonunda karar verilir ve bu kelime wrongWord fonksiyonu ile dondurulur.
	}else{
		printf("%s --> %s\n",word,word);								// Kelime sozlukte varsa aynen yazdirilir.
	}
	
	
	
}

char* wrongWord(char* word){											// Sozlukte bulunmayan kelime ile ilgili sorgularin yapildigi fonksiyon.
	
	int i = searchInWrongWord(word);									// Kelime hatali kelime sozlugunde aranmak uzere searchInWrongWord fonksiyonuna gonderilir.
	
	if(i != -1){														// Kelime hatali kelime sozlugunde bulunduysa searchInWrongWord fonksiyonu kelimenin tablodaki yerini doneceginden bu fonksiyonda tablodaki bu yerdeki kelimenin dogrusunu doner.
		return wrongWordTable[i].correctWord;
		
	}else{																// Kelime hatali kelime sozlugunde bulunamadiysa editDistance uzakligi sorgulanmak uzere findCorrectWord fonksiyonuna gonderilir.
		return findCorrectWord(word);
	}
}

int searchInWrongWord(char* word){										// Gonderilen kelimeyi hatali kelime tablosunda arayan, bulursa tablodaki indisini bulamazsa -1 degerini donduren fonksiyon.
	
	int i=0;
	int key = horner(word);
	
	while(i<M){
		int address = h(key, i);
		if(strlen(wrongWordTable[address].word) == 0){					// Gecerli indis bos ise kelime hatali kelime tablosunda olmayacagindan -1 dondurulur.
			return -1;
		}else if(strcmp(wrongWordTable[address].word, word) == 0){		// Kelimeler eslesiyorsa gecerli adres degeri dondurulur.
			return address;
		}
		i++;
	}
	return -1;
}

char* findCorrectWord(char* word){										// Gonderilen kelimeyle sozlukteki kelimeler arasindaki edit distance degerini bulan ve buna gore en uygun kelimeyi kullaniciya sectirten ve bu kelimeyi donduren fonksiyon.
	
	int i, j=0;
	struct editDistance* head;											// Uygun olabilecek kelimelerin saklandigi linkli listenin ilk elemanina isaret eden pointer.
	head = NULL;
	struct editDistance* tmp;											// Islemlerde kullanilacak gecici pointer.
	
	for(i=0; i<M; i++){
		int editDistance = getEditDistance(word, dictionaryTable[i].word);	// Kelimeler arasindaki mesafe bulunur.
		if(editDistance <= 2 && strlen(dictionaryTable[i].word) != 0){		// Bulunan mesafe 2'ye esit veya 2'den kucukse isleme sokulur.
			
			if(head == NULL){												// Eklenen her kelime head ile isaret edilip onceki head isareti yeni head'in next isaretine atanir.
				head = malloc(sizeof(struct editDistance));
				head->distance = editDistance;
				head->next = NULL;
				strcpy(head->word, dictionaryTable[i].word);
			}else{
				tmp = malloc(sizeof(struct editDistance));
				tmp->distance = editDistance;
				strcpy(tmp->word, dictionaryTable[i].word);
				tmp->next = head;
				head = tmp;
			}
		}
	}
	
	if(head == NULL){														// Islemler sonucunda head bos ise uygun kelime bulunamadigindan '?' karakteri donulur.
		printf("%s icin uygun kelime bulunamadi.\n",word);
		return "?";
	}
	
	i = 0;
	int oneAvailable = 0;
	tmp = head;
	while(tmp != NULL && oneAvailable == 0){								// Mesafesi 1 olan en az bir kelime bulunduysa mesafesi 2 olanlari gostermeye gerek olmayacagindan bunun kontrolu yapilir.
		if(tmp->distance == 1){
			oneAvailable = 1;
		}
		tmp=tmp->next;
	}
	printf("%s Kelimesi icin ", word);
	if(oneAvailable == 1){													// Mesafesi 1 olan kelime bulunduysa sadece mesafesi 1 olan kelimeler ekrana yazdirilir.
		printf("edit distance 1 olup konabilecek kelimeler:\n");
		for(tmp=head; tmp!=NULL; tmp=tmp->next){
			if(tmp->distance == 1){
				printf("%s\t", tmp->word);
			}
		}
	}else{
		printf("edit distance 2 olup konabilecek kelimeler:\n");			// Mesafesi 1 olan kelime yoksa butun kelimelerin mesafesi 2 olacagindan hepsi ekrana yazdirilir.
		for(tmp=head; tmp!=NULL; tmp=tmp->next){
			printf("%s\t", tmp->word);
		}
	}
	
	int newWordSet = 0;
	
	while(newWordSet == 0){													// Kullanici onerilen kelimelerden birini dogru olarak girene kadar kullanicidan tekrar girdi beklenir.
		
		char newWord[STRSIZE] = {0};
		
		printf("\nSecilen kelimeyi yaziniz: ");
	
		fgets(newWord, STRSIZE, stdin);
		if ((strlen(newWord) > 0) && (newWord[strlen (newWord) - 1] == '\n'))
	        newWord[strlen (newWord) - 1] = '\0';
		
		tmp = head;
		while(tmp != NULL && newWordSet == 0){								// Kullanicinin girdigi kelime linkli listedeki kelimelerle karsilastirilip uygun kelime girilip giriledigi kontrol edilir.
			if(strcmp(tmp->word, newWord) == 0){
				newWordSet = 1;
			}else{
				tmp=tmp->next;	
			}
		}
		if(newWordSet == 0){
			printf("Lutfen gecerli bir kelime giriniz.");
		}
	}
	
	
	insertIntoWHash(word,tmp->word,wrongWordTable);							// Kullanicinin sectigi yeni kelime ve kullanicinin hatali yazdigi kelime wHash tablosuna eklenir.
	return tmp->word;														// Yeni kelime donulur.	
}

int getEditDistance(char* word, char* word2){								// Levenshtein Edit Distance yontemi ile kelimeler arasindaki farki bulan fonksiyon.
	
	int i,j;
	int row = strlen(word) + 1;
	int column = strlen(word2) + 1;
	int matrix[row][column];												// Islemlerin gerceklestirilecegi matrisin boyutu ayarlanir.
	
	for(i=0;i<row;i++){														// Matrisin 0. sutununun baslangic degerleri atanir.
		matrix[i][0] = i;
	}
	for(j=0;j<column; j++){													// Matrisin 0. satirinin baslangic degerleri atanir.
		matrix[0][j] = j;
	}
	
	i = 1;
	
	while(i<row){															
		j = 1;
		while(j<column){
			
			if(word[i-1] == word2[j-1]){														// Harfler birbirine esitse matrisin gecerli gozune sol ust caprazindaki deger yazilir.
				matrix[i][j] = matrix[i-1][j-1];
			}else{
				matrix[i][j] = min3(matrix[i][j-1], matrix[i-1][j], matrix[i-1][j-1]) + 1;		// Harfler esit degilse matrisin gecerli gozune sol, ust ya da sol ust capraz degerlerinden en dusuk olaninin bir fazlasi yazilir.
			}
			j++;
		}
		if(minArray(matrix[i], column) > 2){													// [BONUS] Gecerli satirdaki degerlerin en kucugu 2'den buyukse bir alt satira da gidebilecek en dusuk deger 2'den buyuk olacagindan diger satirlari kontrol etmeye gerek kalmadan edit distance degerinin 2'den buyuk olacagi bulunur ve 2'den buyuk bir deger donulur.                               
			return 99;
		}
		i++;
	}
	return matrix[row - 1][column - 1];															// Matris islemeleri tamamlandiysa matrisin sag alt kosesindeki deger edit distance degeri olacagindan bu deger donulur.
}

int min3(int a, int b, int c){																	// Gonderilen 3 sayi arasindaki en dusuk degeri donduren fonksiyon.
	
	if(a <= b && a <= c){
		return a;
	}else if(b <= c){
		return b;
	}else{
		return c;
	}
}

int minArray(int* array, int column){															// Gonderilen dizi icerisindeki en dusuk degeri donduren fonksiyon.
	
	int i;
	int min = 99;
	for(i=0; i<column; i++){
		if(array[i] < min){
			min = array[i];
		}
	}
	return min;
}
