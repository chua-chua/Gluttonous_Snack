#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>


#define High	40					//屏幕高度
#define Width	40					//屏幕宽度
#define Buffer	30					//最大缓冲倍数


int display[High][Width] = {0};		//显示器元素
int snackhead_x;		//蛇頭豎直坐標
int snackhead_y;		//蛇头水平坐标
int i,j;				//循环变量
int direction;			//方向
int length;				//蛇长
int pause;				//缓冲变量
int difficulty;			//难度
int freedom;			//自由难度触发变量
int food_x;				//食物竖直坐标
int food_y;				//食物水平坐标
char ch;				//按键容器


int Gameover();			//是否游戏结束
int Initialize();		//数据初始化
int Show();				//显示
int BackStage();		//后台处理数据
int InputRefresh();		//根据键盘输入 处理数据
int EatFood();			//是否吃到食物
int CreateFood();		//生成食物
void gotoxy(int,int);	//光标清屏
void HideCursor();		//隐藏光标


int main()
{
	Initialize();		//数据初始化
	while(1)
	{
		Show();			//显示
		BackStage();	//后台处理
		InputRefresh();	//输入控制
	}
	return 0;
}

int Gameover()			//游戏结束
{
	int lose = 0;
	if( snackhead_x<1		||	//此4行表示撞墙
		snackhead_x>High-2	||
		snackhead_y<1		||
		snackhead_y>Width-2	||
		display[snackhead_x][snackhead_y]>1		)	//自伤
	{
		printf("\n\n \t\tGameover!	 \n\n");
		printf(" \t\t是否继续？(y/n) \n\n");
		ch = getch();
		if( ch=='y' )
		{
			lose = 1;
			for( i=0;i<High;i++ )
			{
				for( j=0;j<Width;j++ )
					display[i][j] = 0;
			}
			system("cls");
			goto restart;
		}
		exit(0);
	}
restart:
	if( lose )
		Initialize();
	return 0;
}

int Initialize()	//初始化
{
	snackhead_x	= High / 5 * 3;
	snackhead_y = Width / 2;
	display[snackhead_x][snackhead_y] = 1;	//蛇头初始化
	direction	= 3;
	length		= 3;
	difficulty	= 0;
	freedom		= 0;
	pause		= Buffer;					//缓冲倍数初始化

	CreateFood();							//生成食物

	for( i=0;i<High;i++ )					//左右边界初始化
	{
		display[i][Width-1] = -2;
		display[i][0]		= -2;
	}
	for( i=0;i<Width;i++ )					//上下边界初始化
	{
		display[0][i]		= -2;
		display[High-1][i]	= -2;
	}

	for( i=1;i<=length;i++ )				//蛇身初始化
		display[snackhead_x][snackhead_y+i-1] = i;

	printf("	选择游戏难度:		\n");	//初始界面
	printf("		1.简单			\n");
	printf("		2.中等			\n");
	printf("		3.困难			\n");
	printf("		4.高速			\n");
	printf("按其余键 以自由难度开始……");
	ch = getch();	//接收第一键(光标键为双键值,为了之后的操作,也为了更多选择)
	switch(ch)
	{
	case '1':
		difficulty = Buffer/6;
		break;
	case '2':
		difficulty = Buffer/3;
		break;
	case '3':
		difficulty = Buffer/2;
		break;
	case '4':
		difficulty = Buffer/3*2;
		break;
	default:
		freedom	= 1;
	}
	system("cls");
	return 0;
}

int InputRefresh()	//輸入控制
{
	ch = -32;
	if( kbhit() )						//若键盘按下
	{
		ch = getch();
		if( ch==72 && direction!=2 )	//上
			direction = 1;
		if( ch==80 && direction!=1 )	//下
			direction = 2;
		if( ch==75 && direction!=4 )	//左
			direction = 3;
		if( ch==77 && direction!=3 )	//右
			direction = 4;
		if( ch=='+' && difficulty<30 )	//加大难度,加快速度
		{
			difficulty ++;
			pause ++;
		}
		if( ch=='-' && difficulty>0 )	//减小难度,减慢速度
		{
			difficulty --;
			pause --;
		}
	}
	return 0;
}

int BackStage()		//後台控制
{
	if( pause!=difficulty )	//画面缓冲
		pause--;
	else
	{
		pause = Buffer;
		for( i=0;i<High;i++ )
		{
			for( j=0;j<Width;j++ )
			{
				if( display[i][j]>0 )	//蛇数据更新
				{
					display[i][j] ++;
					if( display[i][j]==length+1 )
						display[i][j] = 0;
				}
			}
		}
		if( direction==1 )	//向上移动
		{
			snackhead_x --;
			EatFood();
			if( !Gameover() )
				display[snackhead_x][snackhead_y] = 1;
		}
		if( direction==2 )	//向下移动
		{
			snackhead_x ++;
			EatFood();
			if( !Gameover() )
				display[snackhead_x][snackhead_y] = 1;
		}
		if( direction==3 )	//向左移动
		{
			snackhead_y --;
			EatFood();
			if( !Gameover() )
				display[snackhead_x][snackhead_y] = 1;
		}
		if( direction==4 )	//向右移动
		{
			snackhead_y ++;
			EatFood();
			if( !Gameover() )
				display[snackhead_x][snackhead_y] = 1;
		}
	}
	return 0;
}

int Show()	//顯示
{
	gotoxy(0,0);		//光标移动到原点位置，以下重画清屏
	HideCursor();		//隐藏光标
	printf("\n\n");
	for( i=0;i<High;i++ )
	{
		printf("\t\t");
		for( j=0;j<Width;j++ )
		{
			if( display[i][j]==1 )
				printf("@ ");	//蛇头
			else if( display[i][j]==-2 )
				printf("# ");	//边界
			else if( display[i][j]>1 )
				printf("* ");	//蛇身
			else if( display[i][j]==-1 )
				printf("$ ");	//食物
			else
				printf("  ");	//空白
		}
		printf("\n");
	}
	printf("\n	\t\t\t当前蛇长:%3d",length);
	printf("\n	\t\t\t游戏难度:%3d",difficulty);
	return 0;
}

int EatFood()		//是否吃到食物
{
	if( display[snackhead_x][snackhead_y]==-1 )	//如果吃到
	{
		display[snackhead_x][snackhead_y] = 1;	//食物成为蛇头

		CreateFood();							//生成新的食物
		length ++;								//蛇成长

		if( length%5==0 && freedom )	//自由难度——难度自然增长
			difficulty += 2;
	}
	return 0;
}

int CreateFood()		//随机生成食物
{
	srand(time(NULL));
Create:
	food_x = rand()%High;
	food_y = rand()%Width;
	if( display[food_x][food_y]!=0 )	//食物坐标出现矛盾
		goto Create;					//则重新生成食物
	display[food_x][food_y] = -1;
	return 0;
}

void gotoxy(int x, int y)		
{		//光标移动到(x,y)位置
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle,pos);
}

void HideCursor()	//隱藏光標
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};//第二个值为0，表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}