#include<stdio.h>

//checks for bootable new version
int newCheckS0(char contents[],int start, int end, int msg, int num){
	int i,j;
	char ch; 
	FILE *fp = fopen("flash.txt","r+");
	for(i = start; i < end; i++){
		if(contents[i] != num){
			if(msg==0){
				printf("Corrupted Image. Checking Backup.\n");
				printf("bit %d dun goofd is %c\n",i,contents[i]);

				//for(j = 0; j < sizeof(poo);j++)
				//printf("%d",poo[j]);
			}else{
				printf("Backup Corrupted, Boot Failed\n");
				printf("bit %d dun goofd is %c\n",i,contents[i]);
				//for(j = 0; j < sizeof(poo);j++)
				//printf("%d",poo[j]);
			}
			
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}

//checks for bootable old version
int oldCheckS0(char contents[],int start, int end,int msg, int num, int num2){
	int i;
	
	char ch;
	FILE *fp = fopen("flash.txt","r+");
	for(i = start; i < end; i++){
		if(contents[i] != num){
			fclose(fp);
			return newCheckS0(contents,start,end,msg,num2);
		}
	}
	fclose(fp);
	return 1;

}

//Authenticate Stage0 in block 0
int authS01(char contents[]){
	return (oldCheckS0(contents,0,32,0,49,55) & oldCheckS0(contents,32,48,0,49,53) & oldCheckS0(contents,48,96,0,49,54));
}
//Authenticate Stage0 in backup
int authS02(char contents[]){
	return (oldCheckS0(contents,128,160,1,49,55) & oldCheckS0(contents,160,176,0,49,53) & oldCheckS0(contents,176,224,1,49,54));
}
//Authenticates Stage 1 (main then backup)
int authS1(char contents[]){
	return (oldCheckS0(contents,256,512,0,50,56) || oldCheckS0(contents,512,768,1,50,56));
}
//Authenticates ZXENV (main then backup)
int authZ(char contents[]){
	return (oldCheckS0(contents,768,896,0,51,57) || oldCheckS0(contents,896,1024,1,51,57));
}

int main(){
	FILE *fp = fopen("flash.txt","r+");
	int i;
	int stg = 55;
	char ch;
	char contents[1024]={ 0 };
	fread(contents, sizeof(contents),1024, fp);
	if(authS01(contents) & authS1(contents) & authZ(contents)){
		fseek(fp,0,SEEK_SET);
		while( ( ch = fgetc(fp) ) != EOF )
      			printf("%c",ch);
		printf("\n");
		printf("Boot Successful!\n");
		fclose(fp);
		return 0;
	}else{
		if(authS02(contents) & authS1(contents) & authZ(contents)){
			fseek(fp,0,SEEK_SET);
			while( ( ch = fgetc(fp) ) != EOF )
      				printf("%c",ch);
			printf("\n");
			printf("Boot Successful!\n");
			fclose(fp);
			return 0;
		}
	}
	fseek(fp,0,SEEK_SET);
	while( ( ch = fgetc(fp) ) != EOF )
      		printf("%c",ch);
	printf("\n");		
	fclose(fp);
	return 0;
}
