#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024


int main(void){
	
	int client_socket;
	struct sockaddr_in server_addr;
	char buffer[BUFF_SIZE], order_num='1';

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

	
	write( client_socket, &order_num , sizeof(char));      	
	
	while(1){
		read ( client_socket, buffer, BUFF_SIZE);
	
		if(strlen(buffer)==0){
			break;	
	
		printf( "%s\n", buffer);
		
		buffer=NULL;
	}
	


	close(client_socket);
}
