#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

typedef struct dat{
	char binary[32];
	int shift;
}dt;

typedef struct msgq{
	long mtype;
	char mdata[sizeof(dt)];
}mq;

int main(){
	mq mqw,mqr;
	int mqid=msgget(1234,0666|IPC_CREAT);
	int N=0;
	msgrcv(mqid,(void*)&mqr,sizeof(int),2,0);
	sscanf(mqr.mdata,"%d",&N);
	printf("N: %d\n",N);
	dt* d=(dt*)malloc(N*sizeof(dt));
	int i=0;
	for(i=0;i<N;i++){
		msgrcv(mqid,(void*)&mqr,sizeof(dt),2,0);
		memcpy(&d[i],mqr.mdata,sizeof(dt));
		d[i].shift++;
		d[i].binary[strlen(d[i].binary)-1]='\0';
	}
	for(i=0;i<N;i++){
		mqw.mtype=3;
		memcpy(mqw.mdata,&d[i],sizeof(dt));
		msgsnd(mqid,(void*)&mqw,sizeof(dt),0);
	}
	printf("Modified data:\n");
	for(i=0;i<N;i++){
		printf("Data %d\n",i+1);
		printf("%s \n %d \n",d[i].binary,d[i].shift);
	}
	return 0;
}
