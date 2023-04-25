#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#define MAXLIMIT 64

typedef struct msgq{
	long mtype;
	char mdata[MAXLIMIT];
}mq;

char* dec_to_binary(int decimal){
	int size=sizeof(int)*8;
	char* binary=(char*)malloc(size+1);
	int i=0;
	for(i=size-1;i>=0;i--){
		binary[i]=(decimal&1)+'0';
		decimal=decimal>>1;
	}
	binary[size]='\0';
	return binary;
}

char* dec_to_octal(int decimal){
	int size=sizeof(int)*8;
	char* octal=(char*)malloc(size+1);
	int i=0;
	for(i=size-1;i>=0;i--){
		octal[i]=(decimal&7)+'0';
		decimal=decimal>>3;
	}
	octal[size]='\0';
	return octal;
}

char* dec_to_hex(int decimal){
	int size=sizeof(int)*8;
	char* hex=(char*)malloc(size+1);
	int i=0,val=0;
	for(i=size-1;i>=0;i--){
		val=decimal&15;
		if(val<=9){
			hex[i]=val+'0';
		}
		else{
			hex[i]=(val-10)+'A';
		}
		decimal=decimal>>4;
	}
	hex[size]='\0';
	return hex;
}

int main(int argc,char* argv[]){
	mq mqw;
	int mqid=msgget(1234,0666|IPC_CREAT);
	int digit=0;
	printf("Enter a decimal number: ");
	scanf("%d",&digit);
	mqw.mtype=2;
	strcpy(mqw.mdata,dec_to_binary(digit));
	msgsnd(mqid,(void*)&mqw,MAXLIMIT,0);
	mqw.mtype=3;
	strcpy(mqw.mdata,dec_to_octal(digit));
	msgsnd(mqid,(void*)&mqw,MAXLIMIT,0);
	mqw.mtype=4;
	strcpy(mqw.mdata,dec_to_hex(digit));
	msgsnd(mqid,(void*)&mqw,MAXLIMIT,0);
	return 0;
}
