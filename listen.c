/*******************listen.c************
 * **datagram socket listening program ******/ 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>


#define PORT "4950"
#define MAXBUFLEN 100

void * get_addr(struct sockaddr * sa){
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *)sa)->sin_addr);

	return &(((struct sockaddr_in6 *)sa) ->sin6_addr);
}

int main(void){
	int sockfd ,rv,num_bytes;
	struct addrinfo hints, *res, *p;
	char buf[100], addr_str[INET6_ADDRSTRLEN];
	socklen_t addrlen;
	struct sockaddr_storage theirs;
	
		memset(&hints,0,sizeof hints);
		hints.ai_family = AF_INET6;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;

		if((rv = getaddrinfo(NULL,PORT,&hints,&res))==-1 ){
			fprintf(stderr,"getaddrinfo fialed : %s\n",gai_strerror(rv));
			exit(1);
		}

		for(p = res; p !=NULL; p = p->ai_next){
				if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) ) ==-1){
					perror("  listener :socket ");	
					continue;
				}
					
				if((bind(sockfd , p->ai_addr, p->ai_addrlen) ==-1)){
					close(sockfd);
					perror("listener : bind ");
					continue;
				}

				break;
		}
		freeaddrinfo(res);
		if(p == NULL){
			fprintf(stderr," listener :failed to bind to any socket \n");
			exit(1);
		}

		printf("waiting to recvfrom...\n");

		addrlen = sizeof theirs;
		if((num_bytes = recvfrom(sockfd,buf,MAXBUFLEN-1,0,(struct sockaddr *)&theirs,&addrlen) ) == -1){
			perror("listener :recvfrom");
			exit(1);
		}

		printf("listener : got packet from %s",inet_ntop(theirs.ss_family, get_addr((struct sockaddr *) &theirs),addr_str, sizeof addr_str));
		buf[num_bytes]='\0';
		printf("packet length : %d\n",num_bytes);
		printf("listener : message contains :\n\t\"%s\"\n",buf);
		close(sockfd);

		return 0;
}
