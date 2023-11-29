#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define INTRODUCTION_MESSAGE	"$ ./enseash\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash %"

int main(int argc, char **argv){
	char command[128];
	write(1,INTRODUCTION_MESSAGE,sizeof INTRODUCTION_MESSAGE);		//writing of the introduction message
	while(1){   													// REPL = read-eval-print-loop
		read(1,command,sizeof command);
		command[strcspn(command, "\n")] = '\0';						//keeping only the command with the right size in the string
		int pid;
		pid = fork();
		if(pid == -1){perror("fork");exit(EXIT_FAILURE);}
		if(pid == 0){
			execlp(command,command, (char *)NULL);					//using execlp to run the shell command
			perror("execl");
			exit(EXIT_FAILURE);
		}
	}
}
