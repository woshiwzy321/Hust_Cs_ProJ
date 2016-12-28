#include <math.h>
#include <stdlib.h> 
#include <stdio.h>
#include <windows.h>
int flag=0; 
bool place(int k, int *X)
{
        int i;
        i=1;
        while(i<k)
        {
                  if((X[i]==X[k])||(abs(X[i]-X[k])==abs(i-k)))
                  return false;
                  i++;
         }
        return true;
}


void Nqueens(int n,int *X)
{
       int k;
       X[1]=0;k=1;
       while(k>0)
       {
             X[k]=X[k]+1; //不断的在解空间里从小到大的试探
 
             while((X[k]<=n)&&(!place(k, X)))
                      X[k]=X[k]+1;                     //不符合条件的马上再取解空间的下一个值来试探。
 
             if(X[k]<=n)   //找到了一个位置，而且是合法的
                  if(k==n)   //是不是最后一个皇后，若是则得出一个完整解
                 {
                        
						  for(int i=1;i<=n;i++)
                          printf("%d ",X[i]);
                          printf("\n");
                          flag=1;
                   }
                  else    //若不是最后一个皇后，则给下一个皇后找位置
                 {
                          k=k+1;
                          X[k]=0;
                 }
  
             else      k=k-1;  //若是找了全部的列都无法放置某个皇后，则回溯到上一个k的情况，让上一个k再往下试
        }
 
}
int  main()
{
    int n;
    int *X;
    int i;
    while(i)
   {
   	 system("cls");
     printf("请输入皇后问题的大小：");
     scanf("%d",&n);
     if(n<=2)printf("此问题无解！\n");
     else{
     
     X=(int *)malloc(n*sizeof(int));
     
     Nqueens(n,X);
     if(flag==0)printf("此问题无解！\n");
     }
	 printf("输入1继续运行！输入0停止运行！\n");
     scanf("%d",&i);
     
    // free(X);
     }
     return 0;
}
