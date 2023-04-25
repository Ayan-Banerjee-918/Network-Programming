#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MULTICAST_GRP "224.0.0.2"
#define MULTICAST_PORT 4321

int main(int argc,char* argv[]){
	struct sockaddr_in maddr;
	struct ip_mreqn mrq;
	char data[1024];
	int reuse=1;
	int ss=socket(AF_INET,SOCK_DGRAM,0);
	setsockopt(ss,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));	
	maddr.sin_family=AF_INET;
	maddr.sin_port=htons(MULTICAST_PORT);
	if(argc>1){
		inet_aton(MULTICAST_GRP,&(maddr.sin_addr));
		while(1){
			printf("\nEnter message: ");
			fgets(data,sizeof(data),stdin);
			data[strlen(data)-1]='\0';
			sendto(ss,(void*)data,strlen(data)+1,0,(struct sockaddr*)&maddr,sizeof(maddr));
			if(strcasecmp(data,"bye")==0)
				break;
		}
	}
	else{
		maddr.sin_addr.s_addr=htonl(INADDR_ANY);
		bind(ss,(struct sockaddr*)&maddr,sizeof(maddr));
		inet_aton(MULTICAST_GRP,&(mrq.imr_multiaddr));
		mrq.imr_address.s_addr=htonl(INADDR_ANY);
		mrq.imr_ifindex=0;
		int len=sizeof(maddr);
		setsockopt(ss,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mrq,sizeof(mrq));
		while(1){
			recvfrom(ss,(void*)data,sizeof(data),0,(struct sockaddr*)&maddr,&len);
			printf("Received from %s: %s\n",inet_ntoa(maddr.sin_addr),data);
		}
	}
	close(ss);
	return 0;
}
