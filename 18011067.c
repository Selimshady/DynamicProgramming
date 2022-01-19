#include <stdio.h>
#include <stdlib.h>
#define SIZE 100

struct ADS{
    int no; // Reklam numarası
    int startTime; // Baslangıc zamani
    int duration; // Uzunluk
    int value; // Degeri
};

struct INFO{
    int profit; // O anki adımdaki toplam kazancı saklar.
    int prevAd; // O andan önceki son olabilir reklami gösterir.
};

int readFileIntoStruct(struct ADS** ads) {
    FILE *fp; // To keep file pointer
    struct ADS tmp[SIZE];//Temporary array to get length and data
    int size = 0; // Size of ads
    int i; 
    
    char* line = (char*)malloc(sizeof(char)*10);//To get the line and values//Allocating memory
    struct ADS *tmpStruct; // Temporary struct
 
    if((fp = fopen("Sample.txt","r")) == NULL) {//Opening the file to read
        printf("FILE ACCSESS ERROR!");
        exit(-1);//If there is a problem, exit the program
    } 
    
    while(fgets(line,10,fp) != NULL) {//Read the line
        tmp[size].no = size+1;
        sscanf(line, "%d %d %d" ,&tmp[size].startTime,&tmp[size].duration,&tmp[size].value); // Parsing three values to struct
        size++; //Counts the lines
    }
    
    tmpStruct = (struct ADS*)malloc(sizeof(struct ADS)*size); // Allocating the real struct
    

    for(i=0;i<size;i++) {
        tmpStruct[i] = tmp[i];
        tmpStruct[i] = tmp[i];
    }
    *ads = tmpStruct; // To pass address to main struct
    
    
    fclose(fp); //close the file
    free(line); // free the line address
    return size; //Return the length
}

// function to swap elements
void swap(struct ADS *a, struct ADS *b) {
  struct ADS t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
int partition(struct ADS array[], int low, int high) {
  
  // select the rightmost element as pivot
  int pivot = array[high].startTime + array[high].duration;
  int j;
  // pointer for greater element
  int i = (low - 1);

  
  // compare them with the pivot
  for (j = low; j < high; j++) {
    if (array[j].startTime + array[j].duration <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(struct ADS array[], int low, int high) {
  if (low < high) {
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition(array, low, high);
    
    // recursive call on the left of pivot
    quickSort(array, low, pi - 1);
    
    // recursive call on the right of pivot
    quickSort(array, pi + 1, high);
  }
}

void printTable(struct ADS* ads,int N) {
    int i;
    printf("\n\n");
    printf("%8s | %8s | %8s | %8s | \n","j","starTime","Duration","Value");
    printf("-------------------------------------------");
    for(i=0;i<N;i++) {
        printf("\n%8d | %8d | %8d | %8d | \n",ads[i].no,ads[i].startTime,ads[i].duration,ads[i].value); // Sırali reklamlar ekrana basılır.
        printf("-------------------------------------------");
    }
    printf("\n\n");
}

int main(int argc, char **argv)
{
    int N,i,j,temp;
    struct ADS* ads;// Reklam dizisini saklayacak. // info dizisiyle arasında 1 indis oynama olacak.***
    struct INFO* info;// info hem kendinden önceki son kesişmeyen reklamı gösterecek hem o anki karı gösterecek.
    
    
    N = readFileIntoStruct(&ads);//Get the count of ads
    
    info = (struct INFO*)malloc((N+1)*sizeof(struct INFO));
    
    quickSort(ads,0,N-1); // Reklamlar bitiş süresine göre sıralandirilir.
    
    printTable(ads,N);
    
    info[0].profit = 0; // Hiç reklam yoksa kar.
    info[0].prevAd = -1; // Hiç bir reklami göstermez.
    
    info[1].profit = info[0].profit+ ads[0].value; // info[i].profit ads[i-1]'i  denk geliyor.
    info[1].prevAd = 0; // Hiç bir reklamla karşılaştırma yapılmadığı için 0'ıncı boş reklamı gösterir.
    
    for(i=2;i<=N;i++) { // info[i].profit'i kontrol eder.
        j=1; // En son çakışmayan reklamı aramak için offset.
        while(i != j && ads[i-j-1].startTime + ads[i-j-1].duration > ads[i-1].startTime) // ads dizisi için -1 offseti kullanılıyor. Çünkü reklamların indisleri 0dan başlıyor.
            j++;                                                                         // Bir önceki reklamla kesişik kesişmediği kontrol ediliyor. Son kesişmeyen reklama gidiyor.
            
        temp = info[i-j].profit + ads[i-1].value; // Bir önceki kesimeyen reklamın info degeri ile kendi degerinin degeri tutulur.
        
        if(i!=j) { // Kesişmeyen reklam var mı diye kontrol ediliyor.//Bu if ekrana daha düzgün yazılar basabilmek için.
            if(temp > info[i-1].profit) { // En son kesişmediği reklam ile kendi değeri toplamı önceki gelirden fazlaysa yeni değer göze yazılır.
                printf("Durum[%d]: %d. reklam ve son kesimedigi reklam olan %d.reklamin durumu ile toplamlari bir onceki durumdan karli ciktigi icin %d.goze yazilir.\n",i,ads[i-1].no,ads[i-j-1].no,i);
                info[i].profit = temp;
                info[i].prevAd = i-j; // Kendinden önceki son kesişmeyen reklami gösterir.
            }
            else {
                printf("Durum[%d]: Bir onceki durum, %d. reklam ve son kesismedigi reklam olan %d.reklamin durumu ile toplamlarindan daha karli ciktigi icin %d.goze yazilir.\n",i,ads[i-1].no,ads[i-j-1].no,i);
                info[i].profit = info[i-1].profit;
                info[i].prevAd = i-1; // Kendinden önceki kesişmeyen reklamın karı yeterli olmadığı için bir önceki profitin kullanılacağını söyler.
            }
        }
        else { // Eğer her reklamla kesişiyorsa. Bir önceki profitin kendi değerinden daha iyi olup olmadığı kontrol ediliyor.
            if(temp > info[i-1].profit) {
                printf("Durum[%d]: %d.reklam her reklamla kesitsigi icin ve bir onceki durumdan daha karli oldugu icin %d.goze yazilir.\n",i,ads[i-1].no,i);
                info[i].profit = temp;  // Kendi değeri önceki kazançlardan iyi ise göze yazılır.
                info[i].prevAd = 0;              
            }
            else  {
                printf("Durum[%d]: Bir onceki durum, %d.reklam her reklamla kesitsigi icin ve bir onceki durumdan daha az karli oldugu icin %d.goze yazilir.\n",i,ads[i-1].no,i);
                info[i].profit = info[i-1].profit; // Kendinden önceki kazanç kendisinden fazla ise önceki kazanç göze yazılır.
                info[i].prevAd = i-1;
            }
        }
    }
    
    printf("\n");
    for(i=0;i<=N;i++) 
        printf("Durum[%d] = %2d\n",i,info[i].profit); // Kazanc dizisi adım adım gösterilir.
    printf("\nToplam kazanc %d, yayinlanacak reklamlar: ",info[N].profit);
    
    i=N;
    
    while(i!=0) { // info[i].prevAd'den backtracking yapılır.
        if(info[i].profit != info[i-1].profit) { // O göze denk gelen reklamin kullanilip kullanilmadiğini kontrol eder.
            printf("%d ",ads[i-1].no); // Eğer o reklam kullanılmıssa reklamin numarasi ekrana yazılır.
            i = info[i].prevAd; // Ve en son kesişmediği reklama zıplanır.
        }
        else
            i--; // Eğer o reklam kullanılmamışsa bir önceki göz kullanılmış demektir. O yüzden info dizisinde bir index küçültülür.
    }
    printf("olur.\n");
        
    
    free(ads);
    free(info);
    
	
	return 0;
}
