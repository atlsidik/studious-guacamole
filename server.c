/**********server.c*************/
/******** a small stream socket server that sends out "Hello there !" **********/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>

#define BACKLOG 10
#define PORT "21"

void sigchld_hanndler(int s){
	(void) s;
	int save_errno = errno ;
	while (waitpid(-1,NULL,WNOHANG) >0);

	errno = save_errno;

}

 void * get_in_addr(struct sockaddr *sock_a){
	if(sock_a-> sa_family == AF_INET)
		return &(((struct sockaddr_in *) sock_a) ->sin_addr);

	return &(((struct sockaddr_in6 *) sock_a)->sin6_addr) ;
}

void init_hints (struct addrinfo * hints){
	memset(hints, 0, sizeof (struct addrinfo));
	hints->ai_family =AF_UNSPEC;
	hints->ai_socktype = SOCK_STREAM;
	hints -> ai_flags = AI_PASSIVE;

}



int main (void){
	int sockfd, con_fd;
	struct addrinfo hints , *res , *p, *theirs ;
	struct sockaddr_storage thier_addr;
	socklen_t sin_size;
	struct sigaction sig_a;
	int yes =1;
	char addr_str[INET6_ADDRSTRLEN];
		int rv ;

	
	init_hints(&hints);
	if((rv = getaddrinfo(NULL,PORT,&hints ,&res) )!=0){
		fprintf(stderr, "getaddrinfo failed : %s\n", gai_strerror(rv));
		return 1;
	}

	for(p = res; p!= NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==-1 ){
			perror( "server :socket\n");
			continue;
		}

		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==-1){
			perror("server : setsockopt ; address already in use \n");
			exit(1);
		}

		if(bind(sockfd,p->ai_addr, p->ai_addrlen) ==-1 ){
			close(sockfd);
			perror("server : bind\n");
			continue;
		}
		
		break; //successful bind

	}

	freeaddrinfo(res);
	if (p == NULL){
		fprintf(stderr, "server :  failed to bind socket \n");

	}

	if(listen(sockfd, BACKLOG) ==-1){
		perror("server : listen failed \n");
		exit(1);

	}

	sig_a.sa_handler = sigchld_hanndler;
	sigemptyset(&sig_a.sa_mask);
	sig_a.sa_flags = SA_RESTART;
	 if(sigaction(SIGCHLD,&sig_a,NULL) ==-1){
		perror("sigaction");
		exit(1);
	 }
	

	 printf("server : waiting for incoming connections ...\n");
	

	 while(1){
		 sin_size = sizeof thier_addr;
		 con_fd = accept(sockfd, (struct sockaddr *) &thier_addr, &sin_size);
		 if(con_fd ==-1){
			perror("accept ");
			continue;
		 }

		 inet_ntop(thier_addr.ss_family, get_in_addr((struct sockaddr *)&thier_addr),addr_str, sizeof (addr_str) );

		 printf("server : connnected to client %s\n", addr_str);

		 if(!fork()){
			close(sockfd);
			if(send(con_fd,"Hello there!", 12,0) ==-1)
				perror("send");
			close (con_fd);
			exit(0);
		 }
		 close(con_fd);

	 }

 return 0;


}

