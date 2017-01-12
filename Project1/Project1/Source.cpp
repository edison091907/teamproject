#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <conio.h>
using namespace std;  //有用到cin,cout
#define set_color(c) SetConsoleTextAttribute(hOut,c)

enum { H = 23, W = 14, nBlock = 7 };        //使用列舉設定畫面長寬, 方塊數目
HANDLE hIn, hOut;                           //windows裡I/O 控制器
bool   bExit = false;                       //是否持續遊戲
int    level = 0, score = 0;                //關卡, 積分
int    ox = 4, oy = 1;                      //畫面左上原點
int    B, N;                                //當前&下一個 方塊的編號
int    X, Y, dir = 0;                       //方塊位置, 所處方位(0~3)
int    delay_time = 20;                     //延遲時間
int    flexible_time = 20;                  //加速按鍵控制的程度
char   room[W][H];                          //畫面
int    block[nBlock][4] =                   //方塊樣式
{ { 0x6220,0x1700,0x2230,0x0740 },    // L
{ 0x6440,0x0e20,0x44c0,0x8e00 },      // _|
{ 0x04e0,0x0464,0x00e4,0x04c4 },      // T
{ 0x4620,0x6c00,0x4620,0x6c00 },      // z
{ 0x2640,0xc600,0x2640,0xc600 },      // 反z
{ 0x0f00,0x4444,0x0f00,0x4444 },      // |
{ 0x0660,0x0660,0x0660,0x0660 } };    // 田

void gotoxy(int x, int y) //游標控制
{
	static COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(hOut, c);
}

void draw(int x, int y, char* s) { gotoxy(x * 2, y); cout << s; }

void over(int x, int y, char* s) { draw(x, y, s); getch(); exit(1); }

void show_next_block(char* s)
{
	set_color(N + 7);                            //設定顯示色
	int x, y, p = 0x8000;
	for (y = 0; y<4; y++)
		for (x = 0; x<4; x++, p >>= 1)
			if (block[N][0] & p)
			{
				draw(ox + W + 2 + x, 2 + y, s);	//與show_block類似只是將定位點改成又邊框為原點
			}
}
void show_block(char* s)
{
	set_color(B + 7);                            //設定顯示色
	int x, y, p = 0x8000;
	for (y = 0; y<4; y++)
		for (x = 0; x<4; x++, p >>= 1)			//將方塊和p做AND，如果重合則移動方塊，定位點為當前位置
			if (block[B][dir] & p)
			{
				draw(x + X, y + Y, s);
			}
}
void show_room()								//印出外框
{
	for (int x, y = 0; y<H; y++)
		for (x = 0; x<W; x++)
			if (room[x][y])
			{
				set_color(room[x][y]);
				draw(ox + x, oy + y, "▓");
			}
			else draw(ox + x, oy + y, "  ");
}




void init()                                     //初始配置
{
	srand(time(0));                             //改變亂數取樣的初始直
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);     //使用HANDLE基本設定in/out
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE err = INVALID_HANDLE_VALUE;
	if (hIn == err || hOut == err)
	{
		puts("handle failed");
		getch();
		exit(1);                                //回傳到bExit=1=ture,0=false
	}
	for (int x = 0; x<W; x++)                   //將邊框位址放入room
	{
		room[x][H - 1] = 4;
	}
	for (int y = 0; y<H; y++)
	{
		room[0][y] = room[W - 1][y] = 4;
	}

	B = rand() % nBlock;                       //當前方塊編號取亂數
	N = rand() % nBlock;                       //下一個方塊編號取亂數
	X = ox + W / 2 - 1;                        //初始化房間圍牆位置
	Y = 0;
	show_room();                               //印出房間
	show_next_block("▓");                      //決定第一個落下的方塊
}




int main()
{
	int i;
	CONSOLE_CURSOR_INFO CCI;
	CCI.bVisible = FALSE;
	CCI.dwSize = 24;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CCI);
	init();										//建立初始畫面和第一個落下方塊
	system("pause");
}