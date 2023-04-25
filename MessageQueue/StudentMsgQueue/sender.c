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
	mq mqw,mqr;
	int N=0;
	printf("Enter the number of students:");
	scanf("%d",&N);
	stud* sd=(stud*)malloc(N*sizeof(stud));
	stud* sd1=(stud*)malloc(N*sizeof(stud));
	int mqid=msgget(2345,0666|IPC_CREAT);
	mqw.mtype=2;
	sprintf(mqw.mdata,"%d",N);
	msgsnd(mqid,(void*)&mqw,sizeof(int),0);
	mqw.mtype=3;
	sprintf(mqw.mdata,"%d",N);
	msgsnd(mqid,(void*)&mqw,sizeof(int),0);
	int i=0;
	for(i=0;i<N;i++){
		printf("\nSTUDENT %d\n",i+1);
		printf("Enter roll:");
		scanf("%d",&sd[i].roll);
		getchar();
		printf("Enter name:");
		fgets(sd[i].name,NAMELIMIT,stdin);
		sd[i].name[strlen(sd[i].name)-1]='\0';
		mqw.mtype=2;
		memcpy(mqw.mdata,&sd[i],sizeof(stud));
		msgsnd(mqid,(void*)&mqw,sizeof(stud),0);
		mqw.mtype=3;
		memcpy(mqw.mdata,&sd[i],sizeof(stud));
		msgsnd(mqid,(void*)&mqw,sizeof(stud),0);
	}
	printf("\nSorted data (by name) received from receiver 1:\n");
	for(i=0;i<N;i++){
		msgrcv(mqid,(void*)&mqr,sizeof(stud),1,0);
		memcpy((void*)&sd[i],mqr.mdata,sizeof(stud));
	}
	printData(sd,N);
	printf("\nSorted data (by roll) received from receiver 2:\n");
	for(i=0;i<N;i++){
		msgrcv(mqid,(void*)&mqr,sizeof(stud),4,0);
		memcpy((void*)&sd1[i],mqr.mdata,sizeof(stud));
	}
	printData(sd1,N);
	msgctl(mqid,IPC_RMID,NULL);
	return 0;
}
