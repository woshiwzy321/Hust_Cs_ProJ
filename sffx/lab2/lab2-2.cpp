#include <iostream>
#include <math.h>
#include <stdlib.h> 
using namespace std;
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
                          cout<<X[i]<<" ";
                          cout<<"\n";
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
        cout<<"|--------------N�ʺ�����--------------|"<<endl;
        cout<<"|-------------------------------------|"<<endl<<endl;
        int n;
        int *X;
        int i;
        while(i)
       {
                 cout<<"������ʺ�ĸ���:";
                 cin>>n;
                 X=new int[n];
                 cout<<"����Ľ���:"<<endl;
                 Nqueens(n,X);
                 cout<<"Press<1> to run again"<<endl;
                 cout<<"Press<0> to exit"<<endl;
                 cin>>i;
         }
         return 0;
}
