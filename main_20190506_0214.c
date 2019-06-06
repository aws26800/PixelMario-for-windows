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
struct Pos // mario -> no flag, goomba -> flag:goomba_move(x), QuestionMarkBox -> flag:enabled?
{
	int X;
	int Y;
	int flag;
} mario, goomba[16], Q[11];

//Type WORD
WORD color[HEIGHT * WIDTH];

//Type int
int MapMovX = 1;
int end = 1;
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
char Q_texture_high[8][16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0, 14, 0, 0, 14, 14, 14, 12, 12, 0, 0, 0, 12, 12, 14, 14, 14, 14, 0, 0, 14, 14, 14, 12, 12, 0, 14, 14, 12, 12, 0, 14, 14, 14, 0, 0, 14, 14, 14, 14, 14, 14, 12, 12, 0, 0, 0, 14, 14, 14, 0, 0, 14, 14, 14, 14, 14, 14, 14, 0, 0, 14, 14, 14, 14, 14, 0, 0, 14, 14, 14, 14, 14, 14, 12, 12, 0, 14, 14, 14, 14, 14, 0, 0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0};
char Q_texture_low[8][16] = {0, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0, 0, 14, 14, 14, 14, 12, 12, 12, 12, 12, 14, 14, 14, 14, 14, 0, 0, 14, 14, 14, 12, 12, 0, 14, 14, 12, 12, 0, 14, 14, 14, 0, 0, 14, 14, 14, 14, 0, 0, 14, 12, 12, 12, 0, 14, 14, 14, 0, 0, 14, 14, 14, 14, 14, 14, 12, 12, 0, 14, 14, 14, 14, 14, 0, 0, 14, 14, 14, 14, 14, 14, 12, 12, 14, 14, 14, 14, 14, 14, 0, 0, 14, 0, 14, 14, 14, 14, 14, 0, 0, 14, 14, 14, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char Q_eaten_texture_high[8][16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0};
char Q_eaten_texture_low[8][16] = {0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 12, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char path[50];
char tmp_high[8][16], tmp_low[8][16];

//Function
void Coloring(int order);
void setup();
void Write();
int MovM(int valueX, int valueY);
void MovM_Safe(int valueX, int valueY);
int CanIJump();
void End();
void LoadMap();
void GameOver();
void flag_down();
void check_Q();
void MovQ();

//Thread & HANDLE
void Input(void *args);
void goomba_move_thread(void *args);
void play_sound(int num);
void play_sound_(int num);

int main(int argc, char **argv)
{
	int i, j = 0, k = 0, delay = 30, calcfps1 = 0, calcfps2 = 0, height, width;
	char c[10], text[20] = "title FPS : ", cat[20];
	LoadMap();
	setup();
	i = clock();
	j = clock();
	k = clock();
	while(1) // FPS ǥ�� && FPS ���� 
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
			if(calcfps1 - calcfps2 >= 32)
			{
				delay++;
			}
			else if(calcfps1 - calcfps2 <= 30)
			{
				delay--;
			}
			calcfps2 = calcfps1;
			strcat(cat, text);
			strcat(cat, c);
			system(cat);
			i = clock();
		}
		if(clock() - k > 500 && running) // ���� �����̴� ��� 
		{
			for(height = 0;height < 8;height++)
			{
				for(width = 0;width < 16;width++)
				{
					tmp_high[height][width] = goomba_texture_high[height][width];
					tmp_low[height][width] = goomba_texture_low[height][width];
				}
			}
			for(height = 0;height < 8;height++)
			{
				for(width = 0;width < 16;width++)
				{
					goomba_texture_high[height][width] = tmp_high[height][15 - width];
					goomba_texture_low[height][width] = tmp_low[height][15 - width];
				}
			}
			k = clock();
		}
		while(clock() - j < delay)
		{
			Sleep(1); 
		}
		j = clock();
	}
	
}

void LoadMap() // �� �ε� 
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
void Coloring(int order) // color�迭�� ���� �ֱ� // color = 15 | 14 << 4 -> color >> 4 = 14 -> color ^ ((color >> 4) << 4) = 15
{
	int width, height, i, j;
	for(height = 0;height < HEIGHT;height++)
	{
		for(width = 0 + MapMovX;width < MapMovX + WIDTH;width++)
		{
			color[height * WIDTH + width - MapMovX] = (array_high[height][width] == 16 ? 11 : array_high[height][width]) | (array_low[height][width] == 16 ? 11 : array_low[height][width]) << 4;
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
				color[(goomba[i].Y + height) * WIDTH + goomba[i].X + width - MapMovX] = (goomba_texture_high[height][width] != 11 ? goomba_texture_high[height][width] : (color[(goomba[i].Y + height) * WIDTH + goomba[i].X + width - MapMovX] ^ (color[(goomba[i].Y + height) * WIDTH + goomba[i].X + width - MapMovX] >> 4) << 4)) | (goomba_texture_low[height][width] != 11 ? goomba_texture_low[height][width] : color[(goomba[i].Y + height) * WIDTH + goomba[i].X + width - MapMovX] >> 4) << 4;
			}
		}
	}
	for(i = 0;i < 11;i++)
	{
		if(!(Q[i].X >= MapMovX && Q[i].X <= WIDTH + MapMovX - 16))
		{
			continue;
		}
		for(height = 0;height < 8;height++)
		{
			for(width = 0;width < 16;width++)
			{
				color[(Q[i].Y + height) * WIDTH + Q[i].X + width - MapMovX] = (Q[i].flag ? Q_texture_high[height][width] : Q_eaten_texture_high[height][width]) | (Q[i].flag ? Q_texture_low[height][width] : Q_eaten_texture_low[height][width]) << 4;
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
			color[(height / 2 + mario.Y) * WIDTH + width + mario.X - MapMovX] = (mario_texture[height][width] != 16 && mario_texture[height][width] != 11 ? mario_texture[height][width] : (color[(height / 2 + mario.Y) * WIDTH + width + mario.X - MapMovX] ^ (color[(height / 2 + mario.Y) * WIDTH + width + mario.X - MapMovX] >> 4) << 4)) | (mario_texture[height + 1][width] != 16 && mario_texture[height + 1][width] != 11 ? mario_texture[height + 1][width] : color[(height / 2 + mario.Y) * WIDTH + width + mario.X - MapMovX] >> 4) << 4;
		}
	}
}

void Write() // color�� ������ 
{
	COORD coord = {0, 0};
	DWORD written[HEIGHT * WIDTH];
	WriteConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color, WIDTH * HEIGHT, coord, written);
}

void setup() // �⺻�¾� 
{
	int width, height, i, j;
	mario.X = 0;
	mario.Y = 72;
	printf("���ǻ��� : \n��CPU ��뷮�� ����� �Ƹ��ٿ� �� �ֽ��ϴ٢�\n�̴� �� ���α׷��� ȭ�鿡 ������ִ� �ܼ�ȣ��Ʈ�� �Դ°��Դϴ�.\n\n�ܼ� �۾� ����� 8��Ʈ�� �������ּ���.\n\"���ǻ���\"�� '��'�� �ٷ� ���� \"�������α׷�\" �������� Ŭ�����ּ���.\n�Ӽ�-�۲�-ũ�⸦ 8���� ����\n\n�� ���α׷��� 1920*1080 100������ ����ȭ �Ǿ��ֽ��ϴ�.\n125���������� ȣȯ�Ǵ°��� Ȯ�� �Ͽ����� 100�������� �����ϴ°��� ��õ�帳�ϴ�.\n������Ű-����-�ý���-���÷���-�ؽ�Ʈ, �� �� ��Ÿ �׸��� ũ�� ���� -> 100%���� ����\n\n�ƹ�Ű�� ���� ����ϼ���!");
	_getch();
	system("mode 256, 97");
	_setmode(_fileno(stdout), _O_U16TEXT); // ��°� UTF-16���� ���� 
	for(height = 0;height < HEIGHT;height++)
	{
		for(width = 0;width < WIDTH;width++)
		{
			wprintf(L"\u2580"); // ȭ�鿡 ���� ��� 
		}
		wprintf(L"\n");
	}
	for(i = 0;i < 16;i++) // ���ټ��� 
	{
		if(i != 4 && i != 5)
		{
			goomba[i].Y = 72;
		}
		else
		{
			goomba[i].Y = 8;
		}
		goomba[i].flag = -1;
	}
	goomba[0].X = 352; // ���ټ��� 
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
	for(i = 0;i < 11;i++)
	{
		if(i != 1 && i != 3 && i != 5 && i != 8 && i != 9)
		{
			Q[i].Y = 48;
		}
		else
		{
			Q[i].Y = 16;
		}
		Q[i].flag = 1;
	}
	Q[0].X = 256;
	Q[1].X = 352;
	Q[2].X = 368;
	Q[3].X = 1504;
	Q[4].X = 1696;
	Q[5].X = 1744;
	Q[6].X = 1744;
	Q[7].X = 1792;
	Q[8].X = 2064;
	Q[9].X = 2080;
	Q[10].X = 2720;
	running = 1;
	_beginthread(play_sound, 0, (int)1); // ���� ��� 
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
			if(height + mario.Y - valueY > HEIGHT - 1) // �������� ���ۿ� ���������� >_< 
			{
				GameOver();
			}
			if((array_high[height + mario.Y][width + mario.X + 1] == 6 || array_low[height + mario.Y][width + mario.X + 1] == 6) && end) // �������� ������ ������ 
			{
				end = !end;
				End(); // ����! 
			}
			if(!end)
			{
				if(array_high[height + mario.Y - valueY][width + mario.X + valueX] == 0 || array_low[height + mario.Y - valueY][width + mario.X + valueX] == 0)
				{
					return 1; // ��߶����� ��Ҷ� �ٸ��� ó���� ����� ����� ������ �� ������ 
				}
			}
			else if(array_high[height + mario.Y - valueY][width + mario.X + valueX] != 11 || array_low[height + mario.Y - valueY][width + mario.X + valueX] != 11 || height + mario.Y - valueY < 0 || width + mario.X + valueX < 0 || mario.X + valueX - MapMovX < 0)
			{
				if(array_high[height + mario.Y - valueY][width + mario.X + valueX] != 6 || array_low[height + mario.Y - valueY][width + mario.X + valueX] != 6 || height + mario.Y - valueY < 0 || width + mario.X + valueX < 0 || mario.X + valueX - MapMovX < 0)
				{
					if(valueY > 0)
					{
						check_Q();
					}
					return 1; // �� ������ �����°� ���� 
				}
			}
		}
	}
	mario.Y -= valueY;
	mario.X += valueX;
	MapMovX = (mario.X + 6 - WIDTH / 2 >= 0 && mario.X + 6 - WIDTH / 2 > MapMovX ? mario.X + 6 - WIDTH / 2 : MapMovX);
	return 0;
}

void MovM_Safe(int valueX, int valueY) // MovM ���� �� �����̴� ���� �ٿ��� �ٽ� �õ� 
{
	if(!MovM(valueX, valueY)) // MovM ���� 
	{
		return; // ����
	}
	while(!(valueX == 0 && valueY == 0)) // :(
	{
		if(!MovM((valueX > 0 ? --valueX : (valueX < 0 ? ++valueX : valueX)), (valueY > 0 ? --valueY : (valueY < 0 ? ++valueY : valueY))))
		{
			break; //������ 
		}
	}
	return;
}

void goomba_move_thread(void *args) // ���� ������ && ������&���� ���� 
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
					if(array_high[height + goomba[i].Y][width + goomba[i].X + goomba[i].flag] != 11 || array_low[height + goomba[i].Y][width + goomba[i].X + goomba[i].flag] != 11)
					{
						goomba[i].flag = (goomba[i].flag < 0 ? 1 : -1);
						width = 16;
						height = 8;
						BLOCKED = !BLOCKED;
					}
				}
			}
			if(BLOCKED) // GOTO ������
			{
				BLOCKED = !BLOCKED;
				i--;
				continue;
			}
			goomba[i].X += goomba[i].flag;
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
		
		//���ٰ� ������ ������? 
		for(i = 0;i < 16;i++)
		{
			if(!(goomba[i].X >= MapMovX && goomba[i].X <= WIDTH + MapMovX - 128))
			{
				continue;
			}
			//�������� ���� ��ν������� :( 
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
				goomba[i].X = -200;
				goomba[i].Y = -200;
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
			Sleep(1);
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
			MovM_Safe(0, -2);
		}
	}
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
				MovM_Safe(0, -2);
			}
			//���&����
			/*
			for(i = 0;i < 7;i++)
			{
				for(j = 0;j < 13;j++)
				{
					
				}
			}
			*/
		}
		else
		{
			flag_down();
		}
	}
}

void play_sound(int num) // ��Ȱ�� �뷡 ����� ���� 2�� ���� 
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

void flag_down()
{
	int height, width;
	for(height = HEIGHT - 1;height >= 0;height--)
	{
		for(width = 0 + MapMovX;width < MapMovX + WIDTH;width++)
		{
			if(array_high[height][width] == 15 || array_high[height][width] == 2)
			{
				array_high[height + 2][width] = array_high[height][width];
				array_high[height][width] = 11;
			}
			if(array_low[height][width] == 15 || array_low[height][width] == 2)
			{
				array_low[height + 2][width] = array_low[height][width];
				array_low[height][width] = 11;
			}
		}
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
		mario.Y += 2;
	}
	while(clock() - k < 3000)
	{
		Sleep(1);
	}
	while(1)
	{
		Sleep(10000);
	} 
}

void check_Q()
{
	int i, height, width;
	for(i = 0;i < 11;i++)
	{
		if(!(Q[i].X >= MapMovX && Q[i].X <= WIDTH + MapMovX - 16))
		{
			continue;
		}
		if(Q[i].flag && mario.X > Q[i].X - 11 && mario.X < Q[i].X + 11 && mario.Y <= Q[i].Y + 8 && mario.Y > Q[i].Y)
		{
			Q[i].flag = 0;
			_beginthread(MovQ, 0, (int)i);
		}
	}
}

void MovQ(int i)
{
	int k = clock();
	Q[i].Y -= 1;
	while(1)
	{
		if(clock() - k > 50)
		{
			k = clock();
			break;
		}
		Sleep(1);
	}
	Q[i].Y -= 1;
	while(1)
	{
		if(clock() - k > 50)
		{
			k = clock();
			break;
		}
		Sleep(1);
	}
	Q[i].Y += 1;
	while(1)
	{
		if(clock() - k > 50)
		{
			k = clock();
			break;
		}
		Sleep(1);
	}
	Q[i].Y += 1;
}
