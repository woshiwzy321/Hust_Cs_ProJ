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
             X[k]=X[k]+1; //���ϵ��ڽ�ռ����С�������̽
 
             while((X[k]<=n)&&(!place(k, X)))
                      X[k]=X[k]+1;                     //������������������ȡ��ռ����һ��ֵ����̽��
 
             if(X[k]<=n)   //�ҵ���һ��λ�ã������ǺϷ���
                  if(k==n)   //�ǲ������һ���ʺ�������ó�һ��������
                 {
                        
						  for(int i=1;i<=n;i++)
                          printf("%d ",X[i]);
                          printf("\n");
                          flag=1;
                   }
                  else    //���������һ���ʺ������һ���ʺ���λ��
                 {
                          k=k+1;
                          X[k]=0;
                 }
  
             else      k=k-1;  //��������ȫ�����ж��޷�����ĳ���ʺ�����ݵ���һ��k�����������һ��k��������
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
     printf("������ʺ�����Ĵ�С��");
     scanf("%d",&n);
     if(n<=2)printf("�������޽⣡\n");
     else{
     
     X=(int *)malloc(n*sizeof(int));
     
     Nqueens(n,X);
     if(flag==0)printf("�������޽⣡\n");
     }
	 printf("����1�������У�����0ֹͣ���У�\n");
     scanf("%d",&i);
     
    // free(X);
     }
     return 0;
}
