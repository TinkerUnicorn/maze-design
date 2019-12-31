//@author zxc
#include<stdio.h>   //用  BFS  广度优先搜索 + 队列 
#include<stdlib.h>
#include<string.h>
 
#define MAXSIZE 1000000 //队列最大容量 
#define MAX_STEP 20 //最大允许的步数 

//上下左右移动坐标的变化
int dir[4][2] = {
	{-1, 0},    // 上  ： 高-1 
    { 1, 0},    // 下  ： 高+1 
    { 0,-1},	// 左  ： 宽-1 
    { 0, 1}		// 右  ： 宽+1 
    //二维数组第一个值代表 高，第二个值代表 宽
};


typedef struct{
	int h, w; //坐标x
	
	int step;//step表示从出发到当前点经过几步
	
	/* 
	path表示从出发到当前点经过路径
  	1234分别代表上下左右UDLR 
	static String[] nextpath = { "U", "D", "L", "R" }; */
	char *path;  
	
}Point;

void initPoint(Point *p,int h,int w,int step,char* path)  //创造一个节点P 
{
	p->h = h;
	p->w = w;
	p->step = step;
	p->path = path;
}

//队列的定义 
typedef struct{
	Point data[MAXSIZE]; 
	int front,rear;
}Queue; 


//初始化队列 
Queue* initQueue(){
	Queue* q=(Queue*)malloc(sizeof(Queue));
	q->front=0;
	q->rear=0;
	return q;
}

//入队 
void offer(Queue* q,Point x){
	q->data[q->rear++] = x;
}

//队为空 
int queueIsEmpty(Queue* q){
	if(q->front==q->rear){
		return 1;
	}else{
		return 0;
	}
}

//出队 
Point poll(Queue* q){
	return q->data[q->front++];
} 




int oddEven(int n){
	if(n%2!=0){//奇数 
		return 1;
	}else{
		return 0;
	}
} 

char* bfs(int **mazeArray,int height,int width,Point p1,Point p2);
int minPathLen(char *a);
int nextDir(int i);
void printMaze(int **mazeArray,int height,int width);
int tag;

int main()
{
	
	int i,j;//普通变量 
	int height, width;//宽高 
	Point p1;//起始点
	Point p2;//终点
	
	//动态生成迷宫
	printf("输入迷宫高：");
	scanf("%d",&height);
	printf("输入迷宫宽：");
	scanf("%d",&width);
	
	int **mazeArray = (int **)malloc(sizeof(int *) * height);   //二级指针 height 
	for (i = 0; i < height; i++)
	{
		mazeArray[i] = (int *)malloc(sizeof(int) * width);     //每个二级指针分配 width 个一级指针 
	}
	//迷宫图 0代表有路径，1代表有障碍
	printf("创建地图：输入0代表有路径，1代表有障碍\n");
	for(i=0; i<height; i++){
		for(j=0; j<width; j++){
			scanf("%d",&mazeArray[i][j]);
		}
	}
	printf("迷宫框架如下图\n"); 
	printMaze(mazeArray,height,width);
	
	//输入起始点与终点 
	printf("请输入起点（高，宽）从1开始：\n");
	scanf("%d%d",&p1.h ,&p1.w );
	p1.h--;p1.w--;
	printf("请输入终点（高，宽）从1开始：\n");
	scanf("%d%d",&p2.h ,&p2.w ); 
	p2.h--;p2.w--;
	
	while(mazeArray[p1.h][p1.w] != 0 || mazeArray[p2.h][p2.w] != 0){
		printf("输入错误，请重新输入:\n");
		printf("请输入起点（高，宽）：\n");
		scanf("%d%d",&p1.h ,&p1.w );
		p1.h--;p1.w--;
		printf("请输入终点（高，宽）：\n");
		scanf("%d%d",&p2.h ,&p2.w ); 
		p2.h--;p2.w--;
	} 
	p1.step = 0;	//起点初始步数为0 
	p1.path = (char*)malloc(sizeof(char) * MAX_STEP);
	
	p2.step = 0;
	p2.path = (char*)malloc(sizeof(char) * MAX_STEP);


	//bfs算法求得最短路径的关键数组，用char数组存储节约空间 
	char *minPath = bfs(mazeArray,height,width,p1,p2);
	
	
	if(tag){
		
		//最短路径的长度 
		int len = minPathLen(minPath);
		printf("最短路径的长度 ：%d \n",len);
		//根据最短路径关键数组打印出最短路径
		//UDLR代表上下左右 
		int h=p1.h, w=p1.w;
		
	    for (i=0; i<len; i++)
		{
	        switch (minPath[i])
			{
	            case 1:h--;printf("U");mazeArray[h][w] = 6;break;
	            case 2:h++;printf("D");mazeArray[h][w] = 7;break;
	            case 3:w--;printf("L");mazeArray[h][w] = 8;break;
	            case 4:w++;printf("R");mazeArray[h][w] = 9;break;
	        }
	        if(minPath[i] != minPath[i+1] ){
				mazeArray[h][w] = nextDir(minPath[i+1]);
			}

	    }
	    mazeArray[p1.h][p1.w] = 4;
		mazeArray[p2.h][p2.w] = 5;
		printf("\n最短路径如下：‘X ’ 为障碍，‘箭头方向所指 ’ 为最短路径 ‘$ ’为起点  ‘# ’为终点\n");
		int step=1;
		
		printMaze(mazeArray,height,width);
	}else{
		printf("无起始点至终点的路径\n");
	}
	
}

char* bfs(int **mazeArray,int n,int m,Point p1,Point p2){
	int i;//普通变量
	int minStep = 0x7fffffff;//最小步数 赋 最大值 
	char *minPath = (char *)malloc(sizeof(char)*MAX_STEP);//最短路径，1234分别代表上下左右UDLR 
	tag = 0;
	int height = p2.h;
    int width  = p2.w;
    
    Queue* queue = initQueue(); 
	offer(queue,p1);  //起点入队 
    
    while( !queueIsEmpty(queue) ){//队列不为空 
    	Point currentPoint = poll(queue);   //弹出 
    	if(currentPoint.h == height && currentPoint.w == width){  //当前点是终点 
            if (minStep > currentPoint.step) {
                minStep = currentPoint.step;     //记录最短步数 
				strcpy(minPath,currentPoint.path);  //记录最短路径 
            }
            tag = 1;   	//已找到终点 
            continue;
        }
		if(tag && currentPoint.step>minStep){  //若当前步数大于最小步数，则此次查找不用继续 
			continue;
		}
        
        for(i=0; i<4; i++){
        	//改变高宽 
            int h = currentPoint.h + dir[i][0];
            int w = currentPoint.w + dir[i][1];
            int step = currentPoint.step;
            char* nextPointPath = (char *)malloc(sizeof(char)*MAX_STEP);
            strcpy(nextPointPath,currentPoint.path);
            nextPointPath[step] = i+1;
            step++;
            nextPointPath[step] = 0;
            Point nextPoint;
            initPoint(&nextPoint,h,w,step,nextPointPath);

			//判断下一个点是否出界 
			if(	nextPoint.h>=0 && nextPoint.h<n &&
				nextPoint.w>=0 && nextPoint.w<m &&
				mazeArray[nextPoint.h][nextPoint.w] == 0){
					offer(queue,nextPoint);
					mazeArray[h][w] = 2;
			}
        }
        
	}
	return minPath;
}

int minPathLen(char *a){
	return strlen(a);
} 


int nextDir(int i){
	switch(i){
		case 1: return 6;
		case 2: return 7;
		case 3: return 8;
		case 4: return 9;
	}
}

void printMaze(int **mazeArray,int height,int width){
	int i, j;
	for(i=0; i<height*2+1; i++){
		for(j=0; j<width*2+1; j++){
			if(!oddEven(i) && !oddEven(j)){
				printf(" ");
			}else if(!oddEven(i) && oddEven(j)){
				printf("---");
			}else if(oddEven(i) &&!oddEven(j)){
				printf("|");
			}else if(oddEven(i) && oddEven(j)){
				switch (mazeArray[(i-1)/2][(j-1)/2]){
                    case 0:printf("   ");break;
                    case 1:printf(" X ");break;
                    case 2:printf("   ");break;
                    case 4:printf(" $ ");break;
					case 5:printf(" # ");break;
					case 6:printf(" ↑");break;
					case 7:printf(" ↓");break;
					case 8:printf(" ←");break;
					case 9:printf(" →");break;
            	}
			}
		}
		printf("\n");
	}
} 
//测试地图 
/*
0 0 0 0 0 0 1 0
0 1 0 1 0 0 1 0
0 0 0 1 0 1 0 0
0 0 0 0 0 0 0 0
1 0 1 0 0 1 0 0
1 0 1 0 0 0 1 0
0 0 0 0 1 0 0 1
0 1 0 0 0 0 1 0
*/

