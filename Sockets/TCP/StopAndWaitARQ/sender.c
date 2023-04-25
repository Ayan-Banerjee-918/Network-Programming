#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <pthread.h>
#define PORT 4444

typedef struct data{
	int ss;
	pthread_t th;
	int tot;
	int seqNo;
	sem_t sem;
}req;

void* timer(void* args){
	req *r=(req*)args;
	printf("\nStarting Timer\n");
	sleep(3);
	r->tot=1;
	sem_post(&(r->sem));
	pthread_exit(NULL);
}

void* sender(void* args){	
	req *r=(req*)args;
	int ss=r->ss;
	while(1){
		if(!r->tot){
			printf("\nSending frame with sequence number %d\n",r->seqNo);
			write(r->ss,&(r->seqNo),sizeof(r->seqNo));
		}
		else{
			r->seqNo=(r->seqNo+1)&1;
			printf("\nTimer timed out!No Acknowledgement received!\nRe-sending frame with sequence number: %d\n",r->seqNo);
			write(r->ss,&(r->seqNo),sizeof(r->seqNo));
		}
		r->seqNo=(r->seqNo+1)&1;	
		r->tot=0;
		pthread_create(&(r->th),NULL,(void*)timer,(void*)r);
		sem_wait(&(r->sem));
		pthread_join(r->th,NULL);
	}
	pthread_exit(NULL);
}

void* receiver(void* args){
	req *r=(req*)args;
	int ss=r->ss;
	int ack=-1;
	while(1){
		read(ss,&ack,sizeof(ack));
		if(ack==r->seqNo){		
			printf("\nAcknowledgement with sequence number %d received\n",ack);
			pthread_cancel(r->th);
			r->tot=0;
			sem_post(&(r->sem));
		}
		else{
			printf("\nDelayed Acknowledgment received with sequence number %d. Discarded!\n",ack);
		}
	}
	pthread_exit(NULL);
}

int main(int argc,char* argv[]){
	req r;
	int reuse=1;
	r.ss=socket(AF_INET,SOCK_STREAM,0);
	setsockopt(r.ss,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
	r.seqNo=0;
	r.tot=0;
	int i=0;
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(PORT);
	if(argc==1){
		inet_aton("127.0.0.1",&saddr.sin_addr);
	}
	else{
		inet_aton(argv[1],&saddr.sin_addr);
	}
	connect(r.ss,(struct sockaddr*)&saddr,sizeof(saddr));
	sem_init(&(r.sem),0,1);
	pthread_t th[2];
	pthread_create(&th[0],NULL,&sender,(void*)&r);
	pthread_create(&th[1],NULL,&receiver,(void*)&r);
	pthread_join(th[0],NULL);
	pthread_join(th[1],NULL);
	sem_destroy(&(r.sem));
	return 0;
}
