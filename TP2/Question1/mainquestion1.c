#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#define RETURN "\n"
#define ERROR_MESSAGE_NB_OF_ARGUMENTS "There is the wrong number of arguments."

int main(int argc, char **argv){
	//verifying if there is the right number of arguments :
	if(argc==4){
		
		//creating the variable to memorise the right value :
		char * command = (char*) malloc(sizeof (argv[1]));
		char * host = (char*) malloc(sizeof (argv[2]));
		char * file = (char*) malloc(sizeof (argv[3]));
		strcpy(command,argv[1]);
		strcpy(host,argv[2]);
		strcpy(file,argv[3]);
		
		//Verifying if the programme work well by printing the variables :
		write(1,command,strlen(command));
		write(1,RETURN,strlen(RETURN));
		write(1,host,strlen(host));
		write(1,RETURN,strlen(RETURN));
		write(1,file,strlen(file));
		write(1,RETURN,strlen(RETURN));
		
	//writing an error message if there is the wrong number of arguments in the main :
	}else{
		write(1,ERROR_MESSAGE_NB_OF_ARGUMENTS,strlen(ERROR_MESSAGE_NB_OF_ARGUMENTS));
		write(1,RETURN,strlen(RETURN));
		exit(EXIT_FAILURE);
	}
}
