#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <time.h>
int set_name=0;
int set_filename=0;
//int fstat(int fildes,struct stat *buf);//返回文件描述符相关的文件的状态信息
//int stat(const char *path, struct stat *buf);//通过文件名获取文件信息，并保存在buf所指的结构体stat中
//int lstat(const char *path, struct stat *buf);//如读取到了符号连接，lstat读取符号连接本身的状态信息，而stat读取的是符号连接指向文件的信息。
char* printacc(unsigned short mode)//打印出文件权限信息
{
  char*p=(char*)malloc(11*sizeof(char));
  p[10]='\0';
  if(S_ISDIR(mode))
  p[0]='d';
  else
  p[0]='-';
  if(mode&S_IREAD)
  p[1]='r';
  else
  p[1]='-';
  if(mode&S_IWRITE)
  p[2]='w';
  else
  p[2]='-';
  if(mode&S_IEXEC)
  p[3]='x';
  else
  p[3]='-';
  if(mode&S_IRGRP)
  p[4]='r';
  else
  p[4]='-';
  if(mode&S_IWGRP)
  p[5]='w';
  else
  p[5]='-';
  if(mode&S_IXGRP)
  p[6]='x';
  else
  p[6]='-';
  if(mode&S_IROTH)
  p[7]='r';
  else
  p[7]='-';
  if(mode&S_IWOTH)
  p[8]='w';
  else
  p[8]='-';
  if(mode&S_IXOTH)
  p[9]='x';
  else
  p[9]='-';
  return p;
}
char*time_CN(char*time1)//返回值为时间中文
{
  char* timecn=(char*)malloc(15*sizeof(char));
  char*time=(char*)malloc(4*sizeof(char));
  int i=0;
  time[0]=time1[0];
  time[1]=time1[1];
  time[2]=time1[2];
  time[3]='\0';
  if(strcmp(time,"Jan")==0)
  {
    sprintf(timecn," 1月");
  }
  if(strcmp(time,"Feb")==0)
  {
    sprintf(timecn," 2月");
  }
  if(strcmp(time,"Mar")==0)
  {
    sprintf(timecn," 3月");
  }
  if(strcmp(time,"Apr")==0)
  {
    sprintf(timecn," 4月");
  }
  if(strcmp(time,"May")==0)
  {
    sprintf(timecn," 5月");
  }
  if(strcmp(time,"Jun")==0)
  {
    sprintf(timecn," 6月");
  }
  if(strcmp(time,"Jul")==0)
  {
    sprintf(timecn," 7月");
  }
  if(strcmp(time,"Aug")==0)
  {
    sprintf(timecn," 8月");
  }
  if(strcmp(time,"Sep")==0)
  {
    sprintf(timecn," 9月");
  }
  if(strcmp(time,"Oct")==0)
  {
    sprintf(timecn,"10月");
  }
  if(strcmp(time,"Nov")==0)
  {
    sprintf(timecn,"11月");
  }
  if(strcmp(time,"Dec")==0)
  {
    sprintf(timecn,"12月");
  }
  timecn[5]=' ';
  for(i=3;i<12;i++)
  timecn[i+2]=time1[i];
  timecn[14]='\0';
  return timecn;
}
char *uid_to_name(uid_t uid)//将UID转换为名字
{
    struct passwd *getpwuid(),*pw_ptr;
    static char numstr[10];

    if((pw_ptr = getpwuid(uid))==NULL)
    {
        sprintf(numstr,"%d",uid);
        return numstr;
    }
    else
    {
        return pw_ptr->pw_name;
    }
}
void printdir(char *dir, int depth,char *dirname)
{
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    struct group*p2;
    struct passwd*p1;
    char path[100];
    path[0]="\0";
    if  ((dp = opendir(dir))== NULL)
    {     
        printf("文件目录打开失败！(是否具有相应的访问权限？！\n");
        return ;
    }
    chdir(dir);//将dir设置为当前目录; 
    while((entry=readdir(dp))!=NULL)
    {   
        lstat(entry->d_name,&statbuf);//以该目录项的名字为参数,调用lstat得到该目录项的相关信息;
        p1=getpwuid(statbuf.st_uid);
        p2=getgrgid(statbuf.st_gid);
        if(S_ISDIR(statbuf.st_mode))
        {
            if((strcmp(entry->d_name,"..")==0||strcmp(entry->d_name,".")==0))
               	continue;// 跳过该目录项;
          //  if(set_name==0)
          //  {
          //  	printf("/×××××××××××××××××××××××××××××××××目录递归显示××××××××××××××××××××××××××××××××××/\n");
          //  	printf("读写权限    深度   所有者UID     目录大小   最后修改日期      目录名字  \n");//打印目录项的深度、目录名等信息
         //   	set_name=1;
         //   }
            	printf("（目录）%s %d %s %s %6d %s %s\n",printacc(statbuf.st_mode),statbuf.st_nlink,p1->pw_name,p2->gr_name,statbuf.st_size,time_CN(4+ctime(&statbuf.st_mtime)),entry->d_name);
            
        }
        else
	 	{
	 		//if(set_filename==0)
	 	//	{
	 		//	printf("/×××××××××××××××××××××××××××××××××文件递归显示××××××××××××××××××××××××××××××××××/\n");
          ///  	printf("读写权限    深度   所有者UID     文件大小   最后修改日期      文件名字  \n");//打印文件的深度、文件名等信息
 		//		set_filename=1;   		
	 	//	}
	 			printf("（文件）%s %d %s %s %6d %s %s\n",printacc(statbuf.st_mode),statbuf.st_nlink,p1->pw_name,p2->gr_name,statbuf.st_size,time_CN(4+ctime(&statbuf.st_mtime)),entry->d_name);
    	}

    }
    seekdir(dp,0);
 	while((entry=readdir(dp))!=NULL){   
        lstat(entry->d_name, &statbuf);
        p1=getpwuid(statbuf.st_uid);
        p2=getgrgid(statbuf.st_gid);
        if(S_ISDIR(statbuf.st_mode)){
            if((strcmp(entry->d_name,"..")!=0)&&(strcmp(entry->d_name,".")!=0))
            {
              //printdir(strcat(strcat(strcat(path,dir),"/"),entry->d_name),depth+1);
              path[0]='\0';
              printdir(entry->d_name,depth+1,strcat(strcat(strcat(path,dirname),"/"),entry->d_name));
            }
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
	printdir(dir_name,1,".");
	//printf("类型  读写权限  成员个数   UID 组UID  目录大小   最后修改日期      目录名字  \n");
	return 0;
}