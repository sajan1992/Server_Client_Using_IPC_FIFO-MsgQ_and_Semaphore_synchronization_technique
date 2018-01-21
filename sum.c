#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define FIFO3 "./FIFO3"
#define FIFO4 "./FIFO4"

#include"prototype.h"

Request r1;
Result rs;
int sem_id;
struct sembuf sem_b;
int semaphore_start(int);
int semaphore_release();
int semaphore_wait();
int main()
{
	int pwd,prd,ret,result;
	sem_id = semaphore_start(456);
	// FIFO3 for read purpose
	prd = open(FIFO3,O_RDONLY);
	if(prd == -1)
	{
		perror("\e[41mOPEN\e[0m");
		exit(EXIT_FAILURE);
	}
	
	read(prd,&r1,sizeof(Request));
	close(prd);
	printf("\e[44m%s\e[0m : Fetched values successfully:\na = %d\nb = %d\ntype = %d\n",__FILE__,r1.oper1,r1.oper2,r1.type);

	rs.result = r1.oper1 + r1.oper2;
	rs.type = r1.type;
	
	// FIFO4 for read purpose
	pwd = open(FIFO4,O_WRONLY);
	if(pwd == -1)
	{
		perror("\e[41mOPEN\e[0m");
		exit(EXIT_FAILURE);
	}
	semaphore_wait();	
	result = write(pwd,&rs,sizeof(Result));
	close(pwd);
	printf("\e[44m%s\e[0m :Wrote %d bytes into FIFO for PID = %d\n",__FILE__,result,rs.type);
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
	printf("Release\n");
	return 0;
}
