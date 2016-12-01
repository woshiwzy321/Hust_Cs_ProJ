#include <stdio.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fcntl.h>
int l1;//信号灯
int segment_size = 16;//共享内存区的容量
pid_t readbuf,writebuf;//创建两个子进程
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux specific) */
};
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
int main(int argc,char** argv)
{
	union semun arg;
	int segment_id[4]; 
	char* segment_memory1,segment_memory2,segment_memory3,segment_memory4;
	segment_id[0]    = shmget(0,segment_size,IPC_CREAT|0666);//分配共享内存
	segment_id[1]    = shmget(1,segment_size,IPC_CREAT|0666);//分配共享内存
	segment_id[2]    = shmget(2,segment_size,IPC_CREAT|0666);//分配共享内存
	segment_id[3]    = shmget(3,segment_size,IPC_CREAT|0666);//分配共享内存
	segment_memory1  = (char *)shmat(segment_id[0],NULL,SHM_R|SHM_W);
	segment_memory2  = (char *)shmat(segment_id[1],NULL,SHM_R|SHM_W);
	segment_memory3  = (char *)shmat(segment_id[2],NULL,SHM_R|SHM_W);
	segment_memory4  = (char *)shmat(segment_id[3],NULL,SHM_R|SHM_W);
	//printf("The memory address is %p\n",segment_memory);
	l1=semget(221,4,IPC_CREAT|0666);//创建信号灯
	arg.val=1;
	semctl(l1,0,SETVAL,arg);//信号灯赋初值
	semctl(l1,1,SETVAL,arg);
	semctl(l1,2,SETVAL,arg);
	semctl(l1,3,SETVAL,arg);
	readbuf=fork();//创建第一个进程
	if(readbuf==-1)
	{	
		printf("read－process fork error\n");
		exit(-1);
	}
	else if(readbuf==0)
	{		
		int open_it = open(argv[1],O_RDONLY);
		if(open_it==-1)
		{
			printf("file1 open failed!\n");
			exit(-1);
		}
		read()
	}


	writebuf=fork();//创建第二个进程
	if(writebuf==-1)
	{	
		
		printf("write－process fork error\n");
	}
	else if(writebuf==0)
	{		
		char* write_data = open(argv[2],O_RDWR);
	}
	wait(NULL);//等待两个进程结束
	semctl(l1,3,IPC_RMID);
	shmctl(segment_id, IPC_RMID, 0);
	return 0;
}