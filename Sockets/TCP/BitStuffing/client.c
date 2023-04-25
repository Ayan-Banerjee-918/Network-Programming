#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* unstuff(char* data){
	int cnt=0;
	int i=0,prev=-1,j=0;
	char* result=(char*)malloc(strlen(data));
	for(i=0;i<strlen(data);i++){
		if(data[i]=='1' && (prev==i-1 || prev<0)){
			cnt++;
			prev=i;
		}
		else{
			prev=-1;
			cnt=0;
		}
		if(cnt==5 && data[i+1]=='0' && data[i+2]=='1'){
			result[j++]=data[i++];
			prev=-1;
			cnt=0;
		}
		else{
			result[j++]=data[i];
		}
	}
	result[j]='\0';
	return result;
}

int main(){
	int cs=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in caddr;
	caddr.sin_family=AF_INET;
	caddr.sin_port=ntohs(1234);
	inet_aton("127.0.0.1",&(caddr.sin_addr));
	int success=connect(cs,(struct sockaddr*)&caddr,sizeof(caddr));
	char data[128];
	while(1){
		printf("\nEnter bit stream (type 'exit' to exit): ");
		scanf("%s",data);
		write(cs,(void*)data,strlen(data)+1);
		if(strcasecmp(data,"exit")==0){
			printf("Exiting...\n");
			break;
		}
		printf("\nWaiting for data from server\n");
		read(cs,(void*)data,sizeof(data));
		printf("Bit Stuffed stream received from server: %s\n",data);
		printf("Unstuffed bit stream: %s\n",unstuff(data));
	}
	close(success);
	close(cs);
	return 0;
}
