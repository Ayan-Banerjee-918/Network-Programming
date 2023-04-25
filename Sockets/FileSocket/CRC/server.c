#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

char* crc(char* dividend,char* divisor){
	int dividend_len,divisor_len=0;	
	dividend_len=strlen(dividend);
	divisor_len=strlen(divisor);
	char* data=malloc(128*sizeof(char));
	int i=0,j=0,msb=0;
	snprintf(data,128,"%s%0*d",data,divisor_len-1,0);
	for(i=0;i<dividend_len;i++){
		msb=dividend[0]-'0';
		if(msb==1){
			for(j=0;j<divisor_len;j++){
				data[j]= (data[j]==divisor[j])?'0':'1';
			}
		}
		data++;
		
	}
	return data;
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
	char dividend[128],divisor[128],res[128];
	while(1){
		printf("\nWaiting for data from client\n");
		read(ts,(void*)dividend,sizeof(dividend));
		if(strcasecmp(dividend,"exit")==0){
			printf("Exiting...\n");
			break;
		}
		printf("Received data: %s\n",dividend);
		read(ts,(void*)divisor,sizeof(divisor));
		printf("Received divisor: %s\n",divisor);
		strcpy(res,crc(dividend,divisor));
		printf("Calculated CRC: %s\nSending data to client",res);
		write(ts,(void*)res,strlen(res)+1);
	}
	unlink("mysocket");
	close(ts);
	close(ss);
	return 0;
}
