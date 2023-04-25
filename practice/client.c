#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string.h>

int main(){
	int cs=socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un caddr;
	caddr.sun_family=AF_UNIX;
	strcpy(caddr.sun_path,"mysocket");
	connect(cs,(struct sockaddr*)&caddr,sizeof(caddr));
	char data[100],res;
	while(1){
		printf("\nEnter Ip address:\n");
		scanf("%s",data);
		write(cs,(void*)&data,strlen(data)+1);
		if(strcasecmp(data,"exit")==0){
			printf("Exiting\n");
			break;
		}
		printf("\nWaiting for data from server\n");
		read(cs,(void*)&res,sizeof(res));
		if(res=='1'){
			printf("IP Address %s is valid ipv4\n",data);
		}
		else{
			printf("IP Address %s is invalid ipv4\n",data);
		}
	}
	unlink("mysocket");
	close(cs);
	return 0;
}
