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
	int success=connect(cs,(struct sockaddr*)&saddr,sizeof(saddr));
	char data[128],crc[128],divisor[128];
	while(1){
		printf("Enter data word (type 'exit' to exit): ");
		scanf("%s",data);
		write(cs,(void*)data,strlen(data)+1);
		if(strcasecmp(data,"exit")==0){
			printf("Exiting...\n");
			break;
		}
		printf("Enter divisor: ");
		scanf("%s",divisor);
		write(cs,(void*)divisor,strlen(divisor)+1);
		printf("Waiting for data from server\n");
		read(cs,(void*)crc,sizeof(crc));
		strcat(data,crc);
		printf("CRC: %s\nFinal Word:%s\n",crc,data);
	}
	unlink("mysocket");
	close(cs);
	return 0;
}
