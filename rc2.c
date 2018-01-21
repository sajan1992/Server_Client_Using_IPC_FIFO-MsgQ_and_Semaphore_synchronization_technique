#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<signal.h>
#include<string.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include"prototype.h"


int prd;
Result rs;
int PID;
int sem_id;
struct sembuf sem_b;
int semaphore_wait();
int semaphore_release();
int semaphore_start(int);
int main()
{
	int pwd,result=0,ret,recv=0;
	PID = getpid();
	Request r1;
	//FIFO1 for write purpose
	pwd = msgget((key_t)13,0666|IPC_CREAT);
	
	prd = msgget((key_t)14,0666|IPC_CREAT);

	r1.type = getpid();
	r1.oper = '-';
	r1.oper1= 12;
	r1.oper2= 34;
	sem_id = semaphore_start(123);
	
	
	semaphore_wait();
	
	msgsnd(pwd,&r1,sizeof(Request)-sizeof(long),0);
	

	msgrcv(prd,&rs,(sizeof(Result)-sizeof(long)),PID,0);
	
	printf("\e[42m%s :\e[0m\e[44m Result recieved successfully (Sub->Result) = %d\e[0m \n",__FILE__,rs.result);

	semaphore_release();
	
	return 0;

}

int semaphore_start(int id)
{
	return semget((key_t)id,1,666|IPC_CREAT);
}
int semaphore_wait()
{
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id,&sem_b,1) == -1)
	{
		perror("Semaphore wait failed!");
		return -1;
	}
	printf("Wait\n");
	return 0;
}
int semaphore_release()
{
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id,&sem_b,1) == -1)
	{
		perror("Semaphore release failed!");
		return -1;
	}
	printf("Released\n");
	return 0;
}
