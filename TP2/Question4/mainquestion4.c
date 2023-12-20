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
#include <arpa/inet.h>

#define GET "gettftp"
#define ERROR_MESSAGE_NB_OF_ARGUMENTS "There is the wrong number of arguments."
#define PORT "69"
#define MODE "octet"
#define RETURN "\n"
#define SUCCES_SENDTO "Bravo, la RRQ à bien été envoyé"
#define OPCODE 0x01
#define ZERO 0
#define MODE "octet"
#define CHARSIZE 512
#define ERROR_NB_OF_PACKETS "Paquet de données incorrect reçu."
#define ERROR_OPERATION_CODE "Erreur sur le code d'oppération"

int main(int argc, char **argv){
	if(argc==4){
		
		//reading the command
		char * command = (char*) malloc(strlen(argv[1]));
		char * host = (char*) malloc(strlen(argv[2]));
		char * file = (char*) malloc(strlen(argv[3]));
		strcpy(command,argv[1]);
		strcpy(host,argv[2]);
		strcpy(file,argv[3]);
		
		//hints enable us to filter for UDP protocol :
		struct addrinfo hints;
		memset(&hints,0,sizeof(struct addrinfo));
		hints.ai_protocol = IPPROTO_UDP;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		
		struct addrinfo *res;
		int err;
		err = getaddrinfo(host,PORT,&hints,&res);
		if (err != 0){
			gai_strerror(err);
			exit(EXIT_FAILURE);
		}
		
		//Creation of a socket with res :
		int socket_communication_point;
		if((socket_communication_point = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
			perror("socket error");
			exit(EXIT_FAILURE);
		}
		
		//command gettftp to get a document from the server :
		if (strcmp(command,GET) ==  0){
			
			//Read Request Protocol : RRQ = opcode (01) + file + 0 + mode + 0:
			char RRQ[CHARSIZE];
			sprintf(RRQ, "%c%s%c%s%c", OPCODE, file, ZERO, MODE, ZERO);		
			sendto(socket_communication_point,RRQ, strlen(RRQ),0, res->ai_addr,res->ai_addrlen);
			write(1,SUCCES_SENDTO,strlen(SUCCES_SENDTO));
			write(1,RETURN,strlen(RETURN));
			
			//Reading of the packets send by the server and writing it in a new file :
			FILE *newfile = fopen(file,"w");
			int block_number = 1;
			while(1){
				char data_packet[CHARSIZE];
				
				//geting the packet from the server
				if(recvfrom(socket_communication_point,data_packet,sizeof(data_packet),0,res->ai_addr,&res->ai_addrlen) ==-1){
					perror("Erreur à recvfrom");
					exit(EXIT_FAILURE);
				}
				
				//Verification of the operation code
				if (data_packet[0] == 0x00 && data_packet[1] == 0x03){
					int received_block_number = (data_packet[2] << 8) | data_packet[3];
					
					//Verification of the number of block recieved
					if (received_block_number == block_number) {
						//Writing of the data in the newfile
						fwrite(data_packet + 4, 1, sizeof(data_packet) - 4, newfile);
						
						//Sending the ACK to the server
						char ack_packet[4];
						ack_packet[0] = 0x00;
						ack_packet[1] = 0x04;
						ack_packet[2] = (block_number >> 8) & 0xFF;
						ack_packet[3] = block_number & 0xFF;
						sendto(socket_communication_point, ack_packet, sizeof(ack_packet), 0, res->ai_addr,res->ai_addrlen);
						block_number++;
					} else {
						
						//Recieving of an incorect block :
						write(1,ERROR_NB_OF_PACKETS,strlen(ERROR_NB_OF_PACKETS));
						write(1,RETURN,strlen(RETURN));
						exit(EXIT_FAILURE);
					}
				} else {
					//Operation code incorrect
					write(1,ERROR_OPERATION_CODE,strlen(ERROR_OPERATION_CODE));
					write(1,RETURN,strlen(RETURN));
					exit(EXIT_FAILURE);
				}
				
				//Ending the while by verifying if the size of the data_packets is inferior to the actual size of the buffer
				if (sizeof(data_packet) - 4 < CHARSIZE) {
					break;
				}
			}
			
			fclose(newfile);
		}
		
		close(socket_communication_point);
		freeaddrinfo(res);
		
	}else{
		write(1,ERROR_MESSAGE_NB_OF_ARGUMENTS,strlen(ERROR_MESSAGE_NB_OF_ARGUMENTS));
		write(1,RETURN,strlen(RETURN));
		exit(EXIT_FAILURE);
	}
}
