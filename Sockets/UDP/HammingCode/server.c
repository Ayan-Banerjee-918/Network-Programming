#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

char* revStr(char* data){
	int len=strlen(data);
	int i=0;
	char tmp;
	for(i=0;i<len/2;i++){
		tmp=data[i];
		data[i]=data[len-1-i];
		data[len-1-i]=tmp;
	}
	return data;
}

char* genHc(char* data){
	int m=strlen(data),r=1,i=0,j=0,k=0,cnt=0,shft=0;
	strcpy(data,revStr(data));
	while(1){
		if((1<<r)>=(m+r+1)){
			break;
		}
		r++;
	}
	char* hammingCode=(char*)calloc((m+r+1),sizeof(char));
	hammingCode[m+r]='\0';
	for(i=0;i<m+r;i++){
		if((i+1)& i){
			hammingCode[i]=data[k++];
		}
		else{
			hammingCode[i]='r';
		}
	}
	for(i=0;i<r;i++){
		cnt=0;
		shft=1<<i;
		for(j=1;j<=(m+r);j++){	
			if(j&shft){
				if(hammingCode[j-1]=='1')
					cnt++;
			}
		}
		hammingCode[shft-1]='0'+(cnt&1);
	}
	strcpy(hammingCode,revStr(hammingCode));
	return hammingCode;
}

int main(int argc,char* argv[]){
	char ipaddr[32],data[64],hammingCode[129];
	struct sockaddr_in saddr,caddr;
	if(argc==1){
		strcpy(ipaddr,"127.0.0.1");
	}
	else{
		strcpy(ipaddr,argv[1]);
	}
	int ss=socket(AF_INET,SOCK_DGRAM,0);
	int l=0;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(1234);
	inet_aton(ipaddr,&(saddr.sin_addr));
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));
	while(1){
		printf("\nWaiting for data from client\n");
		l=sizeof(caddr);
		recvfrom(ss,(void*)data,sizeof(data),0,(struct sockaddr*)&caddr,&l);
		printf("Received dataword from client: %s\n",data);
		if(strcasecmp(data,"exit")==0){
			printf("Exiting!\n");
			break;
		}
		strcpy(hammingCode,genHc(data));
		printf("Generated Hamming Code: %s\n",hammingCode);
		sendto(ss,(void*)hammingCode,strlen(hammingCode)+1,0,(struct sockaddr*)&caddr,sizeof(caddr));
	}
	close(ss);
	return 0;
}
