#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
/* What's happening ;A; */
//writes contents of array to file
void printArray(int * array, FILE * fout, int MAX_CHAR)
{ 
     int i;
     fout = fopen("flash.txt","w+b+");    //file named flash.txt
     for (i=0; i<MAX_CHAR; i++)
        fprintf(fout,"%d",*(array+i)); 
     //	fflush(fout);
	fclose(fout);                        

}

//creates a working flash (bootable)
void  popArray(int fArr[],int size){
	printf("printing things");
	int i = 0;
	for(i = 0; i < size; i++){
		//first two blocks are P0
		if(i < 256){
			fArr[i] = 1;
		}
		//next four blocks are S1
		if(i >= 256 && i < 6*128){
			fArr[i] = 2;
		}
		//last two blocks are ZXENV
		if(i >= 6*128){
			fArr[i] = 3;
		}
	}
	printf("\n");
	FILE *f;
	
	printArray(fArr, f, size);
}
//clears (replace with 0) data in an array in between the start and end
void erase(int arr[], int start, int end,int size, char txt[]){
	printf("Erasing Previous %s \n",txt);
	FILE *f;
	char t[1];
	strcpy(t,"0");
	 
	int i;
	

	for(i = start; i < end; i++){
		f = fopen("flash.txt","r+b+");
		fseek(f,i,SEEK_SET);
		fwrite(t,1,sizeof(t),f);
		usleep(10);	
//	fprintf(f,"0");
//		fputs("0",f);
//	fflush(f);
//		arr[i] = 0;
//		printArray(arr, f, size);
		fclose(f);	
	}
	
}

//Writes specified data in between start and end
void ewrite(int arr[], int start, int end, char image[],int size){
	int i;
	char writ[1];	
	//usleep(10000);

	if(image == "XXENV")
		strcpy(writ,"5");	//signifies XXENV
	if(image == "XOS3")
		strcpy(writ,"6");	//signifies XOS3
	if(image == "Stage0")
		strcpy(writ,"7");	//signifies Stage0
	if(image == "Stage1")
		strcpy(writ,"8");
	if(image == "ZXENV")
		strcpy(writ,"9");
	for(i = start; i < end; i++){

		FILE *f=fopen("flash.txt","rb+");
		fseek(f,i,SEEK_SET);
		fwrite(writ,1,sizeof(writ),f);
	//	fprintf(f,"%d",writ);
//		fflush(f);
		//arr[i] = writ;	
		usleep(10);
		//printArray(arr, f, size);
		fclose(f);
	}

}

void upP0Main(int arr[], int size){
	erase(arr,0,128,size,"P0");	//erase block 0
	printf("XXENV Updating\n");
	ewrite(arr,32,48,"XXENV",size);	//write new XXENV

	printf("XOS3 Updating\n");
	ewrite(arr,48,96,"XOS3",size);	//write new XOS3

	printf("Stage0 Updating\n");	//write new Stage0
	ewrite(arr,0,32,"Stage0",size);
}

void upP0Back(int arr[], int size){
	erase(arr,128,256,size,"P0 Backup");	//erase block 0
	printf("XXENV_2 Updating\n");
	ewrite(arr,160,176,"XXENV",size);	//write new XXENV

	printf("XOS3_2 Updating\n");
	ewrite(arr,176,224,"XOS3",size);	//write new XOS3

	printf("Stage0_2 Updating\n");	//write new Stage0
	ewrite(arr,128,160,"Stage0",size);
}
void upS1Main(int arr[], int size){
	erase(arr,256,512,size,"Stage1");
	printf("Stage1 Updating\n");
	ewrite(arr,256,512,"Stage1",size);

}
void upS1Back(int arr[], int size){
	erase(arr, 512, 768, size,"Stage1 Backup");
	printf("Stage1_2 Updating\n");
	ewrite(arr,512,768,"Stage1",size);
}
void upZMain(int arr[], int size){
	erase(arr, 768, 896,size,"ZXENV");
	printf("ZXENV Updating\n");
	ewrite(arr,768,896,"ZXENV",size);
}

void upZBack(int arr[], int size){
	erase(arr,896,1024,size,"ZXENV Backup");
	printf("ZXENV_2 Updating\n");
	ewrite(arr,896,1024,"ZXENV",size);
}
//Updates 
void update(int arr[],int size){
	
	
	upP0Main(arr,size);
	upP0Back(arr,size);
	upS1Main(arr,size);
	upS1Back(arr,size);
	upZMain(arr,size);
	upZBack(arr,size);
	printf("\nUpdating Complete!\n");
}

int main(){
	int size = 8*128;	
	int flash[size];

	popArray(flash,size);	//new flash
	update(flash,size);	//write new block 0
	//not exactly sure what this does, but it's necessary
//	erase(flash,1,24,size,"poo");
	
	return 0;
}
