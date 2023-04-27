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
	syn_pack.seqNo=2000;
	ack_pack.seqNo=0;
	td.SYN=syn_pack;
	td.ACK=ack_pack;
	return td;
}

int perform_handshake(tcp_data* hndshk,int ts){
	tcp_data incoming_data;
	printf("\nINITIAL DATA\nSYN SeqNo: %d\nACK SeqNo:%d\n",hndshk->SYN.seqNo,hndshk->ACK.seqNo);
	read(ts,&incoming_data,sizeof(incoming_data));
	printf("\nReceived SYN Bit from client with seqNo: %d\n",incoming_data.SYN.seqNo);
	printf("\nReceived ACK Bit from client with seqNo: ");
	if(incoming_data.ACK.seqNo==0){
		printf("UNSET\n");
	}
	else{
		printf("Corrupted Acknowledgement with seqNo %d\nTerminating Handshake Request\n",incoming_data.ACK.seqNo);
		return 0;
	}
	hndshk->ACK.seqNo=incoming_data.SYN.seqNo+1;
	write(ts,hndshk,sizeof(*hndshk));
	printf("\nAcknowledgement-%d Sent\n",hndshk->ACK.seqNo);
	read(ts,&incoming_data,sizeof(incoming_data));
	if(incoming_data.ACK.seqNo==hndshk->SYN.seqNo+1)
		printf("\nReceived ACK Bit from client with seqNo: %d\nTCP Handshaking Completed Successfully\n",incoming_data.ACK.seqNo);
	else{	
		printf("Corrupted Acknowledgement with seqNo %d\nTerminating Handshake Request\n",incoming_data.ACK.seqNo);
		return 0;
	}
	return 1;
}

int main(int argc,char* argv[]){
	tcp_data hndshk=init_tcp();
	int ss=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in saddr,caddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(PORT);
	if(argc==1){
		inet_aton("127.0.0.1",&saddr.sin_addr);
	}
	else{
		inet_aton(argv[1],&saddr.sin_addr);
	}
	bind(ss,(struct sockaddr*)&saddr,sizeof(saddr));
	listen(ss,1);
	int len=sizeof(caddr);
	int ts=accept(ss,(struct sockaddr*)&caddr,&len);
	perform_handshake(&hndshk,ts);
	return 0;
}
