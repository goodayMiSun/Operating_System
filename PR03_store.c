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
	
	char order_num='0';

	FILE* fp;
	char client_name[BUFF_SIZE];
	char filename[BUFF_SIZE];
	char store_filename[BUFF_SIZE];
	char buffer[50];
	int file_size, file_len;
	int divid_num, sd;
	
	strcpy(filename, argv[0]);
	strcpy(store_filename, argv[1]);
	
	divid_num=atoi(argv[2]);
	file_size = 0;
	
	//FILE size
	fp=fopen(filename, "rb");
	fseek(fp,0, SEEK_END);
	file_size= ftell(fp);
	fseek(fp,0,SEEK_SET);
	
	printf("%d", file_size);


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

	write(client_socket, &order_num, sizeof(char));
	write(client_socket, store_filename, BUFF_SIZE);
	while(1){
		file_len = fread((void*) buffer, 1, BUFF_SIZE, fp);
		
		if(file_len < BUFF_SIZE){
				write(client_socket, buffer, file_len);
				break;
		}

		write(client_socket, buffer, BUFF_SIZE);

	}

	fclose(fp);
	close(client_socket);
}
