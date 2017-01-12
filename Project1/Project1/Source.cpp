#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <conio.h>
using namespace std;  //���Ψ�cin,cout
#define set_color(c) SetConsoleTextAttribute(hOut,c)

enum { H = 23, W = 14, nBlock = 7 };        //�ϥΦC�|�]�w�e�����e, ����ƥ�
HANDLE hIn, hOut;                           //windows��I/O ���
bool   bExit = false;                       //�O�_����C��
int    level = 0, score = 0;                //���d, �n��
int    ox = 4, oy = 1;                      //�e�����W���I
int    B, N;                                //��e&�U�@�� ������s��
int    X, Y, dir = 0;                       //�����m, �ҳB���(0~3)
int    delay_time = 20;                     //����ɶ�
int    flexible_time = 20;                  //�[�t���䱱��{��
char   room[W][H];                          //�e��
int    block[nBlock][4] =                   //����˦�
{ { 0x6220,0x1700,0x2230,0x0740 },    // L
{ 0x6440,0x0e20,0x44c0,0x8e00 },      // _|
{ 0x04e0,0x0464,0x00e4,0x04c4 },      // T
{ 0x4620,0x6c00,0x4620,0x6c00 },      // z
{ 0x2640,0xc600,0x2640,0xc600 },      // ��z
{ 0x0f00,0x4444,0x0f00,0x4444 },      // |
{ 0x0660,0x0660,0x0660,0x0660 } };    // ��

void gotoxy(int x, int y) //��б���
{
	static COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(hOut, c);
}

void draw(int x, int y, char* s) { gotoxy(x * 2, y); cout << s; }

void over(int x, int y, char* s) { draw(x, y, s); getch(); exit(1); }

bool bMove_block(int bx, int by, int d)         //�ǤJ�s��m�P�����V
{                                               //��������_�ܰ�   
	for (int p = 0x8000, x, y = 0; y<4; y++)
		for (x = 0; x<4; x++, p >>= 1)
			if ((block[B][d] & p) && room[bx + x - ox][by + y - oy])	//�N�ϧλP�C���e��and�_�Ӭ�1 ��ܵL�k��ʩΥ��k���ʡA
			{															//x����0��3 �ϧγ��O��4�Ӥ���զ�
				return false;
			}
	return true;
}

void show_next_block(char* s)
{
	set_color(N + 7);                            //�]�w��ܦ�
	int x, y, p = 0x8000;
	for (y = 0; y<4; y++)
		for (x = 0; x<4; x++, p >>= 1)
			if (block[N][0] & p)
			{
				draw(ox + W + 2 + x, 2 + y, s);	//�Pshow_block�����u�O�N�w���I�令�S��ج����I
			}
}
void show_block(char* s)
{
	set_color(B + 7);                            //�]�w��ܦ�
	int x, y, p = 0x8000;
	for (y = 0; y<4; y++)
		for (x = 0; x<4; x++, p >>= 1)			//�N����Mp��AND�A�p�G���X�h���ʤ���A�w���I����e��m
			if (block[B][dir] & p)
			{
				draw(x + X, y + Y, s);
			}
}
void show_room()								//�L�X�~��
{
	for (int x, y = 0; y<H; y++)
		for (x = 0; x<W; x++)
			if (room[x][y])
			{
				set_color(room[x][y]);
				draw(ox + x, oy + y, "��");
			}
			else draw(ox + x, oy + y, "  ");
}

void try_move_block(int x, int y, int d)       //���ղ���
{
	if (!bMove_block(x, y, d))   //��bMove_block�P�_�O�_�i�H������ʡA�p�G�i�ʫh�N�s�y�бa�J�A�_�h�����{���C
	{
		return;
	}
	show_block("  ");
	X = x;
	Y = y;
	dir = d;
	show_block("��");
}



void move_block()
{
	if (bMove_block(X, Y + 1, dir))						//�P�_�O�_�i�H����
	{													//�p�G�i�H����
		try_move_block(X, Y + 1, dir);					//���ղ��ʤ��
		return;
	}													//�p�L�k���ʥB����y�йF��̰���m(Y�y�Ь�0)�A�h�C������
	if (Y == 0)
	{
		over(ox + 2, H / 2, "G a m e    O v e r");
	}
	for (int p = 0x8000, x, y = 0; y<4; y++)
		for (x = 0; x<4; x++, p >>= 1)
			if (block[B][dir] & p)						//�x�s�e�������}
			{
				room[X + x - ox][Y + y - oy] = B + 7;
			}

	show_next_block("  ");
	B = N; X = ox + W / 2 - 1; Y = 0;
	N = rand() % nBlock;
	show_next_block("��");
}

void init()                                     //��l�t�m
{
	srand(time(0));                             //���ܶüƨ��˪���l��
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);     //�ϥ�HANDLE�򥻳]�win/out
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE err = INVALID_HANDLE_VALUE;
	if (hIn == err || hOut == err)
	{
		puts("handle failed");
		getch();
		exit(1);                                //�^�Ǩ�bExit=1=ture,0=false
	}
	for (int x = 0; x<W; x++)                   //�N��ئ�}��Jroom
	{
		room[x][H - 1] = 4;
	}
	for (int y = 0; y<H; y++)
	{
		room[0][y] = room[W - 1][y] = 4;
	}

	B = rand() % nBlock;                       //��e����s�����ü�
	N = rand() % nBlock;                       //�U�@�Ӥ���s�����ü�
	X = ox + W / 2 - 1;                        //��l�Ʃж������m
	Y = 0;
	show_room();                               //�L�X�ж�
	show_next_block("��");                      //�M�w�Ĥ@�Ӹ��U�����
}


int main()
{
	int i;
	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible = FALSE;
	CCI.dwSize = 24;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CCI);
	init();										//�إߪ�l�e���M�Ĥ@�Ӹ��U���
	while (!bExit)                              //bExit=ture���X,false�~�����(��l��false)
	{
		Sleep(100);                  //����ɶ� ���ms
		move_block();							//���ʤ��
		set_color(14);
	}
}