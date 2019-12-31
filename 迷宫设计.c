//@author zxc
#include<stdio.h>   //��  BFS  ����������� + ���� 
#include<stdlib.h>
#include<string.h>
 
#define MAXSIZE 1000000 //����������� 
#define MAX_STEP 20 //�������Ĳ��� 

//���������ƶ�����ı仯
int dir[4][2] = {
	{-1, 0},    // ��  �� ��-1 
    { 1, 0},    // ��  �� ��+1 
    { 0,-1},	// ��  �� ��-1 
    { 0, 1}		// ��  �� ��+1 
    //��ά�����һ��ֵ���� �ߣ��ڶ���ֵ���� ��
};


typedef struct{
	int h, w; //����x
	
	int step;//step��ʾ�ӳ�������ǰ�㾭������
	
	/* 
	path��ʾ�ӳ�������ǰ�㾭��·��
  	1234�ֱ������������UDLR 
	static String[] nextpath = { "U", "D", "L", "R" }; */
	char *path;  
	
}Point;

void initPoint(Point *p,int h,int w,int step,char* path)  //����һ���ڵ�P 
{
	p->h = h;
	p->w = w;
	p->step = step;
	p->path = path;
}

//���еĶ��� 
typedef struct{
	Point data[MAXSIZE]; 
	int front,rear;
}Queue; 


//��ʼ������ 
Queue* initQueue(){
	Queue* q=(Queue*)malloc(sizeof(Queue));
	q->front=0;
	q->rear=0;
	return q;
}

//��� 
void offer(Queue* q,Point x){
	q->data[q->rear++] = x;
}

//��Ϊ�� 
int queueIsEmpty(Queue* q){
	if(q->front==q->rear){
		return 1;
	}else{
		return 0;
	}
}

//���� 
Point poll(Queue* q){
	return q->data[q->front++];
} 




int oddEven(int n){
	if(n%2!=0){//���� 
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
	
	int i,j;//��ͨ���� 
	int height, width;//��� 
	Point p1;//��ʼ��
	Point p2;//�յ�
	
	//��̬�����Թ�
	printf("�����Թ��ߣ�");
	scanf("%d",&height);
	printf("�����Թ���");
	scanf("%d",&width);
	
	int **mazeArray = (int **)malloc(sizeof(int *) * height);   //����ָ�� height 
	for (i = 0; i < height; i++)
	{
		mazeArray[i] = (int *)malloc(sizeof(int) * width);     //ÿ������ָ����� width ��һ��ָ�� 
	}
	//�Թ�ͼ 0������·����1�������ϰ�
	printf("������ͼ������0������·����1�������ϰ�\n");
	for(i=0; i<height; i++){
		for(j=0; j<width; j++){
			scanf("%d",&mazeArray[i][j]);
		}
	}
	printf("�Թ��������ͼ\n"); 
	printMaze(mazeArray,height,width);
	
	//������ʼ�����յ� 
	printf("��������㣨�ߣ�����1��ʼ��\n");
	scanf("%d%d",&p1.h ,&p1.w );
	p1.h--;p1.w--;
	printf("�������յ㣨�ߣ�����1��ʼ��\n");
	scanf("%d%d",&p2.h ,&p2.w ); 
	p2.h--;p2.w--;
	
	while(mazeArray[p1.h][p1.w] != 0 || mazeArray[p2.h][p2.w] != 0){
		printf("�����������������:\n");
		printf("��������㣨�ߣ�����\n");
		scanf("%d%d",&p1.h ,&p1.w );
		p1.h--;p1.w--;
		printf("�������յ㣨�ߣ�����\n");
		scanf("%d%d",&p2.h ,&p2.w ); 
		p2.h--;p2.w--;
	} 
	p1.step = 0;	//����ʼ����Ϊ0 
	p1.path = (char*)malloc(sizeof(char) * MAX_STEP);
	
	p2.step = 0;
	p2.path = (char*)malloc(sizeof(char) * MAX_STEP);


	//bfs�㷨������·���Ĺؼ����飬��char����洢��Լ�ռ� 
	char *minPath = bfs(mazeArray,height,width,p1,p2);
	
	
	if(tag){
		
		//���·���ĳ��� 
		int len = minPathLen(minPath);
		printf("���·���ĳ��� ��%d \n",len);
		//�������·���ؼ������ӡ�����·��
		//UDLR������������ 
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
		printf("\n���·�����£���X �� Ϊ�ϰ�������ͷ������ָ �� Ϊ���·�� ��$ ��Ϊ���  ��# ��Ϊ�յ�\n");
		int step=1;
		
		printMaze(mazeArray,height,width);
	}else{
		printf("����ʼ�����յ��·��\n");
	}
	
}

char* bfs(int **mazeArray,int n,int m,Point p1,Point p2){
	int i;//��ͨ����
	int minStep = 0x7fffffff;//��С���� �� ���ֵ 
	char *minPath = (char *)malloc(sizeof(char)*MAX_STEP);//���·����1234�ֱ������������UDLR 
	tag = 0;
	int height = p2.h;
    int width  = p2.w;
    
    Queue* queue = initQueue(); 
	offer(queue,p1);  //������ 
    
    while( !queueIsEmpty(queue) ){//���в�Ϊ�� 
    	Point currentPoint = poll(queue);   //���� 
    	if(currentPoint.h == height && currentPoint.w == width){  //��ǰ�����յ� 
            if (minStep > currentPoint.step) {
                minStep = currentPoint.step;     //��¼��̲��� 
				strcpy(minPath,currentPoint.path);  //��¼���·�� 
            }
            tag = 1;   	//���ҵ��յ� 
            continue;
        }
		if(tag && currentPoint.step>minStep){  //����ǰ����������С��������˴β��Ҳ��ü��� 
			continue;
		}
        
        for(i=0; i<4; i++){
        	//�ı�߿� 
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

			//�ж���һ�����Ƿ���� 
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
					case 6:printf(" ��");break;
					case 7:printf(" ��");break;
					case 8:printf(" ��");break;
					case 9:printf(" ��");break;
            	}
			}
		}
		printf("\n");
	}
} 
//���Ե�ͼ 
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

