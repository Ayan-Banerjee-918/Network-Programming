#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 4444

int main(int argc,char* argv[]){
	int reuse=1;
	int ss=socket(AF_INET,SOCK_STREAM,0);
	setsockopt(ss,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
	struct sockaddr_in saddr,raddr;
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
	int len=sizeof(raddr);
	int ts=accept(ss,(struct sockaddr*)&raddr,&len);
	int seqNo=-1,ack=0,delay=0;
	float chk;
	srand(time(NULL));
	while(1){
		read(ts,(void*)&seqNo,sizeof(seqNo));
		if(seqNo==ack){
			printf("\nReceived frame with sequence number: %d\n",seqNo);
			ack=(ack+1)&1;
		}
		else{
			printf("\nReceived duplicate frame with sequence number: %d\n",seqNo);
			ack=(seqNo+1)&1;
		}
		chk=((float)rand()/(float)RAND_MAX);
		if(chk>0.8){
			printf("\nSimulating Lost Acknowledgement\n");
		}
		else{ 
			if(chk>0.6){
				printf("\nSimulating Delayed Acknowledgement\n");
				delay=(rand()%3)+3;
				printf("Delay: %d s\n",delay);
				sleep(delay);
			}
			write(ts,&ack,sizeof(ack));
			printf("\nAcknowledgement with sequence number %d sent\n",ack);
		}
	}
	return 0;
}
