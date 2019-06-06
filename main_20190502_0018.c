#include <stdio.h>
#include <fcntl.h>
#include <Windows.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <io.h>
#include <process.h>

#define WIDTH 256
#define HEIGHT 96
#define FULL_WIDTH 3584

//1 Block = 16
struct Pos
{
	int X;
	int Y;
} mario;
//Type WORD
WORD color[HEIGHT * WIDTH];

//Type int
int MapMovX = 1;
int end = 1;

//Type char
char flag_high[7][13] = {11, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 15, 15, 15, 15, 15, 12, 15, 15, 15, 15, 15, 11, 12, 15, 15, 12, 12, 12, 12, 12, 12, 12, 15, 15, 11, 12, 15, 15, 15, 15, 12, 12, 12, 15, 15, 15, 15, 11, 12, 15, 15, 15, 12, 15, 15, 15, 12, 15, 15, 15, 11, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11};
char flag_low[7][13] = {14, 14, 14, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 11, 12, 15, 15, 15, 15, 15, 12, 15, 15, 15, 15, 15, 11, 12, 15, 15, 15, 12, 12, 12, 12, 12, 15, 15, 15, 11, 12, 15, 15, 15, 12, 12, 15, 12, 12, 15, 15, 15, 11, 12, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 11, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11};
char mario_texture[16][12] = {16, 16, 16, 12, 12, 12, 12, 12, 16, 16, 16, 16, 16, 16, 12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 16, 16, 6, 6, 6, 14, 14, 6, 14, 16, 16, 16, 16, 6, 14, 6, 14, 14, 14, 6, 14, 14, 14, 16, 16, 6, 14, 6, 6, 14, 14, 14, 6, 14, 14, 14, 16, 6, 6, 14, 14, 14, 14, 6, 6, 6, 6, 16, 16, 16, 16, 14, 14, 14, 14, 14, 14, 14, 16, 16, 16, 16, 6, 6, 12, 6, 6, 6, 16, 16, 16, 16, 16, 6, 6, 6, 12, 6, 6, 12, 6, 6, 6, 16, 6, 6, 6, 6, 12, 12, 12, 12, 6, 6, 6, 6, 14, 14, 6, 12, 14, 12, 12, 14, 12, 6, 14, 14, 14, 14, 14, 12, 12, 12, 12, 12, 12, 14, 14, 14, 14, 14, 12, 12, 12, 12, 12, 12, 12, 12, 14, 14, 16, 16, 12, 12, 12, 16, 16, 12, 12, 12, 16, 16, 16, 6, 6, 6, 16, 16, 16, 16, 6, 6, 6, 16, 6, 6, 6, 6, 16, 16, 16, 16, 6, 6, 6, 6};


//Function
void Coloring(int order);
void setup();
void Write();
int MovM(int valueX, int valueY);
void Gravity();
void MovM_Safe(int valueX, int valueY);
int CanIJump();
void End();

//Thread & HANDLE
void Input(void *args);


int main()
{
	int i, j = 0, delay = 30, calcfps1 = 0, calcfps2 = 0;
	char c[10], text[20] = "title FPS : ", cat[20];
	mario.X = 0;
	mario.Y = 72;
	setup();
	_beginthread(Input, 0, (void*)0);
	i = clock();
	j = clock();
	while(1)
	{
		Coloring(0);
		Write();
		calcfps1++;
		if(MapMovX + WIDTH > FULL_WIDTH)
		{
			return -1;
		}
		if(clock() - i > 1000)
		{
			cat[0] = '\0';
			itoa((calcfps1 - calcfps2), c, 10);
			if(calcfps1 - calcfps2 > 16)
			{
				delay++;
			}
			else if(calcfps1 - calcfps2 < 15)
			{
				delay--;
			}
			calcfps2 = calcfps1;
			strcat(cat, text);
			strcat(cat, c);
			system(cat);
			i += 500;
		}
		while(clock() - j < delay)
		{
			Sleep(1);
		}
		j = clock();
	}
	
}

//height 79 (158) -> floor 
void Coloring(int order)
{
	int width, height, i, j;
	for(height = 0;height < HEIGHT;height++)
	{
		for(width = 0 + MapMovX;width < MapMovX + WIDTH;width++)
		{
			color[height * WIDTH + width - MapMovX] = (array_high[height][width] == 16 ? 0 : array_high[height][width]) | (array_low[height][width] == 16 ? 0 : array_low[height][width]) << 4;
		}
	}
	for(height = 0;height < 16;height += 2)
	{
		for(width = 0;width < 12;width++)
		{
			if((height / 2 + mario.Y) * WIDTH + width + mario.X - MapMovX >= HEIGHT * WIDTH - 1)
			{
				continue;
			}
			if(width + mario.X > 3263)
			{
				continue;
			}
			color[(height / 2 + mario.Y) * WIDTH + width + mario.X - MapMovX] = (mario_texture[height][width] != 16 ? mario_texture[height][width] : array_high[height / 2 + mario.Y][width + mario.X - MapMovX]) | (mario_texture[height + 1][width] != 16 ? mario_texture[height + 1][width] : array_low[height / 2 + mario.Y][width + mario.X - MapMovX]) << 4;
		}
	}
}

void Write()
{
	COORD coord = {0, 0};
	DWORD written[HEIGHT * WIDTH];
	WriteConsoleOutputAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color, WIDTH * HEIGHT, coord, written);
}

void setup()
{
	int width, height, x, y;
	printf("���ǻ��� : \nCPU ��뷮�� ����� �Ƹ��ٿ� �� �ֽ��ϴ�.\n�̴� �� ���α׷��� ȭ�鿡 ������ִ� �ܼ�ȣ��Ʈ�� �Դ°��Դϴ�.\n\n�ܼ� �۾� ����� 8��Ʈ�� �������ּ���.\n\"���ǻ���\"�� '��'�� �ٷ� ���� \"�������α׷�\" �������� Ŭ�����ּ���.\n�Ӽ�-�۲�-ũ�⸦ 8���� ����\n\n�� ���α׷��� 1920*1080 100������ ����ȭ �Ǿ��ֽ��ϴ�.\n125���������� ȣȯ�Ǵ°��� Ȯ�� �Ͽ����� 100�������� �����ϴ°��� ��õ�帳�ϴ�.\n������Ű-����-�ý���-���÷���-�ؽ�Ʈ, �� �� ��Ÿ �׸��� ũ�� ���� -> 100%���� ����\n\n�ƹ�Ű�� ���� ����ϼ���!");
	_getch();
	system("mode 256, 97");
	_setmode(_fileno(stdout), _O_U16TEXT);
	for(height = 0;height < HEIGHT;height++)
	{
		for(width = 0;width < WIDTH;width++)
		{
			wprintf(L"\u2580");
		}
		wprintf(L"\n");
	}
}
//3264
int MovM(int valueX, int valueY)
{
	int width, height;
	for(height = 0;height < 8;height++)
	{
		for(width = 0;width < 12;width++)
		{
			if(height + mario.Y - valueY > HEIGHT - 1)
			{
				exit(0);
			}
			if((array_high[height + mario.Y][width + mario.X + 1] == 6 || array_low[height + mario.Y][width + mario.X + 1] == 6) && end)
			{
				end = !end;
				End();
			}
			if(array_high[height + mario.Y - valueY][width + mario.X + valueX] != 11 || array_low[height + mario.Y - valueY][width + mario.X + valueX] != 11 || height + mario.Y - valueY < 0 || width + mario.X + valueX < 0 || mario.X + valueX - MapMovX < 0)
			{
				if(array_high[height + mario.Y - valueY][width + mario.X + valueX] != 6 || array_low[height + mario.Y - valueY][width + mario.X + valueX] != 6 || height + mario.Y - valueY < 0 || width + mario.X + valueX < 0 || mario.X + valueX - MapMovX < 0)
				{
					return 1;
				}
			}
		}
	}
	mario.Y -= valueY;
	mario.X += valueX;
	MapMovX = (mario.X + 6 - WIDTH / 2 >= 0 && mario.X + 6 - WIDTH / 2 > MapMovX ? mario.X + 6 - WIDTH / 2 : MapMovX);
	return 0;
}

void MovM_Safe(int valueX, int valueY)
{
	if(!MovM(valueX, valueY))
	{
		return;
	}
	while(!(valueX == 0 && valueY == 0))
	{
		if(!MovM((valueX > 0 ? --valueX : (valueX < 0 ? ++valueX : valueX)), (valueY > 0 ? --valueY : (valueY < 0 ? ++valueY : valueY))))
		{
			break;
		}
	}
	return;
}

void Input(void *args)
{
	int i = 0, j, tmp = 0;
	while(1)
	{
		if(clock() - i < 18)
		{
			continue;
		}
		i = clock();
		if((GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8000) && CanIJump() && tmp == -7)
		{
			tmp = 22;
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
				Gravity();
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
	}
}