#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#define NAMELIMIT 100

typedef struct Student{
	int roll;
	char name[NAMELIMIT];
}stud;

typedef struct msgq{
	long mtype;
	char mdata[sizeof(stud)];
}mq;

void printData(stud* sd,int N){	
	int i=0;
	for(i=0;i<N;i++){
		printf("\nSTUDENT %d\n",i+1);
		printf("Name: %s\nRoll: %d\n",sd[i].name,sd[i].roll);	
	}
}

int main(){
	mq mqr,mqw;
	int N=0;
	int mqid=msgget(2345,0666|IPC_CREAT);
	msgrcv(mqid,(void*)&mqr,sizeof(int),3,0);
	sscanf(mqr.mdata,"%d",&N);	
	printf("N:%d\n",N);
	stud* sd=(stud*)malloc(N*sizeof(stud));
	int i=0,j=0;
	for(i=0;i<N;i++){
		msgrcv(mqid,(void*)&mqr,sizeof(stud),3,0);
		memcpy(&sd[i],mqr.mdata,sizeof(stud));
	}
	printf("Received Data:\n\n");
	printData(sd,N);
	stud key;
	for(i=1;i<N;i++){
		key=sd[i];
		j=i-1;
		while(j>=0 && key.roll<sd[j].roll){
			sd[j+1]=sd[j];
			j--;
		}
		sd[j+1]=key;
	}
	printf("\nSorted Data:\n\n");
	printData(sd,N);	
	for(i=0;i<N;i++){
		mqw.mtype=4;
		memcpy(mqw.mdata,&sd[i],sizeof(stud));
		msgsnd(mqid,(void*)&mqw,sizeof(stud),0);
	}
	return 0;
}
