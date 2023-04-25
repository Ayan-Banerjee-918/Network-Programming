#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define SPORT 1234
#define DPORT 1235

typedef struct arg{
	int ss;
	struct sockaddr_in caddr;
}dat;

void* sender(void* args){
	dat d1=*((dat*)args);
	int ss=d1.ss;
	char data[1024];
	struct sockaddr_in caddr=d1.caddr;
	while(1){
		fgets(data,sizeof(data),stdin);
		data[strlen(data)-1]='\0';
		sendto(ss,(void*)data,strlen(data)+1,0,(struct sockaddr*)&caddr,sizeof(caddr));
		if(strcasecmp(data,"bye")==0){
			break;
		}
	}
	pthread_exit(NULL);
}

void* receiver(void* args){
	dat d1=*((dat*)args);
	int ss=d1.ss;
	struct sockaddr_in caddr=d1.caddr;
	int l=sizeof(caddr);
	char data[1024];
	while(1){
		recvfrom(ss,(void*)data,sizeof(data),0,(struct sockaddr*)&caddr,&l);
		printf("Received message: %s\n",data);
		if(strcasecmp(data,"bye")==0){
			break;
		}
	}
	pthread_exit(NULL);
}

int main(int argc,char* argv[]){
	char sipaddr[32],dipaddr[32];
	pthread_t th[2];
	dat d1;
	int i=0;
	int ss=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in saddr,caddr;
	if(argc==1 || inet_pton(AF_INET,argv[1],NULL)<=0 || inet_pton(AF_INET,argv[2],NULL)<=0){
		strcpy(sipaddr,"127.0.0.1");
		strcpy(dipaddr,"127.0.0.1");
	}
	else{
		strcpy(sipaddr,argv[1]);
		strcpy(dipaddr,argv[2]);
	}
	saddr.sin_family=AF_INET;
	inet_aton(sipaddr,&(saddr.sin_addr));
	saddr.sin_port=htons(SPORT);
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));
	d1.ss=ss;
	caddr.sin_family=AF_INET;
	inet_aton(dipaddr,&(caddr.sin_addr));
	caddr.sin_port=htons(DPORT);
	d1.caddr=caddr;
	pthread_create(&th[0],NULL,&sender,(void*)&d1);
	pthread_create(&th[1],NULL,&receiver,(void*)&d1);
	for(i=0;i<2;i++){
		pthread_join(th[i],NULL);
	}
	printf("Chat session terminated...\n");
	return 0;
}
