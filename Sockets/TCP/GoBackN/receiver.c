#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#define PORT 4444

int main(int argc,char* argv[]){
	int reuse=1;
	int ss=socket(AF_INET,SOCK_STREAM,0);
	setsockopt(ss,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
	struct sockaddr_in raddr;
	raddr.sin_family=AF_INET;
	raddr.sin_port=htons(PORT);
	srand(time(NULL));
	if(argc==1){
		inet_aton("127.0.0.1",&raddr.sin_addr);
	}
	else{
		inet_aton(argv[1],&raddr.sin_addr);
	}
	connect(ss,(struct sockaddr*)&raddr,sizeof(raddr));
	int pos=0,frameNo=0;
	float decider;
	while(1){
		read(ss,&frameNo,sizeof(frameNo));
		printf("\nReceived frame with seqNo: %d\n",frameNo);
		if(frameNo==pos){
			decider=((float)rand()/(float)RAND_MAX);
			if(decider<0.9){
				pos+=1;
				printf("\nSending Acknowledgement-%d for received frame-%d\n",pos,frameNo);
				write(ss,&pos,sizeof(pos));
			}
			else{
				printf("\nSimulating lost acknowledgement-%d\n",pos+1);
			}
		}
		else{
			printf("\nOut Of Order Frame! Discarded!\n");
			write(ss,&pos,sizeof(pos));
		}
	}
	return 0;
}
