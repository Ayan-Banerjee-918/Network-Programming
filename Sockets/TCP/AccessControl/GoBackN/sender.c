#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <time.h>
#define PORT 4444

typedef struct data{
	int ts;
	pthread_t* th;
	int tot;
	int seqNo;
	int N;
	int start;
	int end;
	int exit;
}req;

void* timer(void* args){
	req *r=(req*)args;
	int i=0;
	sleep(5);
	r->tot=1;
	//the below commented portion causing segmentation fault in some machines, needs fix
	/*for(i=0;i<r->N;i++){
		pthread_cancel(r->th[i]);
	}*/
	pthread_exit(NULL);
}

void* receiver(void* args){
	req* r=(req*)args;
	int ts=r->ts;
	int ack=-1,i=0;
	while(!r->exit){
		read(ts,&ack,sizeof(ack));
		if((ack>r->start) && (ack<=r->end)){
			printf("\nAcknowledgement-%d received\n",ack);
			for(i=0;i<(ack-1)%(r->N);i++){
				pthread_cancel(r->th[i]);
			}
			while(r->start<ack){
				r->start++;
			}
			if(ack==r->seqNo+1){
				r->exit=1;
			}
		}
		else{
			printf("\nDuplicate Acknowledgement-%d received!Discarded!\n",ack);
		}
	}
	pthread_exit(NULL);
}

void* sender(void* args){
	req* r=(req*)args;
	srand(time(NULL));
	int i=0;
	int ts=r->ts;
	float decider;
	while(!r->exit){
		if(r->tot==1){
			printf("\nTimer timed out!\nRe-sending frames from sequence number %d to %d\n",r->start,r->end-1);
			for(i=r->start;i<r->end;i++){
				decider=((float)rand()/(float)RAND_MAX);
				if(decider<0.9){
					printf("\nFrame with sequence number %d sent\n",i);
					write(ts,&i,sizeof(i));	
					pthread_create(&(r->th[i]),NULL,(void*)timer,(void*)r);	
				}
				else{
					printf("\nSimulating lost packet %d\n",i);
				}
			}
			r->tot=0;
		}
		else{
			while(r->end-r->start<r->N && r->end<r->seqNo+1){
				decider=((float)rand()/(float)RAND_MAX);
				if(decider<0.9){
					i=r->end%(r->N-1);
					printf("\nSending frame with sequence number %d\n",r->end);
					write(ts,&(r->end),sizeof(r->end));
					r->end++;
					pthread_create(&(r->th[i]),NULL,(void*)timer,(void*)r);	
				}
				else{
					printf("\nSimulating lost packet %d\n",r->end);
					r->end++;
				}
			}
		}
	}
	for(i=0;i<r->N;i++){
		pthread_join(r->th[i],NULL);
	}
	pthread_exit(NULL);
}

int main(int argc,char* argv[]){
	req r;
	int reuse=1,len=0;
	int ss=socket(AF_INET,SOCK_STREAM,0);
	setsockopt(ss,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
	struct sockaddr_in saddr,caddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(PORT);
	if(argc==1){
		inet_aton("127.0.0.1",&saddr.sin_addr);
	}
	else{
		inet_aton(argv[1],&saddr.sin_addr);
	}
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));
	listen(ss,1);
	len=sizeof(caddr);
	r.ts=accept(ss,(struct sockaddr*)&caddr,&len);
	printf("Enter size of window:");
	scanf("%d",&r.N);
	r.seqNo=1<<r.N;
	r.start=0;
	r.end=0;
	r.tot=0;
	r.exit=0;
	r.th=(pthread_t*)malloc(r.N*sizeof(pthread_t));
	pthread_t th[2];
	pthread_create(&th[0],NULL,&sender,(void*)&r);
	pthread_create(&th[1],NULL,&receiver,(void*)&r);
	pthread_join(th[0],NULL);
	pthread_join(th[1],NULL);
	return 0;
}
