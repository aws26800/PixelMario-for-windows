#include <stdio.h>
#include <fcntl.h>
#include <Windows.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <io.h>
#include <process.h>
#include <string.h>

#define WIDTH 256
#define HEIGHT 96
#define FULL_WIDTH 3584

//1 Block = 16
struct Pos
{
	int X;
	int Y;
} mario, goomba[16];

struct Qest
{
	int x;
	int Y;
	int hit;
} Q[11];
//Type WORD
WORD color[HEIGHT * WIDTH];

//Type int
int MapMovX = 1;
int end = 1;
int goomba_move[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int tmp = 0;
int running = 1;
int play_sound_selection = 1;

//Type char
char array_high[96][3584], array_low[96][3584];
char flag_high[7][13] = {11, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 15, 15, 15, 15, 15, 12, 15, 15, 15, 15, 15, 11, 12, 15, 15, 12, 12, 12, 12, 12, 12, 12, 15, 15, 11, 12, 15, 15, 15, 15, 12, 12, 12, 15, 15, 15, 15, 11, 12, 15, 15, 15, 12, 15, 15, 15, 12, 15, 15, 15, 11, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11};
char flag_low[7][13] = {14, 14, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 11, 12, 15, 15, 15, 15, 15, 12, 15, 15, 15, 15, 15, 11, 12, 15, 15, 15, 12, 12, 12, 12, 12, 15, 15, 15, 11, 12, 15, 15, 15, 12, 12, 15, 12, 12, 15, 15, 15, 11, 12, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 11, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11};
char mario_texture[16][12] = {16, 16, 16, 12, 12, 12, 12, 12, 16, 16, 16, 16, 16, 16, 12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 16, 16, 6, 6, 6, 14, 14, 6, 14, 16, 16, 16, 16, 6, 14, 6, 14, 14, 14, 6, 14, 14, 14, 16, 16, 6, 14, 6, 6, 14, 14, 14, 6, 14, 14, 14, 16, 6, 6, 14, 14, 14, 14, 6, 6, 6, 6, 16, 16, 16, 16, 14, 14, 14, 14, 14, 14, 14, 16, 16, 16, 16, 6, 6, 12, 6, 6, 6, 16, 16, 16, 16, 16, 6, 6, 6, 12, 6, 6, 12, 6, 6, 6, 16, 6, 6, 6, 6, 12, 12, 12, 12, 6, 6, 6, 6, 14, 14, 6, 12, 14, 12, 12, 14, 12, 6, 14, 14, 14, 14, 14, 12, 12, 12, 12, 12, 12, 14, 14, 14, 14, 14, 12, 12, 12, 12, 12, 12, 12, 12, 14, 14, 16, 16, 12, 12, 12, 16, 16, 12, 12, 12, 16, 16, 16, 6, 6, 6, 16, 16, 16, 16, 6, 6, 6, 16, 6, 6, 6, 6, 16, 16, 16, 16, 6, 6, 6, 6};
char goomba_texture_high[8][16] = {11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 11, 12, 0, 0, 12, 12, 12, 12, 12, 12, 0, 0, 12, 11, 11, 11, 12, 12, 12, 7, 0, 0, 0, 0, 0, 0, 7, 12, 12, 12, 11, 12, 12, 12, 12, 7, 7, 7, 12, 12, 7, 7, 7, 12, 12, 12, 12, 11, 12, 12, 12, 12, 7, 7, 7, 7, 7, 7, 12, 12, 12, 12, 11, 11, 11, 11, 11, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 11, 11, 11, 11, 11, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0};
char goomba_texture_low[8][16] = {11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 11, 11, 11, 11, 12, 12, 12, 7, 0, 12, 12, 12, 12, 0, 7, 12, 12, 12, 11, 12, 12, 12, 12, 7, 0, 7, 12, 12, 7, 0, 7, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 11, 11, 11, 11, 7, 7, 7, 7, 7, 7, 7, 7, 0, 11, 11, 11, 11, 11, 11, 0, 0, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 11, 11, 11, 11, 11, 0, 0, 0, 7, 7, 0, 0, 0, 0, 0, 11, 11};
char path[50];

//Function
void Coloring(int order);
void setup();
void Write();
int MovM(int valueX, int valueY);
void Gravity();
void MovM_Safe(int valueX, int valueY);
int CanIJump();
void End();
void LoadMap();
void GameOver();

//Thread & HANDLE
void Input(void *args);
void goomba_move_thread(void *args);
void play_sound(int num);
void play_sound_(int num);

int main(int argc, char **argv)
{
	int i, j = 0, delay = 30, calcfps1 = 0, calcfps2 = 0;
	char c[10], text[20] = "title FPS : ", cat[20];
	LoadMap();
	setup();
	i = clock();
	j = clock();
	while(1) // FPS 표시 && FPS 조절 
	{
		Coloring(0);
		Write();
		calcfps1++;
		if(MapMovX + WIDTH > FULL_WIDTH)
		{
			return 1;
		}
		if(clock() - i > 1000)
		{
			cat[0] = '\0';
			itoa((calcfps1 - calcfps2), c, 10);
			if(calcfps1 - calcfps2 > 32)
			{
				delay++;
			}
			else if(calcfps1 - calcfps2 < 30)
			{
				delay--;
			}
			calcfps2 = calcfps1;
			strcat(cat, text);
			strcat(cat, c);
			system(cat);
			i += 1000;
		}
		while(clock() - j < delay)
		{
			Sleep(1);
		}
		j = clock();
	}
	
}

void LoadMap() // 맵 로딩 
{
	FILE *in;
	int i, j;
	if((in = fopen("Map_1.bin", "rb")) == NULL)
	{
		printf("[ERROR]Load -> Map_1.bin");
		while(1)
		{
			Sleep(1000);
		}
	}
	for(i = 0;i < 96;i++)
	{
		for(j = 0;j < 3584;j++)
		{
			array_high[i][j] = fgetc(in);
		}
	}
	fclose(in);
	if((in = fopen("Map_2.bin", "rb")) == NULL)
	{
		printf("[ERROR]Load -> Map_2.bin");
		while(1)
		{
			Sleep(1000);
		}
	}
	for(i = 0;i < 96;i++)
	{
		for(j = 0;j < 3584;j++)
		{
			array_low[i][j] = fgetc(in);
		}
	}
	fclose(in);
}

//height 79 (158) -> floor 
void Coloring(int order) // color배열에 색상값 넣기 
{
	int width, height, i, j;
	for(height = 0;height < HEIGHT;height++)
	{
		for(width = 0 + MapMovX;width < MapMovX + WIDTH;width++)
		{
			color[height * WIDTH + width - MapMovX] = (array_high[height][width] == 16 ? 0 : array_high[height][width]) | (array_low[height][width] == 16 ? 0 : array_low[height][width]) << 4;
		}
	}
	for(i = 0;i < 16;i++)
	{
		if(!(goomba[i].X >= MapMovX && goomba[i].X <= WIDTH + MapMovX - 16))
		{
			continue;
		}
		for(height = 0;height < 8;height++)
		{
			for(width = 0;width < 16;width++)
			{
				color[(goomba[i].Y + height) * WIDTH + goomba[i].X + width - MapMovX] = goomba_texture_high[height][width] | goomba_texture_low[height][width] << 4;
			}
		}
	}
	for(height = 0;height < 16;height += 2)
	{
		for(width = 0;width < 12;width++)
		{
			if((height / 2 + mario.Y) * WIDTH + width + mario.X - MapMovX >= HEIGHT * WIDTH - 1 || !(mario.Y + height < HEIGHT) || width + mario.X > 3263)
			{
				continue;
			}
			color[(height / 2 + mario.Y) * WIDTH + width + mario.X - MapMovX] = (mario_texture[height][width] != 16 ? mario_texture[height][width] : array_high[height / 2 + mario.Y][width + mario.X - MapMovX]) | (mario_texture[height + 1][width] != 16 ? mario_texture[height + 1][width] : array_low[height / 2 + mario.Y][width + mario.X - MapMovX]) << 4;
		}
	}
}

void Write() // color값 덮어씌우기 
{
	COORD coord = {0, 0};
	DWORD written[HEIGHT * WIDTH];
	WriteConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color, WIDTH * HEIGHT, coord, written);
}

void setup() // 기본셋업 
{
	int width, height, i, j;
	mario.X = 0;
	mario.Y = 72;
	printf("유의사항 : \n♡CPU 사용량이 상당히 아름다울 수 있습니다♡\n이는 이 프로그램을 화면에 출력해주는 콘솔호스트가 먹는것입니다.\n\n콘솔 글씨 사이즈를 8폰트로 설정해주세요.\n\"유의사항\"의 '유'자 바로 위에 \"응용프로그램\" 아이콘을 클릭해주세요.\n속성-글꼴-크기를 8으로 설정\n\n이 프로그램은 1920*1080 100배율에 최적화 되어있습니다.\n125배율까지는 호환되는것을 확인 하였으나 100배율에서 실행하는것을 추천드립니다.\n윈도우키-설정-시스템-디스플레이-텍스트, 앱 및 기타 항목의 크기 변경 -> 100%으로 설정\n\n아무키나 눌러 계속하세요!");
	_getch();
	system("mode 256, 97");
	_setmode(_fileno(stdout), _O_U16TEXT); // 출력값 UTF-16으로 변경 
	for(height = 0;height < HEIGHT;height++)
	{
		for(width = 0;width < WIDTH;width++)
		{
			wprintf(L"\u2580"); // 화면에 글자 출력 
		}
		wprintf(L"\n");
	}
	for(i = 0;i < 16;i++) // 굼바셋팅 
	{
		if(i != 4 && i != 5)
		{
			goomba[i].Y = 72;
		}
		else
		{
			goomba[i].Y = 8;
		}
	}
	goomba[0].X = 352; // 굼바셋팅 
	goomba[1].X = 640;
	goomba[2].X = 816;
	goomba[3].X = 840;
	goomba[4].X = 1280;
	goomba[5].X = 1312;
	goomba[6].X = 1552;
	goomba[7].X = 1576;
	goomba[8].X = 1824;
	goomba[9].X = 1848;
	goomba[10].X = 1984;
	goomba[11].X = 2008;
	goomba[12].X = 2048;
	goomba[13].X = 2072;
	goomba[14].X = 2784;
	goomba[15].X = 2808;
	
	_beginthread(play_sound, 0, (int)1); // 사운드 재생 
	running = 1;
	_beginthread(Input, 0, (void*)0);
	_beginthread(goomba_move_thread, 0, (void*)0);
	
}
//3264
int MovM(int valueX, int valueY) // MoveMario
{
	int width, height;
	for(height = 0;height < 8;height++)
	{
		for(width = 0;width < 12;width++)
		{
			if(height + mario.Y - valueY > HEIGHT - 1) // 마리오가 구멍에 떨어졌을때 >_< 
			{
				GameOver();
			}
			if((array_high[height + mario.Y][width + mario.X + 1] == 6 || array_low[height + mario.Y][width + mario.X + 1] == 6) && end) // 마리오가 끝까지 갔을때 
			{
				end = !end;
				End(); // 성공! 
			}
			if(array_high[height + mario.Y - valueY][width + mario.X + valueX] != 11 || array_low[height + mario.Y - valueY][width + mario.X + valueX] != 11 || height + mario.Y - valueY < 0 || width + mario.X + valueX < 0 || mario.X + valueX - MapMovX < 0)
			{
				if(array_high[height + mario.Y - valueY][width + mario.X + valueX] != 6 || array_low[height + mario.Y - valueY][width + mario.X + valueX] != 6 || height + mario.Y - valueY < 0 || width + mario.X + valueX < 0 || mario.X + valueX - MapMovX < 0)
				{
					return 1; // 맵 밖으로 나가는것 방지 
				}
			}
		}
	}
	mario.Y -= valueY;
	mario.X += valueX;
	MapMovX = (mario.X + 6 - WIDTH / 2 >= 0 && mario.X + 6 - WIDTH / 2 > MapMovX ? mario.X + 6 - WIDTH / 2 : MapMovX);
	return 0;
}

void MovM_Safe(int valueX, int valueY) // MovM 실패 시 움직이는 값을 줄여서 다시 시도 
{
	if(!MovM(valueX, valueY)) // MovM 성공 
	{
		return; // ♡♡
	}
	while(!(valueX == 0 && valueY == 0)) // :(
	{
		if(!MovM((valueX > 0 ? --valueX : (valueX < 0 ? ++valueX : valueX)), (valueY > 0 ? --valueY : (valueY < 0 ? ++valueY : valueY))))
		{
			break; //성공시 
		}
	}
	return;
}

void goomba_move_thread(void *args) // 굼바 움직임 && 마리오&굼바 감지 
{
	int i, height, width, l = 0, BLOCKED = 0;
	while(running)
	{
		if(clock() - l < 30)
		{
			Sleep(1);
			continue;
		}
		l = clock();
		for(i = 0;i < 16;i++)
		{
			if(!(goomba[i].X >= MapMovX - WIDTH && goomba[i].X <= WIDTH + MapMovX + 14))
			{
				continue;
			}
			for(height = 0;height < 8;height++)
			{
				for(width = 0;width < 16;width++)
				{
					if(array_high[height + goomba[i].Y][width + goomba[i].X + goomba_move[i]] != 11 || array_low[height + goomba[i].Y][width + goomba[i].X + goomba_move[i]] != 11)
					{
						goomba_move[i] = (goomba_move[i] < 0 ? 1 : -1);
						width = 16;
						height = 8;
						BLOCKED = !BLOCKED;
					}
				}
			}
			if(BLOCKED) // GOTO 수정♡
			{
				BLOCKED = !BLOCKED;
				i--;
				continue;
			}
			goomba[i].X += goomba_move[i];
			for(height = 0;height < 8;height++)
			{
				for(width = 0;width < 16;width++)
				{
					if(array_high[height + goomba[i].Y + 1][width + goomba[i].X] != 11 || array_low[height + goomba[i].Y + 1][width + goomba[i].X] != 11)
					{
						width = 16;
						height = 8;
						BLOCKED = !BLOCKED;
					}
				}
			}
			if(BLOCKED)
			{
				BLOCKED = !BLOCKED;
				continue;
			}
			goomba[i].Y += 1;
		}
		
		//굼바가 마리오 만났쪙? 
		for(i = 0;i < 16;i++)
		{
			if(!(goomba[i].X >= MapMovX && goomba[i].X <= WIDTH + MapMovX - 128))
			{
				continue;
			}
			//마리오가 굼바 찌부시켰을때 :( 
			if(mario.X >= goomba[i].X - 15 && mario.X < goomba[i].X + 16 && mario.Y < goomba[i].Y && mario.Y > goomba[i].Y - 8)
			{
				if(play_sound_selection)
				{
					play_sound_selection = !play_sound_selection;
					_beginthread(play_sound, 0, (int)3);
				}
				else
				{
					play_sound_selection = !play_sound_selection;
					_beginthread(play_sound_, 0, (int)3);
				}
				goomba[i].X = -100;
				goomba[i].Y = -100;
				tmp = (tmp < 0 ? tmp + 19 : tmp);
			}
			else if(mario.X >= goomba[i].X - 14 && mario.X < goomba[i].X + 16 && mario.Y > goomba[i].Y - 8 && mario.Y < goomba[i].Y + 8)
			{
				GameOver(); //>_<
			}
		}
	}
}

void Input(void *args)
{
	int i = 0, j;
	while(running)
	{
		if(clock() - i < 25)
		{
			continue;
		}
		i = clock();
		if((GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8000) && CanIJump() && tmp == -7)
		{
			tmp = 22;
			if(play_sound_selection)
			{
				play_sound_selection = !play_sound_selection;
				_beginthread(play_sound, 0, (int)2);
			}
			else
			{
				play_sound_selection = !play_sound_selection;
				_beginthread(play_sound_, 0, (int)2);
			}
		}
		if(GetAsyncKeyState('A') & 0x8000)
		{
			MovM_Safe(-2, 0);
		}
		if(GetAsyncKeyState('S') & 0x8000)
		{
			//MovM(0, -2);
		}
		if(GetAsyncKeyState('D') & 0x8000)
		{
			//MapMovX++;
			MovM_Safe(2, 0);
		}
		if(tmp > -7)
		{
			tmp--;
			if(tmp > 0)
			{
				if(MovM(0, 2))
				{
					MovM(0, 1);
					tmp = -3;
				}
			}
		}
		else
		{
			Gravity();
		}
	}
}

void Gravity()
{
	MovM_Safe(0, -2);
}

int CanIJump()
{
	if(MovM(0, -1))
	{
		return 1;
	}
	MovM(0, 1);
	return 0;
}

void End()
{
	int k = 0, i, j;
	mciSendString("stop main.mp3", NULL, 0, NULL);
	mciSendString("play complete.wav", NULL, 0, NULL);
	while(1)
	{
		if(clock() - k < 30)
		{
			Sleep(1);
			continue;
		}
		k = clock();
		if(MovM(0, -1))
		{
			while(1)
			{
				if(clock() - k < 30)
				{
					continue;
				}
				if(mario.X > 3263)
				{
					break;
				}
				k = clock();
				mario.X++;
				Gravity();
			}
			//깃발&폭죽
			/*
			for(i = 0;i < 7;i++)
			{
				for(j = 0;j < 13;j++)
				{
					
				}
			}
			*/
		}
	}
}

void play_sound(int num) // 원활한 노래 출력을 위해 2개 만듦 
{
	if(num == 1)
	{
		mciSendString("play main.mp3", NULL, 0, NULL);
	}
	else if(num == 2)
	{
		mciSendString("play jump.wav", NULL, 0, NULL);
	}
	else if(num == 3)
	{
		mciSendString("play stomp.wav", NULL, 0, NULL);
	}
}

void play_sound_(int num)
{
	if(num == 2)
	{
		mciSendString("play jump_.wav", NULL, 0, NULL);
	}
	else if(num == 3)
	{
		mciSendString("play stomp_.wav", NULL, 0, NULL);
	}
}

void GameOver()
{
	int i, k = 0;
	running = !running;
	mciSendString("stop main.mp3", NULL, 0, NULL);
	mciSendString("play gameover.mp3", NULL, 0, NULL);
	for(i = 0;i < 10;i++)
	{
		while(clock() - k < 50)
		{
			Sleep(1);
		}
		k = clock();
		mario.Y -= 1;
	}
	while(mario.Y < HEIGHT)
	{
		if(clock() - k < 50)
		{
			Sleep(1);
			continue;
		}
		k = clock();
		mario.Y += 1;
	}
	while(clock() - k < 3000)
	{
		Sleep(1);
	}
	
}
