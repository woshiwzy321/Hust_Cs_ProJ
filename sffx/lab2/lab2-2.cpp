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
             X[k]=X[k]+1; //不断的在解空间里从小到大的试探
 
             while((X[k]<=n)&&(!place(k, X)))
                      X[k]=X[k]+1;                     //不符合条件的马上再取解空间的下一个值来试探。
 
             if(X[k]<=n)   //找到了一个位置，而且是合法的
                  if(k==n)   //是不是最后一个皇后，若是则得出一个完整解
                 {
                          for(int i=1;i<=n;i++)
                          cout<<X[i]<<" ";
                          cout<<"\n";
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
        cout<<"|--------------N皇后问题--------------|"<<endl;
        cout<<"|-------------------------------------|"<<endl<<endl;
        int n;
        int *X;
        int i;
        while(i)
       {
                 cout<<"请输入皇后的个数:";
                 cin>>n;
                 X=new int[n];
                 cout<<"问题的解有:"<<endl;
                 Nqueens(n,X);
                 cout<<"Press<1> to run again"<<endl;
                 cout<<"Press<0> to exit"<<endl;
                 cin>>i;
         }
         return 0;
}
