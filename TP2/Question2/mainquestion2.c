#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netdb.h>

#define RETURN "\n"
#define SPACE " "
#define ERROR_MESSAGE_NB_OF_ARGUMENTS "There is the wrong number of arguments."

int main(int argc, char **argv){
	if(argc==4){
		
		//reading the command
		char * command = (char*) malloc(sizeof (argv[1]));
		char * host = (char*) malloc(sizeof (argv[2]));
		char * file = (char*) malloc(sizeof (argv[3]));
		strcpy(command,argv[1]);
		strcpy(host,argv[2]);
		strcpy(file,argv[3]);
		
		//hints enable us to filter for UDP protocol
		struct addrinfo hints;
		memset(&hints,0,sizeof(struct addrinfo));
		hints.ai_protocol = IPPROTO_UDP;
		struct addrinfo *res;
		int err;
		err = getaddrinfo(host,NULL,&hints,&res);
		if (err != 0){
			gai_strerror(err);
			exit(EXIT_FAILURE);
		}
		
	}else{
		write(1,ERROR_MESSAGE_NB_OF_ARGUMENTS,strlen(ERROR_MESSAGE_NB_OF_ARGUMENTS));
		write(1,RETURN,strlen(RETURN));
		exit(EXIT_FAILURE);
	}
}
