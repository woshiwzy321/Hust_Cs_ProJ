#include <stdio.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux specific) */
};
int a;
int l1;
int j;
void V(int semid,int index)
{	
	struct sembuf sem;	
	sem.sem_num = index;
	sem.sem_op =  1;
      	sem.sem_flg = 0;	
      	semop(semid,&sem,1);	
      	return;
}
void P(int semid,int index)
{	 
	struct sembuf sem;	
        sem.sem_num = index;
        sem.sem_op = -1;	
        sem.sem_flg = 0; //操作标记：0或IPC_NOWAIT等
        semop(semid,&sem,1);	//1:表示执行命令的个数	
        return;
}
void* subp1()
{
        for(j=1;j<=100;j++)
        {
                P(l1,0);
                a=a+j;
		V(l1,1);
        }
}
void* subp2()
{
        for(int i=0;i<100;i++)
        {
              P(l1,1);
	      printf("现在的和是%d\n",a);
              V(l1,0);
	}
}

int main()
{
	union semun arg;
	int light1,light2;
	pthread_t p1,p2;
	int proc1,proc2;
	l1=semget(221,2,IPC_CREAT|0666);
	arg.val=1;
	semctl(l1,0,SETVAL,arg);	
	arg.val=0;
	semctl(l1,1,SETVAL,arg);
	proc1=pthread_create(&p1,NULL,&subp1,NULL);
	proc2=pthread_create(&p2,NULL,&subp2,NULL);
	if(proc1!=0||proc2!=0)
	{
		printf("Create pthread_child faulure\n");
		exit(1);
	}
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	semctl(l1,1,IPC_RMID);
	return 0;				
}







