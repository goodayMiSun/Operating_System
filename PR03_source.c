#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/unistd.h>

#define BUFF_SIZE 1024
void forstore();
void forlist();
void forload();

int server_socket;
int client_addr_size;
int client_socket;


char order_num;
char buff_rcv[BUFF_SIZE+5];

void *t_function()
{
	switch(order_num){
	
	case 0:
		forstore();
		break;
	case 1:
		forlist();
		break;
	case 2: 
		forload();
		break;
	}
		
}	

void forstore(){
	
	char store_filename[BUFF_SIZE];
	char buffer[BUFF_SIZE];
	FILE *fp;
	
	read ( client_socket, store_filename, BUFF_SIZE);

	
	fp=fopen(store_filename, "wb");
	
	while(1){
		read ( client_socket, buffer, BUFF_SIZE);
		
		if(strlen(buffer) < BUFF_SIZE){
				fwrite( buffer, sizeof(char), strlen(buffer),fp);
				break;
		}

		fwrite( buffer, sizeof(char), strlen(buffer),fp);

	}

	fclose(fp);
	

	


}

void forlist(){

	char buffer[BUFF_SIZE];

	while(1){
		
		write ( client_socket, buffer, BUFF_SIZE);
	
		if(strlen(buffer)==0)
			break;	
	
		printf( "%s\n", buffer);
	}

}

void forload(){

	char store_filename[BUFF_SIZE];
	char buffer[BUFF_SIZE];
	FILE *fp;
	char b;
	int file_len;

	read( client_socket, store_filename, strlen(store_filename)); 
 
	fp=fopen(store_filename,"rb");
	
	if(fp==NULL){
		b='0';
  		write ( client_socket, &buffer, sizeof(char));
		exit(1);
 	
	}
	
	while(1){

		file_len=fread(buffer,sizeof(char), BUFF_SIZE,fp);
		
		
		
		if(file_len<BUFF_SIZE){
			write ( client_socket, buffer, file_len);
			break;
		}
		else
			write ( client_socket, buffer, file_len);
	}

}




void main(){
	


	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;


	
	//thread part
	pthread_t p_thread;
	int pid;
	int status;
	
	

	//socket()
	server_socket=socket(PF_INET,SOCK_STREAM,0);
	if(server_socket == -1){

			printf("server socket fail");
			exit(1);
	}
	else 
			printf("socket() OK\n");

	//bind()
	memset(&server_addr, 0, sizeof(server_addr));// 초기화
	server_addr.sin_family = AF_INET;//IPv4 인터넷 프로토콜
	server_addr.sin_port = htons(4000); // 사용할 포트 번호 4000
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 32bit IPV4주소

	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))){
		printf("bind fail\n");
		exit(1);
	}
	else
			printf("bind() OK\n");

	//listen()
	while(1){
			if(listen(server_socket,5) == -1){
			printf("listen() OK\n");
			break;
		}
	


	//accept()
	client_addr_size = sizeof(client_addr);
	client_socket=accept(server_socket,(struct sockaddr*)&client_addr, &client_addr_size);

	if(client_socket == -1){
		printf("client connect fail\n");
		exit(1);
	}
	else
		printf("client connect \n");


	
	
	//thread create
	pid = pthread_create(&p_thread, NULL, t_function, NULL);

	if(pid < 0){
			perror("thread create error");
			exit(0);
	}
	else
			printf("thread create\n");
	
	ptheard_join(p_thread, (void**)&status);

	

	}
}



