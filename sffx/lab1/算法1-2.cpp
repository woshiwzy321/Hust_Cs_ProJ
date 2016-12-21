#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#define MAX 8

int r=5;

void INTERCHANGE(int* x,int* y)
{
	int temp;
	temp=*x;
	*x=*y;
	*y=temp;
} 
/*函数名称：PARTITION 
* 函数功能：实现将输入的数组A进行划分从而使A[m]到A[p]之间的元素划分为大于A[m]和小于A[m]的两部分，划分位置通过*p带出 
* 函数输入：位置m和&p 
* 函数输出：无 
*/
void PARTITION(int A[],int m,int* p)
{
	int i,v;
	 v=A[m];
	 i=m;
	 while(1)
	 {
	 	do
	 	{
	 		i=i+1;
	 	}while(A[i]<v);//i由左向右移 
	 	do
	 	{
	 		*p=*p-1;
	 	}while(A[*p]>v);//p由右向左移
	 	if(i<*p)
	 		INTERCHANGE(&A[i],&A[*p]);
	 		else break;
	 }
	A[m]=A[*p];
	A[*p]=v;
	 
}
/*函数名称：INTERSORT 
* 函数功能： 对p和q内的元素进行排序 
* 函数输入：位置p和q 
* 函数输出：无 
*/
void INSERTIONSORT(int A[],int p,int q)
{
    int item,j,i;
    for(j=p+1;j<q;j++)
        {
            item=A[j];i=j-1;
            while(item<A[i]&&i>=p)//fix
            {
                A[i+1]=A[i];i=i-1;
            }
            A[i+1]=item;
        }

}
/*函数名称：SEL 
* 函数功能： select2的实现 
* 函数输入：位置A,M,P,K
* 函数输出：无 
*/
int SEL(int A[],int m,int p,int k)
{
	int n,i,j,temp_floor,temp_ceil;
	double temp_div,temp_div2; 
	if((p-m+1)<=r)
	{
		INSERTIONSORT(A,m,p);
		return (m+k-1);
	}
	while(1)
	{	
		n=p-m+1;
		temp_div=n*1.0/r;
		temp_floor=floor(temp_div);
		for(i=1;i<temp_floor;i++)
		{
			INSERTIONSORT(A,m+(i-1)*r,m+i*r-1);//将中间值收集到A(m:p)的前部
			temp_div=r/2.0;
			temp_floor=floor(temp_div);
			INTERCHANGE(&(A[m+i-1]),&(A[m+(i-1)*r+temp_floor-1]) ); 
		}
		temp_div=n*1.0/r;
		temp_floor=floor(temp_div);
		temp_div=temp_floor*1.0/2;
		temp_ceil=ceil(temp_div);
		j=SEL(A,m,m+temp_floor-1,temp_ceil);
		INTERCHANGE(&A[m],&A[j]);
		j=p+1;
		PARTITION(A,m,&j);
		if((j-m+1)==k) return j;
		else if((j-m+1)>k) p=j-1;
		else 
		{
			k=k-j+m-1;
			m=j+1;
		}
	}
} 

int main()
{
	int A[MAX]={22,44,33,12,45,65,43,3};
	int p=3,q=8,k=4;
	int answer;
	answer=SEL(A,p,q,k);
	printf("最终结果是%d  \n",answer);
	for(int tt=0;tt<MAX;tt++)
	{
		printf("%d  ",A[tt]);
	}
	return 0;
	
}
