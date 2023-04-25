#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
	int cs=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in caddr;
	caddr.sin_family=AF_INET;
	caddr.sin_port=ntohs(1234);
	inet_aton("127.0.0.1",&(caddr.sin_addr));
	connect(cs,(struct sockaddr*)&caddr,sizeof(caddr));
	char data[128],res[3];
	while(1){
		printf("\nEnter ipv4 address (type 'exit' to exit): ");
		scanf("%s",data);
		write(cs,(void*)data,strlen(data)+1);
		if(strcasecmp(data,"exit")==0){
			printf("Exiting...\n");
			break;
		}
		printf("\nWaiting for data from server\n");
		read(cs,(void*)res,sizeof(res));
		if(strcasecmp(res,"NO")==0){
			printf("\nNO, IPV4 Address %s is invalid!\n",data);
		}
		else{
			printf("\nYES, IPV4 Address %s is valid!\n",data);
		}
	}
	close(cs);
	return 0;
}
