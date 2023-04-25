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
	char tmp[32];
	msgrcv(mqid,(void*)&mqr,sizeof(int),3,0);
	sscanf(mqr.mdata,"%d",&N);
	printf("N: %d\n",N);
	dt* d=(dt*)malloc(N*sizeof(dt));
	int i=0;
	for(i=0;i<N;i++){
		msgrcv(mqid,(void*)&mqr,sizeof(dt),3,0);
		memcpy(&d[i],mqr.mdata,sizeof(dt));
		strcpy(tmp,d[i].binary);
		snprintf(tmp,64,"%s%0*d",tmp,d[i].shift,0);
		strcpy(d[i].binary,tmp);
		mqw.mtype=1;
		memcpy(mqw.mdata,&d[i],sizeof(dt));
		msgsnd(mqid,(void*)&mqw,sizeof(dt),0);
	}
	printf("Shifted data:\n");
	for(i=0;i<N;i++){
		printf("Data %d\n",i+1);
		printf("%s \n %d \n",d[i].binary,d[i].shift);
	}
	msgctl(mqid,IPC_RMID,NULL);
	return 0;
}
