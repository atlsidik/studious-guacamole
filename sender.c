/**************		sender.c		***************
 ******* datagram socket program to ********************
 **send message speicified on cmd line  to listener program********/ 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define expand(x)(printf(x))

#define PORT "4950"
void * get_addr(struct sockaddr * sa){
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *)sa)->sin_addr);

	return &(((struct sockaddr_in6 *)sa)-> sin6_addr);
}

int main (int argc, char *argv[]){
	int rv,sockfd,num_bytes ;
	struct addrinfo hints, *res, *p;
	char addr_str[INET6_ADDRSTRLEN];

	if (argc != 3){
		fprintf(stderr,"usage : sender <hostname> <message> \n");
		exit(1);
	}

	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if((rv = getaddrinfo(argv[1],PORT,&hints,&res)) ==-1){
		fprintf(stderr, "sender : getaddrinfo failed : %s\n", gai_strerror(rv));
		exit(1);
	}

	for(p =res ; p != NULL ; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==-1){
			perror("sender : socket fail");
			continue;
		}

		break;
	}
	if(p == NULL){
		fprintf(stderr, "sender : failed to bind \n");
	perror("sender : socket fail");
		exit(1);
	}


	if((num_bytes = sendto(sockfd,argv[2],strlen(argv[2]),0,p->ai_addr,p->ai_addrlen) ) ==-1){
		perror("sender : sendto fail");
		exit(1);
	}
	
	printf("sender : sent %d bytes to %s on %s\n", num_bytes,argv[1],
			inet_ntop(p->ai_family,get_addr((struct sockaddr *) p->ai_addr),addr_str,p->ai_addrlen));

	freeaddrinfo(res);
	close(sockfd);
	return 0;
}
