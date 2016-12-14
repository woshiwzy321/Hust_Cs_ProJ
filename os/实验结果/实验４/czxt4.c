#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
int set_name=0;
int set_filename=0;
//int fstat(int fildes,struct stat *buf);//返回文件描述符相关的文件的状态信息
//int stat(const char *path, struct stat *buf);//通过文件名获取文件信息，并保存在buf所指的结构体stat中
//int lstat(const char *path, struct stat *buf);//如读取到了符号连接，lstat读取符号连接本身的状态信息，而stat读取的是符号连接指向文件的信息。
void printdir(char *dir, int depth);//传入目录名字和深度
void printdir(char *dir, int depth)
{
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if  ((dp = opendir(dir))== NULL)
    {     
        printf("文件目录打开失败！或该目录下无子目录！\n");
        return ;
    }
    chdir(dir);//将dir设置为当前目录; 
    while((entry=readdir(dp))!=NULL)
    {   
        lstat(entry->d_name,&statbuf);//以该目录项的名字为参数,调用lstat得到该目录项的相关信息;
        if(S_ISDIR(statbuf.st_mode))
        {
            if((strcmp(entry->d_name,"..")==0||strcmp(entry->d_name,".")==0))
               	continue;// 跳过该目录项;
            if(set_name==0)
            {
            	printf("/×××××××××××××××××××××××××××××××××目录递归显示××××××××××××××××××××××××××××××××××/\n");
            	printf("读写权限    深度   所有者UID     目录大小   最后修改日期      目录名字  \n");//打印目录项的深度、目录名等信息
            	set_name=1;
            }
            	printf("%c           %d       %d         %ld       %ld        %s\n",entry->d_type,depth,statbuf.st_uid,statbuf.st_size,statbuf.st_atime,entry->d_name);
            
            printdir(entry->d_name,depth+4);//递归调用printdir,打印子目录的信息,其中的depth+4;
        }
        else
	 	{
	 		if(set_filename==0)
	 		{
	 			printf("/×××××××××××××××××××××××××××××××××文件递归显示××××××××××××××××××××××××××××××××××/\n");
            	printf("读写权限    深度   所有者UID     文件大小   最后修改日期      文件名字  \n");//打印文件的深度、文件名等信息
 				set_filename=1;   		
	 		}
	 			printf("%c           %d       %d         %ld       %ld        %s\n",entry->d_type,depth,statbuf.st_uid,statbuf.st_size,statbuf.st_atime,entry->d_name);
    	}
    }   
	 
    chdir("..");//返回父目录; 
    closedir(dp);//关闭目录项; 
}
int main()
{
	char dir_name[256];
	printf("请输入要跳转的目录\n");
	scanf("%s",&dir_name);
	printdir(dir_name,0);
	return 0;
}