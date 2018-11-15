
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int main(void){

		int pid[10]={0},i=0,j, PID, PPID, spid;
		time_t t;
		int status;

		PPID=getpid(); // 부모 PID 저장

		for(i=0;i<10;i++){ // 부모프로세스만 자식 10번 생성
				
			if(PPID==getpid()){
					pid[i]=fork();
			}
	
		}

		for(i=0;i<10;i++){	 //자식 프로세스일 때만 100번 출력문 생성	
			
			if(pid[i]==0){
		 
					for(j=0;j<100;j++){
						time(&t);						
						printf("[PID=%d, Count = #%d, PPID = %d] datetime = %s\n", getpid(), j+1, getppid(), ctime(&t));
					
					}
				

					exit(3);// 자식 프로세스 종료
			}

		}

		if(PPID==getpid()){ //부모프로세스일 때
					spid=wait(NULL);
// 자식 프로세스가 끝날 때까지 기다리고 기다린 자식의 PID를 spid에 저장

					printf("Child has finished : PID = %d\n", spid);
 //자식 PID 출력
		} 

	
				


		


		return 0; // 부모 프로세스 종료
}


