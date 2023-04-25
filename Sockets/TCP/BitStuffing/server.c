#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* stuff(char* data){
	int cnt=0;
	int i=0,prev=-1,j=0;
	char* result=(char*)malloc(strlen(data)<<1);
	for(i=0;i<strlen(data);i++){
		if(data[i]=='1' && (prev==i-1 || prev<0)){
			cnt++;
			prev=i;
		}
		else{
			prev=-1;
			cnt=0;
		}
		if(cnt==6){
			result[j++]='0';
			prev=-1;
			cnt=0;
			i--;
		}
		else{
			result[j++]=data[i];
		}
	}
	result[j]='\0';
	return result;
}

int main(){
	int ss=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in saddr,caddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(1234);
	inet_aton("127.0.0.1",&(saddr.sin_addr));
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));
	listen(ss,1);
	int len=sizeof(caddr);
	int ts=accept(ss,(struct sockaddr*)&caddr,&len);
	char data[128];
	while(1){
		printf("\nWaiting for data from client\n");
		read(ts,(void*)data,sizeof(data));
		if(strcasecmp(data,"exit")==0){
			printf("Exiting...\n");
			break;
		}
		printf("Received bit stream: %s\n",data);
		strcpy(data,stuff(data));
		printf("Stuffed bit stream: %s\n",data);
		write(ts,(void*)data,strlen(data)+1);
	}
	close(ts);
	close(ss);
	return 0;
}
