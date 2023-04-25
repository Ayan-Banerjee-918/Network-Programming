#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct msgq{
	long mtype;
	char mdata[1024];
}mq;

int main(){
	mq mqw;
	int mqid=msgget(1234,0666|IPC_CREAT);
	int* N;
	msgrcv(mqid,(void*)&mqw,1024,2,0);
	printf("%s\n",mqw.mdata);
	N=(void*)(uintptr_t)mqw.mdata;
	printf("%p\n",N);
	printf("N:%d\n",*N);
	msgctl(mqid,IPC_RMID,NULL);
	return 0;
}
