#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>


#define High	40					//��Ļ�߶�
#define Width	40					//��Ļ���
#define Buffer	30					//��󻺳屶��


int display[High][Width] = {0};		//��ʾ��Ԫ��
int snackhead_x;		//���^�Qֱ����
int snackhead_y;		//��ͷˮƽ����
int i,j;				//ѭ������
int direction;			//����
int length;				//�߳�
int pause;				//�������
int difficulty;			//�Ѷ�
int freedom;			//�����Ѷȴ�������
int food_x;				//ʳ����ֱ����
int food_y;				//ʳ��ˮƽ����
char ch;				//��������


int Gameover();			//�Ƿ���Ϸ����
int Initialize();		//���ݳ�ʼ��
int Show();				//��ʾ
int BackStage();		//��̨��������
int InputRefresh();		//���ݼ������� ��������
int EatFood();			//�Ƿ�Ե�ʳ��
int CreateFood();		//����ʳ��
void gotoxy(int,int);	//�������
void HideCursor();		//���ع��


int main()
{
	Initialize();		//���ݳ�ʼ��
	while(1)
	{
		Show();			//��ʾ
		BackStage();	//��̨����
		InputRefresh();	//�������
	}
	return 0;
}

int Gameover()			//��Ϸ����
{
	int lose = 0;
	if( snackhead_x<1		||	//��4�б�ʾײǽ
		snackhead_x>High-2	||
		snackhead_y<1		||
		snackhead_y>Width-2	||
		display[snackhead_x][snackhead_y]>1		)	//����
	{
		printf("\n\n \t\tGameover!	 \n\n");
		printf(" \t\t�Ƿ������(y/n) \n\n");
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

int Initialize()	//��ʼ��
{
	snackhead_x	= High / 5 * 3;
	snackhead_y = Width / 2;
	display[snackhead_x][snackhead_y] = 1;	//��ͷ��ʼ��
	direction	= 3;
	length		= 3;
	difficulty	= 0;
	freedom		= 0;
	pause		= Buffer;					//���屶����ʼ��

	CreateFood();							//����ʳ��

	for( i=0;i<High;i++ )					//���ұ߽��ʼ��
	{
		display[i][Width-1] = -2;
		display[i][0]		= -2;
	}
	for( i=0;i<Width;i++ )					//���±߽��ʼ��
	{
		display[0][i]		= -2;
		display[High-1][i]	= -2;
	}

	for( i=1;i<=length;i++ )				//�����ʼ��
		display[snackhead_x][snackhead_y+i-1] = i;

	printf("	ѡ����Ϸ�Ѷ�:		\n");	//��ʼ����
	printf("		1.��			\n");
	printf("		2.�е�			\n");
	printf("		3.����			\n");
	printf("		4.����			\n");
	printf("������� �������Ѷȿ�ʼ����");
	ch = getch();	//���յ�һ��(����Ϊ˫��ֵ,Ϊ��֮��Ĳ���,ҲΪ�˸���ѡ��)
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

int InputRefresh()	//ݔ�����
{
	ch = -32;
	if( kbhit() )						//�����̰���
	{
		ch = getch();
		if( ch==72 && direction!=2 )	//��
			direction = 1;
		if( ch==80 && direction!=1 )	//��
			direction = 2;
		if( ch==75 && direction!=4 )	//��
			direction = 3;
		if( ch==77 && direction!=3 )	//��
			direction = 4;
		if( ch=='+' && difficulty<30 )	//�Ӵ��Ѷ�,�ӿ��ٶ�
		{
			difficulty ++;
			pause ++;
		}
		if( ch=='-' && difficulty>0 )	//��С�Ѷ�,�����ٶ�
		{
			difficulty --;
			pause --;
		}
	}
	return 0;
}

int BackStage()		//��̨����
{
	if( pause!=difficulty )	//���滺��
		pause--;
	else
	{
		pause = Buffer;
		for( i=0;i<High;i++ )
		{
			for( j=0;j<Width;j++ )
			{
				if( display[i][j]>0 )	//�����ݸ���
				{
					display[i][j] ++;
					if( display[i][j]==length+1 )
						display[i][j] = 0;
				}
			}
		}
		if( direction==1 )	//�����ƶ�
		{
			snackhead_x --;
			EatFood();
			if( !Gameover() )
				display[snackhead_x][snackhead_y] = 1;
		}
		if( direction==2 )	//�����ƶ�
		{
			snackhead_x ++;
			EatFood();
			if( !Gameover() )
				display[snackhead_x][snackhead_y] = 1;
		}
		if( direction==3 )	//�����ƶ�
		{
			snackhead_y --;
			EatFood();
			if( !Gameover() )
				display[snackhead_x][snackhead_y] = 1;
		}
		if( direction==4 )	//�����ƶ�
		{
			snackhead_y ++;
			EatFood();
			if( !Gameover() )
				display[snackhead_x][snackhead_y] = 1;
		}
	}
	return 0;
}

int Show()	//�@ʾ
{
	gotoxy(0,0);		//����ƶ���ԭ��λ�ã������ػ�����
	HideCursor();		//���ع��
	printf("\n\n");
	for( i=0;i<High;i++ )
	{
		printf("\t\t");
		for( j=0;j<Width;j++ )
		{
			if( display[i][j]==1 )
				printf("@ ");	//��ͷ
			else if( display[i][j]==-2 )
				printf("# ");	//�߽�
			else if( display[i][j]>1 )
				printf("* ");	//����
			else if( display[i][j]==-1 )
				printf("$ ");	//ʳ��
			else
				printf("  ");	//�հ�
		}
		printf("\n");
	}
	printf("\n	\t\t\t��ǰ�߳�:%3d",length);
	printf("\n	\t\t\t��Ϸ�Ѷ�:%3d",difficulty);
	return 0;
}

int EatFood()		//�Ƿ�Ե�ʳ��
{
	if( display[snackhead_x][snackhead_y]==-1 )	//����Ե�
	{
		display[snackhead_x][snackhead_y] = 1;	//ʳ���Ϊ��ͷ

		CreateFood();							//�����µ�ʳ��
		length ++;								//�߳ɳ�

		if( length%5==0 && freedom )	//�����Ѷȡ����Ѷ���Ȼ����
			difficulty += 2;
	}
	return 0;
}

int CreateFood()		//�������ʳ��
{
	srand(time(NULL));
Create:
	food_x = rand()%High;
	food_y = rand()%Width;
	if( display[food_x][food_y]!=0 )	//ʳ���������ì��
		goto Create;					//����������ʳ��
	display[food_x][food_y] = -1;
	return 0;
}

void gotoxy(int x, int y)		
{		//����ƶ���(x,y)λ��
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle,pos);
}

void HideCursor()	//�[�ع��
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};//�ڶ���ֵΪ0����ʾ���ع��
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}