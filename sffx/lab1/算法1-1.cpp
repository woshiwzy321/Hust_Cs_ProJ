#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#define MAX 10
int A[MAX]={44,23,13,25,34,76,111,222,88,1};
/*函数名称：INTERCHANGE 
* 函数功能：实现将传入的两个数字x，y进行交换 
* 函数输入：将要交换的两个数的地址&x,&y
* 函数输出：无 
*/
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
void PARTITION(int m,int* p)
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
/*函数名称：QUICKSORT2 
* 函数功能： 以位于p位置的元素为标准，对前q个元素进行快速划分 
* 函数输入：p和q 
* 函数输出：无 
*/
void QUICKSORT2(int p,int q)
{
	int stack_max,j,top;
	double log_tmp1,log_tmp2;
	log_tmp1=log((double) MAX);
	log_tmp2=log((double) 2);
	stack_max=2*log_tmp1/log_tmp2;
	//printf("%d",stack_max);getchar();getchar();
	int *STACK;
	STACK=(int *)malloc(stack_max*sizeof(int));//使用数组模拟堆栈操作 
	//printf("%d",sizeof(STACK));getchar();getchar(); 
	top=0;
	while(1)
	{
		while(p<q)
		{
			j=q+1;
			PARTITION(p,&j);
			if((j-p)<(q-j))
			{
				STACK[top+1]=j+1;
				STACK[top+2]=q;
				q=j-1;
			} 
			else
			{
				STACK[top+1]=p;
				STACK[top+2]=j-1;
				p=j+1;
			}
			top=top+2;
		} 
		if(top==0) return ;
		q=STACK[top];
		p=STACK[top-1];
		top=top-2;
	}

}
int main ()
{
	int i,j;
	printf("请输入要传入QUICKSORT2算法的P,Q的值:\n");
	scanf("%d %d",&i,&j);
	printf("等待排序的数组为:\n");
	for(int k=0;k<MAX;k++)
	{
		printf("%d  ",A[k]);
	}
	printf("\n");
	QUICKSORT2(i,j); 
	printf("排序后的结果为：\n");
	for(int k=0;k<MAX;k++)
	{
		printf("%d  ",A[k]);
	} 
	return 0;
}
