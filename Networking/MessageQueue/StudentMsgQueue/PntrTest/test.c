#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

typedef struct msgq{
	long mtype;
	char mdata[1024];
}mq;

int main(){
	mq mqw;
	int mqid=msgget(1234,0666|IPC_CREAT);
	int *N=(int*)malloc(sizeof(int));
	*N=3;
	mqw.mtype=2;
	sprintf(mqw.mdata,"%p",N);
	printf("%s\n",mqw.mdata);
	msgsnd(mqid,(void*)&mqw,1024,0);
	printf("%p\n",N);
	return 0;
}
