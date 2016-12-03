#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _SEM_SEMUN_UNDEFINED
union semun{
  int val;
  struct semid_ds*buf;
  unsigned short*array;
#if defined(__linux__)
  struct seminfo* __buf;
#endif
};
#endif

void V(int semid,int index)
{
  struct sembuf sem;
  sem.sem_num=index;
  sem.sem_op=1;
  sem.sem_flg=0;
  semop(semid,&sem,1);
}
void P(int semid,int index)
{
  struct sembuf sem;
  sem.sem_num=index;
  sem.sem_op=-1;
  sem.sem_flg=0;
  semop(semid,&sem,1);
}

pid_t p1;//写缓冲区进程
pid_t p2;//读缓冲区进程
int main(int argc,char *argv[])
{
  char*filename1="out.txt",*filename2="in.txt";
  //信号量定义
  union semun semopts;
  int s_full,s_empty;
  FILE *f1,*f2;
  s_full=semget(IPC_PRIVATE,4,IPC_CREAT | 0666);//缓冲区是否为满
  semopts.val=0;
  semctl(s_full,0,SETVAL,semopts);
  semctl(s_full,1,SETVAL,semopts);
  semctl(s_full,2,SETVAL,semopts);
  semctl(s_full,3,SETVAL,semopts);

  s_empty=semget(IPC_PRIVATE,4,IPC_CREAT | 0666);//缓冲区是否为空
  semopts.val=1;
  semctl(s_empty,0,SETVAL,semopts);
  semctl(s_empty,1,SETVAL,semopts);
  semctl(s_empty,2,SETVAL,semopts);
  semctl(s_empty,3,SETVAL,semopts);

  //共享缓冲区创建
  int buf1=shmget(0,20,IPC_CREAT|0666);
  int buf2=shmget(1,20,IPC_CREAT|0666);
  int buf3=shmget(2,20,IPC_CREAT|0666);
  int buf4=shmget(3,20,IPC_CREAT|0666);
  char*cbuf[4];
  cbuf[0]=(char *)shmat(buf1,NULL,SHM_R|SHM_W);
  cbuf[1]=(char *)shmat(buf2,NULL,SHM_R|SHM_W);
  cbuf[2]=(char *)shmat(buf3,NULL,SHM_R|SHM_W);
  cbuf[3]=(char *)shmat(buf4,NULL,SHM_R|SHM_W);

  if(argc<3)
  {
    f1=fopen(filename2,"w");
    f2=fopen(filename1,"r");
  }
  else
  {
    f1=fopen(argv[2],"w");
    f2=fopen(argv[1],"r");
  }

  p1=fork();
  if(p1==0)
  {
    int i =0,j=0,num=0;
    if(f1==NULL)
    {
      printf("File cannot be opened/n");  
      exit(0); 
    }
    while(1)
    { 
      for(i=0;i<4;i=(i==3)?0:(i+1))
      {
        P(s_full,i);
        for(j=0;j<20;j++)
        { 
          printf("put word%d\n",++num);
          if(cbuf[i][j]!=-1)
          fputc(cbuf[i][j],f1);
          else 
          {
            printf("write over!\n");
            exit(0);
          }
        }
        V(s_empty,i);
      }
    }  
  }
  p2=fork();
  if(p2==0)
  {
     int i =0,j=0,num=0;
    if(f2==NULL)
    {
      printf("File cannot be opened/n");  
      exit(0); 
    }
    while(1)
    { 
      for(i=0;i<4;i=(i==3)?0:(i+1))
      {
        P(s_empty,i);
        for(j=0;j<20;j++)
        { 
          cbuf[i][j]=fgetc(f2);
          printf("get word%d\n",++num);
          if(cbuf[i][j]==EOF)
          {
            printf("read over!\n");
            V(s_full,i);
            exit(0);
          }
        }
        V(s_full,i);
      }
    }  
  }
  wait(NULL);
  wait(NULL);
  fclose(f1);
  fclose(f2);
  shmctl(buf1,IPC_RMID,0);
  printf("共享缓冲区1释放！\n");
  shmctl(buf2,IPC_RMID,0);
  printf("共享缓冲区2释放！\n");
  shmctl(buf3,IPC_RMID,0);
  printf("共享缓冲区3释放！\n");
  shmctl(buf4,IPC_RMID,0);
  printf("共享缓冲区4释放！\n");
  semctl(s_empty,4,IPC_RMID);
  semctl(s_full,4,IPC_RMID);
  printf("Parent Process is killed!\n");
  exit(0);
}
