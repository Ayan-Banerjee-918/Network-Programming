#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	char ipaddr[32],data[64],hammingCode[128];
	int l=0;
	struct sockaddr_in saddr;
	if(argc==1){
		strcpy(ipaddr,"127.0.0.1");
	}
	else{
		strcpy(ipaddr,argv[1]);
	}
        int cs=socket(AF_INET,SOCK_DGRAM,0);
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(1234);
	inet_aton(ipaddr,&(saddr.sin_addr));
	while(1){
		printf("\nEnter dataword:");
		fgets(data,sizeof(data),stdin);
		data[strlen(data)-1]='\0';
		sendto(cs,(void*)data,strlen(data)+1,0,(struct sockaddr*)&saddr,sizeof(saddr));
		if(strcasecmp(data,"exit")==0){
			printf("Exiting!\n");
			break;
		}
		usleep(10000);
		l=sizeof(saddr);
		recvfrom(cs,(void*)hammingCode,sizeof(hammingCode),0,(struct sockaddr*)&saddr,&l);
		printf("Received Codeword: %s\n",hammingCode);
	}
	close(cs);
	return 0;
}
