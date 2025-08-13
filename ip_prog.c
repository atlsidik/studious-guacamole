
/*******ip_prog.c********
******program to  show IP of any host specifid by name - i think *******/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main (int argc , char * argv[]){
	struct addrinfo hints , *res, *p ;
	int stat;
	char ipstr[INET6_ADDRSTRLEN];

	if(argc != 2){
		fprintf(stderr,"usage : showip hostname \n");
		exit(1);
	}

	memset(&hints , 0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((stat = getaddrinfo(argv[1], NULL, &hints, &res) !=0)){
		fprintf(stderr,"getaddrinfo failed : %s\n",gai_strerror(stat));
		exit(2);
	}
	
	printf("IP addresses for %s\n", argv[1]);

	for(p =res ; p!=NULL; p = p->ai_next){
		void *addr;
		char *ipver;
		struct sockaddr_in *ipv4_addr;
		struct sockaddr_in6 *ipv6_addr;

		if(p->ai_family == AF_INET){
			ipv4_addr = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4_addr->sin_addr);
			ipver ="IPv4";
		}
		else {
				ipv6_addr = (struct sockaddr_in6 * ) p->ai_addr;
				addr = &(ipv6_addr->sin6_addr);
				ipver = "IPv6";
		}

		inet_ntop(p->ai_family,addr,ipstr,sizeof ipstr);
		printf("\t%s:\t%s\n",ipver, ipstr);
	}
	
	freeaddrinfo(res);
}

