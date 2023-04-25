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
	printf("Enter number of data:");
	int N=0;
	scanf("%d",&N);
	dt* d=(dt*)malloc(N*sizeof(dt));
	mqw.mtype=2;
	sprintf(mqw.mdata,"%d",N);
	msgsnd(mqid,(void*)&mqw,sizeof(int),0);
	mqw.mtype=3;
	sprintf(mqw.mdata,"%d",N);
	msgsnd(mqid,(void*)&mqw,sizeof(int),0);
	int i=0;
	for(i=0;i<N;i++){
		printf("\nData %d\n",i+1);
		printf("Enter data:");
		scanf("%s",&d[i].binary);
		printf("Enter how many bytes to shift:");
		scanf("%d",&d[i].shift);
		mqw.mtype=2;
		memcpy(mqw.mdata,&d[i],sizeof(dt));
		msgsnd(mqid,(void*)&mqw,sizeof(mqw),0);
	}
	printf("Final data:\n");
	for(i=0;i<N;i++){
		msgrcv(mqid,(void*)&mqr,sizeof(dt),1,0);
		memcpy((void*)&d[i],mqr.mdata,sizeof(dt));
		printf("Data %d\n",i+1);
		printf("%s shifted by %d bits\n",d[i].binary,d[i].shift);
	}
	msgctl(mqid,IPC_RMID,NULL);
	return 0;
}
