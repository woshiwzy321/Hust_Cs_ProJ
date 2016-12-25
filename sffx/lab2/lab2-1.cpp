#include <cstdio> 
#include <cstring> 
#include <algorithm> 
using namespace std; 
const int INF = -1; 
int G[100][100]; 
int P[100]; 
int F[100]; 
int D[100]; 
int N; 
int main() 
{ 
	/*scanf("%d",&N); 
	for( int i = 0 ; i < N ; i ++ ) 
		for( int j = 0 ; j < N ; j ++ ) 
			scanf("%d",&G[i][j]); 
	*/
	//int f=-4294967296;
	//int f=-4294967296;
	//printf("%d",f);getchar();getchar();
	N=7;
	//memset( D , INF , sizeof(D) );
	for(int i=0;i<100;i++)
	{
		D[i]=INF ;//inf is -4294967296 in 64 bit
		P[i]=-1;
	} 
	printf("%d  dddd",D[1]); getchar();getchar();
	//memset( P , -1 , sizeof(D) ); if(P[1]=D[1])printf("DAHCULAIFDAWF "); 
	D[0] = 0; 
	for( int i = 0 ; i < N ; i ++ ) 
	{ 
		int min_Vertex = -1; 
		for( int j = 0 ; j < N ; j ++ ) 
			if( !F[j] && D[j] != INF && ( min_Vertex == -1 || D[j] < D[min_Vertex]  ) ) 
				min_Vertex = j; 
		if( min_Vertex == -1 ) break; 
		F[min_Vertex] = true; printf("%d  dd",&F[min_Vertex]); 
		for( int j = 0 ; j < N ; j ++ ) 
			if( !F[j] && G[min_Vertex][j] != INF && 
			     ( D[j] > D[min_Vertex] + G[min_Vertex][j] || D[j] == -1 ) ) 
			{ 
				P[j] = min_Vertex; 
				D[j] = D[min_Vertex] + G[min_Vertex][j]; 
			} 
	} 
	for( int i = 0 ; i < N ; i ++ ) 
		if( D[i] != INF ) 
		{ 
			printf("V%d->V%d : %d\t",0,i,D[i]); 
			int stack[100]; 
			int sp = 0; 
			for( int j = i ; j != -1 ; j = P[j] ) 
				stack[sp++] = j; 
			while( sp ) 
				printf("V%d,",stack[--sp]); 
			puts(""); 
		} 
	return 0; 
}

