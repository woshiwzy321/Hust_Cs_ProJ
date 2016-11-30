#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
pid_t p1,p2;
void chi1kill()
{	
	printf("\nChild Process 1 is Killed by Parent!\n");
	exit(0);
}
void chi2kill()
{
	printf("\nChild Process 2 is Killed by Parent!\n");
		
	exit(0);
}
void parkill()
{	
	kill(p1,SIGUSR1);
	kill(p2,SIGUSR2);
}
int main()
{	

	char* get_ch;
	int pipefd[2];
	pipe(pipefd);//pipefd[0]只用于读，pipefd[1]只用于写
	signal(SIGINT,parkill);//收到信号执行结束程序　SIGINT=ctrl+c
	p1=fork();
	if(p1==-1)
	{	
		
		printf("fork error\n");
	}
	else if(p1==0)
	{		
		int x=1;
		char str_cpy[25];		
		signal(SIGINT,SIG_IGN);		
		signal(SIGUSR1,chi1kill);		
		while(1)
		{		
			sprintf(str_cpy,"I send you %d times\n",x);
			close(pipefd[0]);		  	
			write(pipefd[1],str_cpy,25);
		  	sleep(1);//xiumian yimiao
			x++;
		}
	}
		
	p2=fork();
	if(p2==-1)
	{
		printf("fork error\n");
	}
	else if(p2==0)
	{
		char get_str[25];
		signal(SIGINT,SIG_IGN);		
		signal(SIGUSR2,chi2kill);
		while(1)
		{		
			
			close(pipefd[1]);
		  	read(pipefd[0],get_str,25);
		  	printf("%s",get_str);
			sleep(1);
			
		}
	}
	wait(NULL);//	wait(NULL);
//	printf("%d",get_end);
		printf("Parent killed!\n");	
	return 0;	
}

