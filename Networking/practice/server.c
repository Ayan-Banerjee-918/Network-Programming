#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char check(char* data){
	if(inet_pton(AF_INET,data,&data)<=0){
		return '0';
	}
	return '1';
}

int main(){
	int ss=socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un saddr,caddr;
	saddr.sun_family=AF_UNIX;
	strcpy(saddr.sun_path,"mysocket");
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));
	listen(ss,1);
	int len=sizeof(caddr);
	int ts=accept(ss,(struct sockaddr*)&caddr,&len);
	char data[100],res;
	while(1){
		printf("\nWaiting for data from client\n");
		read(ts,(void*)&data,sizeof(data));
		if(strcasecmp(data,"exit")==0){
			printf("Exiting\n");
			break;
		}
		printf("Received address: %s",data);
		res=check(data);
		write(ts,(void*)&res,sizeof(char));
		printf("\nResponse sent to client!\n");
	}
	unlink("mysocket");
	close(ts);
	close(ss);
	return 0;
}
