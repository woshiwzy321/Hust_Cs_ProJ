#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#define MAX 10
int A[MAX]={44,23,13,25,34,76,111,222,88,1};
/*�������ƣ�INTERCHANGE 
* �������ܣ�ʵ�ֽ��������������x��y���н��� 
* �������룺��Ҫ�������������ĵ�ַ&x,&y
* ����������� 
*/
void INTERCHANGE(int* x,int* y)
{
	int temp;
	temp=*x;
	*x=*y;
	*y=temp;
} 
/*�������ƣ�PARTITION 
* �������ܣ�ʵ�ֽ����������A���л��ִӶ�ʹA[m]��A[p]֮���Ԫ�ػ���Ϊ����A[m]��С��A[m]�������֣�����λ��ͨ��*p���� 
* �������룺λ��m��&p 
* ����������� 
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
	 	}while(A[i]<v);//i���������� 
	 	do
	 	{
	 		*p=*p-1;
	 	}while(A[*p]>v);//p����������
	 	if(i<*p)
	 		INTERCHANGE(&A[i],&A[*p]);
	 		else break;
	 }
	A[m]=A[*p];
	A[*p]=v;
	 
}
/*�������ƣ�QUICKSORT2 
* �������ܣ� ��λ��pλ�õ�Ԫ��Ϊ��׼����ǰq��Ԫ�ؽ��п��ٻ��� 
* �������룺p��q 
* ����������� 
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
	STACK=(int *)malloc(stack_max*sizeof(int));//ʹ������ģ���ջ���� 
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
	printf("������Ҫ����QUICKSORT2�㷨��P,Q��ֵ:\n");
	scanf("%d %d",&i,&j);
	printf("�ȴ����������Ϊ:\n");
	for(int k=0;k<MAX;k++)
	{
		printf("%d  ",A[k]);
	}
	printf("\n");
	QUICKSORT2(i,j); 
	printf("�����Ľ��Ϊ��\n");
	for(int k=0;k<MAX;k++)
	{
		printf("%d  ",A[k]);
	} 
	return 0;
}
