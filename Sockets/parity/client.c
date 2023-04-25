#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

int main(){
	int cs=socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un saddr;
	saddr.sun_family=AF_UNIX;
	strcpy(saddr.sun_path,"mysocket");
	connect(cs,(struct sockaddr*)&saddr,sizeof(saddr));
	char data[32],bit[1];
	while(1){
		printf("Enter a bit stream or type 'exit' to exit: ");
        	scanf("%s",data);
        	printf("Sending data\n");
        	write(cs,(void*)data,strlen(data)+1);
        	if(strcasecmp(data,"exit")==0){
            		printf("Exiting...\n");
            		break;
        	}
        	printf("Waiting for data from server\n");
        	read(cs,(void*)data,sizeof(data));
        	printf("Modified bit stream with added parity bit: %s \n\n",data);
	}
	unlink("mysocket");
	close(cs);
	return 0;
}
