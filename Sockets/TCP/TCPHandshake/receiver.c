#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 1234

typedef struct dat{
	int seqNo;
}packet;

typedef struct req_data{
	packet SYN;
	packet ACK;
}tcp_data;

tcp_data init_tcp(){
	packet syn_pack,ack_pack;
	tcp_data td;
	syn_pack.seqNo=1000;
	ack_pack.seqNo=0;
	td.SYN=syn_pack;
	td.ACK=ack_pack;
	return td;
}

int perform_handshake(tcp_data* hndshk,int cs){
	tcp_data incoming_data;
	printf("\nINITIAL DATA\nSYN SeqNo: %d\nACK SeqNo:%d\n",hndshk->SYN.seqNo,hndshk->ACK.seqNo);
	write(cs,hndshk,sizeof(*hndshk));
	printf("\nSent SYN Bit to server with seqNo: %d\n",hndshk->SYN.seqNo);
	printf("\nACK Bit: UNSET\n");
	read(cs,&incoming_data,sizeof(incoming_data));
	if(incoming_data.ACK.seqNo==hndshk->SYN.seqNo+1)
		printf("\nReceived ACK Bit from server with seqNo: %d\n",incoming_data.ACK.seqNo);
	else{	
		printf("Corrupted Acknowledgement received from server with seqNo %d\nTerminating Handshake Request\n",incoming_data.ACK.seqNo);
		return 0;
	}
	hndshk->ACK.seqNo=incoming_data.SYN.seqNo+1;
	write(cs,hndshk,sizeof(*hndshk));
	printf("\nAcknowledgement-%d sent\n",hndshk->ACK.seqNo);
	return 1;
}

int main(int argc,char* argv[]){
	tcp_data hndshk=init_tcp();
	int cs=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in caddr;
	caddr.sin_family=AF_INET;
	caddr.sin_port=htons(PORT);
	if(argc==1){
		inet_aton("127.0.0.1",&caddr.sin_addr);
	}
	else{
		inet_aton(argv[1],&caddr.sin_addr);
	}
	connect(cs,(struct sockaddr*)&caddr,sizeof(caddr));
	perform_handshake(&hndshk,cs);
	return 0;
}
