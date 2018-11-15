#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024


int main(int argc, char *argv[]){
	
	int client_socket;
	struct sockaddr_in server_addr;
	char buffer[BUFF_SIZE], order_num='2';

	FILE* fp;
	char store_filename[BUFF_SIZE];
	char filename[BUFF_SIZE];
	
	char file_path[BUFF_SIZE];

	strcpy(store_filename,argv[1]);
	strcpy(filename, argv[2]);

	//socket()
	client_socket=socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket == -1){
			printf("socket() fail\n");
			exit(1);
	}
	else 
			printf("socket() OK\n");

	//connect()
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;//일반 인터넷 용
	server_addr.sin_port = htons(4000);
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))==-1){
			printf("connect() fail \n");
			exit(1);
	}
	else
			printf("connect() ok \n");

	write( client_socket,  &order_num, sizeof(char));   
	write( client_socket, store_filename, strlen(store_filename));  

  	read ( client_socket, buffer, BUFF_SIZE);
 	
	if(buffer=='0'){
		printf("no file\n");
		exit(1);
	}

	
	fp=fopen(filename,"wb");
	

	while(1){

		read ( client_socket, buffer, BUFF_SIZE);
		
		if(strlen(buffer)<BUFF_SIZE){
			fwrite( buffer, sizeof(char), strlen(buffer),fp);
			break;
		}
		else
			fwrite( buffer, sizeof(char), BUFF_SIZE,fp);
	}

	
	fclose(fp);
	close(client_socket);
}
