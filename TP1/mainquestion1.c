#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define INTRODUCTION_MESSAGE	"$ ./enseash\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash %"

int main(){
	write(1,INTRODUCTION_MESSAGE,sizeof INTRODUCTION_MESSAGE); 		//writing of the introduction message
}
