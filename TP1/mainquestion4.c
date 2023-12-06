#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define INTRODUCTION_MESSAGE	"$ ./enseash\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash %"
#define EXIT_MESSAGE			"Bye bye...\n$\n"
#define ENSEA					"\nenseash % "

int main(int argc, char **argv){
	char command[128];
	int nbbytes;
	write(1,INTRODUCTION_MESSAGE,sizeof INTRODUCTION_MESSAGE);												//writing of the introduction message
	while(1){   																							//REPL = read-eval-print-loop
		nbbytes = read(1,command,sizeof command);
		command[strcspn(command, "\n")] = '\0';																//keeping only the command with the right size in the string
		if((strcmp(command,"exit") == 0)||nbbytes==0){
			if(write(1,EXIT_MESSAGE,sizeof EXIT_MESSAGE) == -1){perror("write");exit(EXIT_FAILURE);}		//writing of the exit message
			exit(EXIT_SUCCESS);																				
		}else{
			int pid,status;
			pid = fork();
			if(pid == -1){perror("fork");exit(EXIT_FAILURE);}
			if(pid == 0){
				execlp(command,command, (char *)NULL);														//using execlp to run the shell command
				perror("unknowned or invalid command");
				exit(EXIT_FAILURE);
			}else if(waitpid(pid,&status,0) == -1){ 														//the father process is waiting for the child to finish
				perror("wait");
				exit(EXIT_FAILURE);
			}
		
			command[0] = '\0';																				//emptying the command string
			if (WIFEXITED(status)){																			//checking if child ended correctly
				int exit_status = WTERMSIG(status);
				strcat(command,"\nenseash [exit:");
				write(1,command,strlen(command));
				snprintf(command,sizeof(exit_status),"%d",exit_status);
				strcat(command,"] % ");
				write(1,command,strlen(command));
			}else if(WIFSIGNALED(status)){																	//cheking the signal that killed the child (if there is one)
				int signal_status = WTERMSIG(status);
				strcat(command,"\nenseash [sign:");
				write(STDOUT_FILENO, command,strlen(command));
				snprintf(command,sizeof(signal_status),"%d",signal_status);
				strcat(command, "] % ");
				write(1,command,strlen(command));
			}else{
			write(1,ENSEA,strlen(ENSEA));																	//writing the new prompt message
			}
		}
	}
}
