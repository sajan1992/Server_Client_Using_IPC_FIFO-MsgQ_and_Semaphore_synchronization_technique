#include"header.h"
#include"prototype.h"
#include"declaration.h"
#define FIFO3 "./FIFO3"
#define FIFO4 "./FIFO4"

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short * array;
};

int sem_id1,sem_id2,sem_id3;
union semun s1;
struct sembuf sem_b;
int semaphore_wait();
int semaphore_release();
int semaphore_start(int);

	Request rr;
	Result rs;
int main()
{
	int ret,ret1,ret2,ret3,ret4,rfork,result,i=0;
	signal(SIGCHLD,SIG_IGN);

	sem_id1 = semaphore_start(123);
	s1.val = 1;
	semctl(sem_id1,0,SETVAL,s1);
	
	sem_id2 = semaphore_start(456);
	semctl(sem_id2,0,SETVAL,s1);
	
	printf("\e[44m%s\e[0m : Initiating Server.\n",__FILE__);
	
	ret1 = msgget((key_t)13, 0666|IPC_CREAT);
	if(ret1 == -1)
	{
		perror("ret1 failed");
		exit(EXIT_FAILURE);
	}
	printf("MsgQ1 successfully created!\n");
	
	ret2 = msgget((key_t)14, 0666|IPC_CREAT);
	if(ret2 == -1)
	{
		perror("ret2 failed");
		exit(EXIT_FAILURE);
	}
	printf("MsgQ2 successfully created!\n");
	
	//FIFO3 for write purpose
	if(access(FIFO3,F_OK) == 0)
	system("unlink FIF03");
	else
	{
		ret = mkfifo(FIFO3,666);
		if(ret == -1)
		{
			perror("\e[41mFIFO3\e[0m");
			exit(EXIT_FAILURE);
		}
	}
			
			
	//FIFO4 for read purpose
	if(access(FIFO4,F_OK) == 0)
	system("unlink FIF04");
	else
	{
		ret = mkfifo(FIFO4,666);
		if(ret == -1)
		{
			perror("\e[41mFIFO4\e[0m");
			exit(EXIT_FAILURE);
		}
	}
			
	printf("\e[44m%s\e[0m : \e[38mWaiting for Requesting Client.\e[0m\n",__FILE__);
	while(1)
	while(msgrcv(ret1,&rr,sizeof(Request)-sizeof(long),0,0))
	{
	printf("\e[44m%s\e[0m : Request recieved \n[ PROCESSING.. ]\n",__FILE__);
	switch(rr.oper)
	{
		case '+':
			printf("---Inside process switch---\n");
			rfork = fork();
			if(rfork == 0)
				execl("./sum","sum.c",NULL);
			if(rfork == -1)
			{
				perror("\e[41mfork\e[0m");
				exit(EXIT_FAILURE);
			}
			else
			{
			ret3 = open(FIFO3,O_WRONLY);
			if(ret3 == -1)
			{
				perror("\e[41mOPEN\e[0m");
				exit(EXIT_FAILURE);
			}
			printf("\e[42mCONNECTION ESTABLISHED SUCCESSFULLY [ WRONLY ]\e[0m\n");
			write(ret3,&rr,sizeof(Request));
			close(ret3);
			break;
			}

		case '-':
			printf("---Inside process switch---\n");
			rfork = fork();
			if(rfork == 0)
				execl("./sub","sub.c",NULL);
			if(rfork == -1)
			{
				perror("\e[41mfork\e[0m");
				exit(EXIT_FAILURE);
			}
			else
			{
			ret3 = open(FIFO3,O_WRONLY);
			if(ret3 == -1)
			{
				perror("\e[41mOPEN\e[0m");
				exit(EXIT_FAILURE);
			}
			printf("\e[42mCONNECTION ESTABLISHED SUCCESSFULLY [ WRONLY ]\e[0m\n");
			write(ret3,&rr,sizeof(Request));	
			break;
			}
		case '*':
			printf("---Inside process switch---\n");
			rfork = fork();
			if(rfork == 0)
				execl("./mul","mul.c",NULL);
			if(rfork == -1)
			{
				perror("\e[41mfork\e[0m");
				exit(EXIT_FAILURE);
			}
			else
			{
			ret3 = open(FIFO3,O_WRONLY);
			if(ret3 == -1)
			{
				perror("\e[41mOPEN\e[0m");
				exit(EXIT_FAILURE);
			}
			printf("\e[42mCONNECTION ESTABLISHED SUCCESSFULLY [ WRONLY ]\e[0m\n");
			write(ret3,&rr,sizeof(Request));
			break;
			}
		case '/':
			printf("---Inside process switch---\n");
			rfork = fork();
			if(rfork == 0)
				execl("./div","div.c",NULL);
			if(rfork == -1)
			{
				perror("\e[41mfork\e[0m");
				exit(EXIT_FAILURE);
			}
			else
			{
				ret3 = open(FIFO3,O_WRONLY);
				if(ret3 == -1)
				{
					perror("\e[41mOPEN\e[0m");
					exit(EXIT_FAILURE);
				}	
				printf("\e[42mCONNECTION ESTABLISHED SUCCESSFULLY [ WRONLY ]\e[0m\n");	
				write(ret3,&rr,sizeof(Request));
				break;
			}
		default :
			printf("\e[41mnot found\e[0m\n");
	}
	ret4 = open(FIFO4,O_RDONLY);
	if(ret4 == -1)
	{
		perror("\e[41mOPEN\e[0m");
		exit(EXIT_FAILURE);
	}
	printf("\e[42mCONNECTION ESTABLISHED SUCCESSFULLY [ RDONLY ]\e[0m\n");	
	read(ret4,&rs,sizeof(Result));
	close(ret4);
	rs.type =rr.type;
			
	msgsnd(ret2,&rs,(sizeof(Result)-sizeof(long)),0);
	printf("sent signal to %d PID result =%d\n",rs.type,rs.result);
	//FIFO3 for write purpose
	if(access(FIFO3,F_OK) == 0)
	system("unlink FIF03");
	else
	{
		ret = mkfifo(FIFO3,666);
		if(ret == -1)
		{
			perror("\e[41mFIFO3\e[0m");
			exit(EXIT_FAILURE);
		}
	}
			
			
	//FIFO4 for read purpose
	if(access(FIFO4,F_OK) == 0)
	system("unlink FIF04");
	else
	{
		ret = mkfifo(FIFO4,666);
		if(ret == -1)
		{
			perror("\e[41mFIFO4\e[0m");
			exit(EXIT_FAILURE);
		}
	}
			
	}	
	
	printf("\e[44m%s\e[0m : Ends\n",__FILE__);
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
	if(semop(sem_id3,&sem_b,1) == -1)
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
	if(semop(sem_id3,&sem_b,1) == -1)
	{
		perror("Semaphore release failed!");
		return -1;
	}
	printf("Release\n");
	return 0;
}
