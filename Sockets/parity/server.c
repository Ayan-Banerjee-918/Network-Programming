#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

int main(){
	int ss=socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un saddr,caddr;
	saddr.sun_family=AF_UNIX;
	strcpy(saddr.sun_path,"mysocket");
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));
	listen(ss,1);
	int len=sizeof(caddr);
	int ts=accept(ss,(struct sockaddr*)&caddr,&len);
	char data[32],bit[1];
	while(1){
		printf("\nWaiting for data from client\n");
		read(ts,(void*)data,sizeof(data));
		if(strcasecmp(data,"exit")==0){
			printf("Exiting...\n");
			break;
		}
		printf("Received data: %s\n",data);
		int i=0,cnt=0;
		for(i=0;i<strlen(data);i++){
			if(data[i]=='1'){
				cnt++;
			}
		}
		bit[0]='0'+(cnt%2);
		strncat(data,bit,1);
		printf("Sending data: %s\n",data);
		write(ts,(void*)data,strlen(data)+1);

	}
	unlink("mysocket");
	close(ts);
	close(ss);
	return 0;
}
