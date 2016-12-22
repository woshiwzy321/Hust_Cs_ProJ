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
	int l1,l2;//信号灯
	int segment_size = 16;//共享内存区的容量
	pid_t readbuf,writebuf;//创建两个子进程
	FILE* read_file;
	FILE* write_file;
	union semun arg;
	int segment_id[4]; 
	char* segment_memory[4];//四个缓冲区的读入指针
	l1=semget(221,4,IPC_CREAT|0666);//创建是否为空的信号灯检测
	arg.val=0;
	semctl(l1,0,SETVAL,arg);//信号灯赋初值
	semctl(l1,1,SETVAL,arg);
	semctl(l1,2,SETVAL,arg);
	semctl(l1,3,SETVAL,arg);
	l2=semget(223,4,IPC_CREAT|0666);//创建是否为满的信号灯检测
	arg.val=1;
	semctl(l2,0,SETVAL,arg);//信号灯赋初值
	semctl(l2,1,SETVAL,arg);
	semctl(l2,2,SETVAL,arg);
	semctl(l2,3,SETVAL,arg);	
	segment_id[0]    = shmget(0,segment_size,IPC_CREAT|0666);//分配共享内存
	segment_id[1]    = shmget(1,segment_size,IPC_CREAT|0666);//分配共享内存
	segment_id[2]    = shmget(2,segment_size,IPC_CREAT|0666);//分配共享内存
	segment_id[3]    = shmget(3,segment_size,IPC_CREAT|0666);//分配共享内存
	segment_memory[0]  = (char *)shmat(segment_id[0],NULL,SHM_R|SHM_W);
	segment_memory[1]  = (char *)shmat(segment_id[1],NULL,SHM_R|SHM_W);
	segment_memory[2] = (char *)shmat(segment_id[2],NULL,SHM_R|SHM_W);
	segment_memory[3] = (char *)shmat(segment_id[3],NULL,SHM_R|SHM_W);
	//printf("The memory address is %p\n",segment_memory);
	//打开两个文件
	read_file=fopen(argv[1],"r");//文件1用来读入
	write_file=fopen(argv[2],"w");//文件2用来写入	
	readbuf=fork();//创建第一个进程用来读
	if(readbuf==-1)
	{	
		printf("read－process fork error\n");
		exit(-1);
	}
	else if(readbuf==0)
	{		
		if(read_file==NULL)
		{
			printf("read_file cannot opened!/n");
			exit(0);
		}
		while (1)
		{
			//对四个缓冲区进行循环写入查询
			for(int i=0;i<4;i++)
			{
				//printf("%d\n",i);
				P(l2,i);
				for(int j=0;j<16;j++)
				{
					segment_memory[i][j]=fgetc(read_file);
					//printf("%s    %d  %d\n",segment_memory[i],i,j);
					if(segment_memory[i][j]==EOF)//判断是否到达文件结束  prbo
					{
						printf("finish read\n");
						V(l1,i);
						exit(0);
					}
				}
				V(l1,i);
				printf("%d\n",i);
				if(i==3)//适配以满足循环
				{
					i=-1;
				}
			}
		}
	}


	writebuf=fork();//创建第二个进程用来写入
	if(writebuf==-1)
	{	
		
		printf("write_process fork error\n");
	}
	else if(writebuf==0)
	{		
		if(write_file==NULL)
		{
			printf("write file open error\n ");
			exit(0);
		}
		while(1)
		{
			for(int i2=0;i2<4;i2++)
			{
				P(l1,i2);
				//printf("ddd %d\n",i2);
				for(int j2=0;j2<16;j2++)
				{
				//	printf("ddd 123123%d\n",i2);
					
					if(segment_memory[i2][j2]!=EOF)
					{  
						fputc(segment_memory[i2][j2],write_file);            
					//	printf("现在写入的是%c\n",segment_memory[i2][j2]);
					}
						//fputc(segment_memory[i2][j2],write_file);
					else
					{
						printf("write finish!\n");
						exit (0);
					}
				}
				V(l2,i2);
				if(i2==3)
				{
					i2=-1;
				}
				
			}
		}
	}
	wait(NULL);//等待两个进程结束
	wait(NULL);//等待两个进程结束
	fclose(write_file);
	fclose(read_file);
	semctl(l1,3,IPC_RMID);
	printf("开始释放缓冲区 从0-3\n");
	for(int i=0;i<4;i++)
	{
		shmctl(segment_id[i], IPC_RMID, 0);
		printf("缓冲区%d 已经被释放!\n",i);
	}
	
		
	printf("程序执行完毕，释放主进程！\n");	
	return 0;
}