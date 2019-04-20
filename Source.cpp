//code_by_VU_TIEN_MUNG
//and_NGUYEN_QUOC_AN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include "console.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
 
#define consoleWidth	85
#define consoleHeight	30

enum TrangThai { UP, DOWN, LEFT, RIGHT };
bool pastPress[4];										//cap nhap trang thai trong qua khu

int map[consoleHeight][consoleWidth+1];					//Cap nhap map
int chooseMap;											//Chon map

struct ToaDo
{
	int y, x;											// y là tung độ, x là hoành độ
};

struct HoaQua
{
	ToaDo td;
	ToaDo tim;
};

struct Snake
{
	ToaDo dot[100];
	int n;
	TrangThai tt;
};

void StartGame();

void Menu()
{
	std::fstream f("Menu.txt");
	for (int i = 1; i <= 7; i++)
	{
		gotoXY(1, i);
		TextColor(ColorCode_Cyan);
		std::string s;
		std::getline(f, s);
		std::cout << "                          " << s << std::endl;
	}
	f.close();
}

void Help()
{
	std::fstream f("Help.txt");
	for (int i = 1; i <= 10; i++)
	{
		gotoXY(1, i);
		TextColor(ColorCode_Cyan);
		std::string s;
		std::getline(f, s);
		std::cout << s << std::endl;
	}
	f.close();
}

void Nocursortype()										// Xóa con trỏ chuột trên màn hình console
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void MapClear();

int ChonMap() 
{
	clrscr();
	MapClear();
	Menu();
	gotoXY(1, 8);
	printf("                                1. No Wall\n");
	gotoXY(1, 9);
	printf("                                2. EEG Map\n");
	gotoXY(1, 10);
	printf("                                3. Never Die\n");
	gotoXY(1, 11);
	printf("                                4. Snake Bot\n");
	gotoXY(1, 12);
	printf("                                5. Exit\n");
	gotoXY(1, 13);
	printf("                           Your choose: ");
	int i = 0;
	scanf("%d", &i);
	if (i == 5) exit(0);
	return i;
}

void KhoiTao(Snake& snake, HoaQua& hq)
{
	snake.n = 1;
	snake.dot[0].x = 0;
	snake.dot[0].y = 0;

	snake.tt = RIGHT;

	hq.td.x = 20;
	hq.td.y = 1;
	hq.tim.x = 25;
	hq.tim.y = 5;

	pastPress[UP] = false;
	pastPress[DOWN] = false;
	pastPress[RIGHT] = true;
	pastPress[LEFT] = false;
}

void KhoiTaoBot(Snake& snakeBot)
{
	snakeBot.n = 1;
	snakeBot.dot[0].x = 10;
	snakeBot.dot[0].y = 10;
	snakeBot.tt = RIGHT;
}

// map
void MapClear()
{
	TextColor(default_ColorCode);
	for (int i = 0; i < consoleHeight; i++)
	{
		gotoXY(consoleWidth, i);
		putchar('*');
	}// in tường bên phải
	for (int i = 0; i < consoleWidth; i++) {
		gotoXY(i, 0);
		if (i % 2 == 0)  putchar('*');
	}// in tường trên
	for (int i = 0; i < consoleHeight; i++)
	{
		gotoXY(0, i);
		putchar('*');
	}// in tường trái
	for (int i = 0; i <= consoleWidth; i++) {
		gotoXY(i, consoleHeight);
		if (i % 2 == 0)  putchar('*');
	}// in tường dưới

}

void MapEGG()
{
	std::fstream f("map.txt");

	for (int i=0; i<consoleHeight; i++)
		for (int j = 0; j < consoleWidth; j++)
		{
			if (j % 2 == 0)
			{
				int x;
				f >> x;
				if (x == 1)
				{
					TextColor(ColorCode_Yellow);
					gotoXY(j, i);
					map[i][j] = 1;
					putchar(16);
					
				}
				else if (x == 2)
				{
					TextColor(ColorCode_Red);
					gotoXY(j, i);
					putchar('*');
				}
			}
		}
	f.close();

}

void HienThi(Snake snake, HoaQua hq)
{
	//choose map
	if (chooseMap==1 || chooseMap == 3) MapClear();
	else if (chooseMap==2)	MapEGG();

	// in ra hoa quả
	TextColor(ColorCode_Green);
	gotoXY(hq.td.x, hq.td.y);
	putchar('O');

	// in ra con rắn
	//in ra cái đầu.
	TextColor(ColorCode_Pink);
	gotoXY(snake.dot[0].x, snake.dot[0].y);
	putchar(2);
	//in nốt phần thân còn lại.
	for (int i = 1; i <= snake.n; i++)
	{
		gotoXY(snake.dot[i].x, snake.dot[i].y);
		if (i == snake.n)
			putchar(' ');
		else 
			putchar(15);// xóa cái đuôi.
	}
	// in điểm, mạng
	gotoXY(consoleWidth + 5, 1);
	printf("Your Score: %d", snake.n);
	gotoXY(consoleWidth + 5, 4);
	printf("Press Space to Return menu!");
	gotoXY(consoleWidth + 5, 5);
	printf("Press Enter to Quit!");
}

void HienThiBot(Snake snakeBot) {
	TextColor(ColorCode_Yellow);
	gotoXY(snakeBot.dot[0].x, snakeBot.dot[0].y);
	putchar(2);
	//in nốt phần thân còn lại.
	for (int i = 1; i <= snakeBot.n; i++)
	{
		gotoXY(snakeBot.dot[i].x, snakeBot.dot[i].y);
		if (i == snakeBot.n)
			putchar(' ');
		else
			putchar(15);// xóa cái đuôi.
	}
}
void DieuKhien_DiChuyen(Snake& snake)
{
	// truyền trạng thái cho đốt cũ
	for (int i = snake.n; i > 0; i--)
		snake.dot[i] = snake.dot[i - 1];

	if (_kbhit()) // nếu phát hiện có phím nhấn vào
	{
		int key = _getch();

		// Chọn lại map
		if (key == ' ') {
			clrscr();
			chooseMap = ChonMap();
			clrscr();
		}
		// Thoát Game
		if (key == 13) exit(0);
		// điều khiển cái đầu mà thôi
		if ((key == 'A' || key == 'a') && pastPress[RIGHT] == false)
		{
			snake.tt = LEFT;
			pastPress[LEFT] = true;
			pastPress[UP] = false;
			pastPress[RIGHT] = false;
			pastPress[DOWN] = false;
		}
		else if ((key == 'D' || key == 'd') && pastPress[LEFT] == false)
		{
			snake.tt = RIGHT;
			pastPress[DOWN] = false;
			pastPress[UP] = false;
			pastPress[LEFT] = false;
			pastPress[RIGHT] = true;
		}
		else if ((key == 'W' || key == 'w') && pastPress[DOWN] == false)
		{
			pastPress[UP] = true;
			pastPress[DOWN] = false;
			pastPress[LEFT] = false;
			pastPress[RIGHT] = false;
			snake.tt = UP;
		}
		else if ((key == 'S' || key == 's') && pastPress[UP] == false)
		{
			snake.tt = DOWN;
			pastPress[RIGHT] = false;
			pastPress[LEFT] = false;
			pastPress[UP] = false;
			pastPress[DOWN] = true;
		}
	}


	if (snake.tt == UP)
	{
		snake.dot[0].y--;
	}
	else if (snake.tt == DOWN )
	{
		snake.dot[0].y++;
	}
	else if (snake.tt == LEFT)
	{
		snake.dot[0].x--;
		
	}
	else if (snake.tt == RIGHT )
	{
		snake.dot[0].x++;
	}
}

void DieuKhien_DiChuyen_Bot(Snake& snakeBot, HoaQua hq) {

	if ((hq.td.x < snakeBot.dot[0].x) && (snakeBot.tt != RIGHT)) snakeBot.tt = LEFT;
	if ((hq.td.x > snakeBot.dot[0].x) && (snakeBot.tt != LEFT)) snakeBot.tt = RIGHT;
	if ((hq.td.y < snakeBot.dot[0].y) && (snakeBot.tt != DOWN)) snakeBot.tt = UP;
	if ((hq.td.y > snakeBot.dot[0].y) && (snakeBot.tt != UP)) snakeBot.tt = DOWN;
	for (int i = snakeBot.n; i > 0; i--)
		snakeBot.dot[i] = snakeBot.dot[i - 1];

	if (snakeBot.tt == UP)
	{
		snakeBot.dot[0].y--;
	}
	else if (snakeBot.tt == DOWN)
	{
		snakeBot.dot[0].y++;
	}
	else if (snakeBot.tt == LEFT)
	{
		snakeBot.dot[0].x--;

	}
	else if (snakeBot.tt == RIGHT)
	{
		snakeBot.dot[0].x++;
	}

}

// trả về 0 nếu không có gì
// trả về -1 nếu thua game
int XuLy(Snake & snake, Snake & snakeBot, HoaQua & hq, int& ThoiGianSleep, int chooseMap)
{
	// xử lý chạm biên không chết.
	// rắn người.
	if (snake.dot[0].x <= 0) 
	{
		snake.dot[0].x = consoleWidth - 1;
		snake.tt = LEFT;
	}
	if (snake.dot[0].x >= consoleWidth) {
		snake.dot[0].x = 1;
		snake.tt = RIGHT;
	}
	if (snake.dot[0].y <= 0) {
		snake.dot[0].y = consoleHeight - 1;
		snake.tt = UP;
	}
	if (snake.dot[0].y >= consoleHeight) {
		snake.dot[0].y = 1;
		snake.tt = DOWN;
	}
	// rắn bot.
	if (snakeBot.dot[0].x <= 0)
	{
		snakeBot.dot[0].x = consoleWidth - 1;
		snakeBot.tt = LEFT;
	}
	if (snakeBot.dot[0].x >= consoleWidth) {
		snakeBot.dot[0].x = 1;
		snakeBot.tt = RIGHT;
	}
	if (snakeBot.dot[0].y <= 0) {
		snakeBot.dot[0].y = consoleHeight - 1;
		snakeBot.tt = UP;
	}
	if (snakeBot.dot[0].y >= consoleHeight) {
		snakeBot.dot[0].y = 1;
		snakeBot.tt = DOWN;
	}
	// Xử lý cắn vào thân	
	for (int i = 1; i < snake.n; i++) {
		if (snake.dot[0].x == snake.dot[i].x && snake.dot[0].y == snake.dot[i].y) {
			if (chooseMap != 3) return -1;// Chết.
			if (chooseMap == 3) {
				for (int j = snake.n; j > i; j--) {
					gotoXY(snake.dot[j].x, snake.dot[j].y);
					putchar(' ');
				}
				snake.n = i;// Không chết.
			}

		}
		// nếu rắn bot cắn rắn người thi thua game.
		if (chooseMap == 4) {
			if (snake.dot[i].x == snakeBot.dot[0].x && snake.dot[i].y == snakeBot.dot[0].y) {
				return -1;
			}
		}

	}
	for (int i = 1; i < snakeBot.n; i++) {
		if (snakeBot.dot[0].x == snakeBot.dot[i].x && snakeBot.dot[0].y == snakeBot.dot[i].y) {// nếu rắn bot tự cắn vào thân thì.
			for (int i = snakeBot.n; i > 0; i--) {
				gotoXY(snakeBot.dot[i].x, snakeBot.dot[i].y);
				putchar(' ');// xóa hết con rắn bot == coi như nó bị chết rồi xuất hiện bot mới.
				snakeBot.n = 1;
				snakeBot.dot[0].x = rand() % (consoleWidth - 1) + 1;
				snakeBot.dot[0].y = rand() % (consoleHeight - 1) + 1;
			}
		}
		if (snake.dot[0].x == snakeBot.dot[i].x && snake.dot[0].y == snakeBot.dot[i].y) {// nếu rắn người cắn vào bot thì bot bị khỏi tạo lại
			for (int j = snakeBot.n; j >= 0; j--) {
				gotoXY(snakeBot.dot[j].x, snakeBot.dot[j].y);
				putchar(' ');// xóa hết con rắn bot == coi như nó bị chết rồi xuất hiện bot mới.
				snakeBot.n = 1;
				snakeBot.dot[0].x = rand() % (consoleWidth - 1) + 1;
				snakeBot.dot[0].y = rand() % (consoleHeight - 1) + 1;
			}
		}
	}
	//xu ly ran can vao map
	if (chooseMap == 2) 
		if (map[snake.dot[0].y][snake.dot[0].x] == 1)
		return -1;
	// rắn người ăn được hoa quả	
	if (snake.dot[0].x == hq.td.x && snake.dot[0].y == hq.td.y)
	{
		for (int i = snake.n + 1; i > 0; i--)
			snake.dot[i] = snake.dot[i - 1];

		snake.n++;

		if (snake.tt == UP)
			snake.dot[0].y--;
		else if (snake.tt == DOWN)
			snake.dot[0].y++;
		else if (snake.tt == LEFT)
			snake.dot[0].x--;
		else if (snake.tt == RIGHT)
			snake.dot[0].x++;


		hq.td.x = rand() % (consoleWidth - 1) + 1;

		hq.td.y = rand() % (consoleHeight - 1) + 1;

			
		if (chooseMap == 2) {
			while (map[hq.td.y][hq.td.x] == 1)
			{
				hq.td.y = rand() % consoleHeight;
			}
		}
		if (ThoiGianSleep > 30)
			ThoiGianSleep -= 10;
	}
	// rắn bot ăn được hoa quả
	if (snakeBot.dot[0].x == hq.td.x && snakeBot.dot[0].y == hq.td.y)
	{
		for (int i = snakeBot.n + 1; i > 0; i--)
			snakeBot.dot[i] = snakeBot.dot[i - 1];

		snakeBot.n++;

		if (snakeBot.tt == UP)
			snakeBot.dot[0].y--;
		else if (snakeBot.tt == DOWN)
			snakeBot.dot[0].y++;
		else if (snakeBot.tt == LEFT)
			snakeBot.dot[0].x--;
		else if (snakeBot.tt == RIGHT)
			snakeBot.dot[0].x++;

		hq.td.x = rand() % (consoleWidth - 1) + 1;

		hq.td.y = rand() % (consoleHeight - 1) + 1;
	}
	return 0;
}

void StartGame()
{
	clrscr();
	std::fstream f("Startgame.txt");
	for (int i = 0; i < 8; i++) std::cout << std::endl;
	for (int i = 0; i < 15; i++)
	{
		TextColor(ColorCode_Cyan);
		std::string s;
		std::getline(f, s);
		std::cout << "                    " << s << std::endl;
	}
	f.close();
	Snake snakeBot, snake;
	HoaQua hq;
	hq.td.x = 54;
	hq.td.y = 24;

	KhoiTaoBot(snakeBot);
	snakeBot.dot[0].x = 0;
	snakeBot.dot[0].y = 0;
	TextColor(ColorCode_Green);
	gotoXY(hq.td.x, hq.td.y);
	putchar('O');

	int ma = 0;
	int ThoiGianSleep = 100;
	bool loop = true;
	while (loop)
	{
		gotoXY(0, 0);
		Nocursortype();
		//Bot
		HienThiBot(snakeBot);
		DieuKhien_DiChuyen_Bot(snakeBot, hq);
		// xử lý ăn hoa quả, thua game
		ma = XuLy(snake, snakeBot, hq, ThoiGianSleep, chooseMap);
		Sleep(ThoiGianSleep);
		if ((snakeBot.dot[0].x == 49) && (snakeBot.dot[0].y == 24)) loop = false;;
	}
	clrscr();
	Help();
	Sleep(6000);
}

void EndGame()
{
	std::fstream f("Endgame.txt");
	for (int i = 0; i < 8; i++) std::cout << std::endl;
	for (int i=0; i<15; i++)
	{
		TextColor(ColorCode_Cyan);
		std::string s;
		std::getline(f, s);
		std::cout << "                                     "<<s << std::endl;
			
		

	}
	f.close();
}
int main()
{
	StartGame();
	bool play = true;
	while (play)
	{	
		//Init
		TextColor(ColorCode_DarkYellow);
		clrscr();
		Snake snake, snakeBot;
		HoaQua hq;
		int ma = 0;
		int ThoiGianSleep = 150;


		chooseMap = ChonMap();
		clrscr();
		srand(time(NULL));	// khởi tạo bộ sinh số ngẫu nhiên
		KhoiTao(snake, hq);
		KhoiTaoBot(snakeBot);

		
		bool loop = true;
		while (loop)
		{
		
			Nocursortype();
			//Bot
			if (chooseMap == 4) {
				HienThiBot(snakeBot);
				DieuKhien_DiChuyen_Bot(snakeBot, hq);
			}
			// hiển thị
			HienThi(snake, hq);

			// điều khiển
			DieuKhien_DiChuyen(snake);

			// xử lý ăn hoa quả, thua game
			ma = XuLy(snake, snakeBot, hq, ThoiGianSleep, chooseMap);

			// thua game, thắng game
			if (ma == -1)	// thua game đó nha
			{
				clrscr();
				EndGame();
				gotoXY(54, 24);
				printf("Your Score: %d", snake.n);
				gotoXY(46, 25);
				printf("Press Space to Return menu!");
				gotoXY(50, 26);
				printf("Press Enter to Quit!");
				loop = false;
				int key = _getch();
				while ((key != ' ') && (key!=13)) key = _getch();
				if (key != ' ')	play = false;
				else if (key == 13) exit(0);

			}

			if (pastPress[LEFT] || pastPress[RIGHT]) Sleep(ThoiGianSleep / 2);
			else Sleep(ThoiGianSleep);
		}
	}
	endgame: return 0;
}