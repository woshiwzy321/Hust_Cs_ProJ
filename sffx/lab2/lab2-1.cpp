#include <stdio.h> 
//#include <string.h> 

int COST[100][100]; //成本邻接矩阵 
int P[100]; //pick矩阵 
int S[100]; //判断选择最小边是否重复矩阵 
int DIST[100]; //dist（j）表示结点v到j的最小长度 
int N; //图中点的个数 
int main() 
{
	for(int i=0;i<100;i++)
	{
		DIST[i]=-1 ;//inf is -4294967296 in 64 bit
		P[i]=-1;
	} 
	int sel_ch,v,i,j;
	printf("输入1调用程序内例子，输入2自己输入例子:");
	scanf("%d",&sel_ch);
	if(sel_ch==2)
	{
		printf("请输入图的大小N："); 
		scanf("%d",&N); 
		N=N+1;
		printf("请输入各个边的权值（无法到达输-1）(双向注意)：\n"); 
		for( i = 0 ; i < N ; i ++ ) 
			for( j = 0 ; j < N ; j ++ ) 
			{
				printf("请输入结点%d到%d的距离: ",i,j);
				scanf("%d",&COST[i][j]); 
			} 
	
	}
	if(sel_ch==1)
	{
		N=6;
		for( int i = 0 ; i < N ; i ++ ) 
			for( int j = 0 ; j < N ; j ++ ) 
				 COST[i][j]=-1;
		COST[0][1]=50;COST[0][2]=10;COST[0][4]=45;
		COST[1][2]=15;COST[1][4]=10;
		COST[2][0]=10;COST[2][3]=15;
		COST[3][1]=20;COST[3][4]=35;
		COST[4][3]=30;
		COST[5][3]=3; 
	}
//	DIST[0] = 0; 
	printf("请输入开始结点v：");
	scanf("%d",&v); 
	for(i=0;i<N;i++)
	{
		S[i]=0;DIST[i]=COST[v][i];
	}
	S[v]=1;DIST[v]=0;
	for( int i = 0 ; i < N ; i ++ ) 
	{ 
		int min_Vertex = -1; 
		for( int j = 0 ; j < N ; j ++ ) 
			if( !S[j] && DIST[j] != -1 && ( min_Vertex == -1 || DIST[j] < DIST[min_Vertex]  ) ) 
				min_Vertex = j; 
		if( min_Vertex == -1 ) break; 
		S[min_Vertex] = true; 
		for( int j = 0 ; j < N ; j ++ ) 
			if( !S[j] && COST[min_Vertex][j] != -1 && 
			     ( DIST[j] > DIST[min_Vertex] + COST[min_Vertex][j] || DIST[j] == -1 ) ) 
			{ 
				P[j] = min_Vertex; 
				DIST[j] = DIST[min_Vertex] + COST[min_Vertex][j]; 
			} 
	} 
	for( int i = 0 ; i < N ; i ++ ) 
		if( DIST[i] != -1 ) 
		{   printf("start    end length     nodelist\n");
			printf("V%d  ->   V%d : %d        ",v,i,DIST[i]); 
			int stack[100]; 
			int sp = 0; 
			for( int j = i ; j != -1 ; j = P[j] ) 
				stack[sp++] = j; 
				printf("V%d  ",v);
			while( sp ) 
				printf("V%d  ",stack[--sp]); 
			puts(""); 
		} 
	return 0; 
}

