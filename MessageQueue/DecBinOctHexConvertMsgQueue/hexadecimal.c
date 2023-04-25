#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#define MAXLIMIT 64

typedef struct msgq{
	long mtype;
	char mdata[MAXLIMIT];
}mq;

int main(){
	mq mqr;
	int mqid=msgget(1234,0666|IPC_CREAT);
	msgrcv(mqid,(void*)&mqr,MAXLIMIT,4,0);
	printf("Decimal number in hexadecimal: %s\n",mqr.mdata);
	return 0;
}
