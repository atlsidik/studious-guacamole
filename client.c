/***************client.c***************
 * ********simple client program to connect to server on 
 * host speciified on comman line ****/ 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#define PORT "3490"
#define MAXDATASIZE 100

void * get_addr(struct sockaddr * sa){
	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in *) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *) sa)->sin6_addr);

}

int main(int argc , char *argv[]){
	struct addrinfo hints , *res, *p;
	int stat, rv,sockfd,num_bytes;
	char buf[MAXDATASIZE], addr_str[INET6_ADDRSTRLEN];

 
	if(argc != 2){
		fprintf(stderr,"usage : client hostname \n");
		exit(1);
	}

	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
		
	if((rv = getaddrinfo(argv[1],PORT,&hints,&res)) ==-1  ){
		fprintf(stderr,"getasddrinfo failed : %s \n", gai_strerror(rv));
		exit(1);
	}

	for(p = res ; res != NULL ; p = p->ai_next){
		
		if((sockfd = socket(p->ai_family, p->ai_socktype , p->ai_protocol))==-1){
			perror("client  : socket fail \n");
			continue;
		}

		inet_ntop(p->ai_family, get_addr((struct sockaddr *) p->ai_addr), addr_str,sizeof (addr_str));
		
		printf("client : attempting to connect to %s\n", addr_str);
		
		if(connect(sockfd,p->ai_addr,p->ai_addrlen) ==-1){
				perror("client : connect err\n");
				close(sockfd);
				continue;
		}
		break;//successful connection
	}

	if(p == NULL){
		fprintf(stderr,"client : unable to establidh connection\n");
		exit(1);
	}else 
		printf("client : connected to server %s \n",addr_str);
	

	freeaddrinfo(res);

	if((num_bytes = recv(sockfd,buf,MAXDATASIZE-1,0)) ==-1){
		perror("server : recv error \n");
		exit(1);
	}

	buf[num_bytes] ='\0';
	printf("client : received '%s'\n",buf);

	close(sockfd);

	return 0;


}
