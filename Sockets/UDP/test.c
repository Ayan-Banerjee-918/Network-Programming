#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
static void list_interfaces(int fd) {

    struct ifreq *ifreq,*inf;
    struct ifconf ifconf;
    int i=0;
	char buf[1024];
    size_t len;
 
    ifconf.ifc_len = sizeof(buf);

    if (ioctl(fd, SIOCGIFCONF, &ifconf) != 0)
    {
        perror("ioctl(SIOCGIFCONF)");
        exit(EXIT_FAILURE);
    }
 
    printf("Listing all interfaces:\n");
    ifreq = ifconf.ifc_req;
    int cnt = ifconf.ifc_len/sizeof(struct ifreq);
    int flag=0;
    while(flag==0)
    {
        inf = &ifreq[i++];
	if(!strcasecmp(inf->ifr_name,"lo")){
		continue;
	}
    /* Show the device name and IP address */
        printf("%s: IP %s",
               inf->ifr_name,
               inet_ntoa(((struct sockaddr_in *)&inf->ifr_addr)->sin_addr));


    /* Get the broadcast address (added by Eric) */
        if(ioctl(fd, SIOCGIFBRDADDR, inf) >= 0){
            printf(", BROADCAST %s", inet_ntoa(((struct sockaddr_in *)&inf->ifr_broadaddr)->sin_addr));
            flag++;
	}printf("\n");
	
    }
}
 
int main(int argc, char** argv) {

    int sock;
    struct ifreq ifreq;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    list_interfaces(sock);

    close(sock);

    return EXIT_SUCCESS;
}