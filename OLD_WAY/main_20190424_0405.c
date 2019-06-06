#include <stdio.h>
#include <fcntl.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <io.h>
#include <process.h>

#define WIDTH 256
#define HEIGHT 96
#define FULL_WIDTH 3584

//1 Block = 16

//Type WORDd
WORD color[HEIGHT * WIDTH];

//Type int
int MapMovX = 0;

//Type char
char E_Location[192][3584] = {0};
char E_Location_B[192][3584] = {0};


//Function
void Coloring(int order);
void setup();
void Write();
void MovE(int value);

//Thread & HANDLE
void Input(void *args);

int main()
{
	int i, j = 0, delay = 30, calcfps1 = 0, calcfps2 = 0;
	char c[10], text[20] = "title FPS : ", cat[20];
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
			return 0;
		}
		if(clock() - i > 500)
		{
			cat[0] = '\0';
			itoa((calcfps1 - calcfps2) * 2, c, 10);
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
			color[height * WIDTH + width - MapMovX] = (array_high[height][width] == 16 ? 0 : array_high[height][width]) | (array_low[height][width] == 16 ? 0 : array_low[height][width]) * 16;
		}
	}
	for(height = 0;height < HEIGHT * 2;height += 2)
	{
		for(width = 0 + MapMovX;width < MapMovX + WIDTH;width++)
		{
			color[height / 2 * WIDTH + width - MapMovX] = (E_Location[height][width] == 1 ? 0 : color[height / 2 * WIDTH + width - MapMovX]);
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
	printf("���ǻ��� : \n\CPU ��뷮�� ����� �Ƹ��ٿ� �� �ֽ��ϴ�.\n�̴� �� ���α׷��� ȭ�鿡 ������ִ� �ܼ�ȣ��Ʈ�� �Դ°��Դϴ�.\n\n�ܼ� �۾� ����� 8��Ʈ�� �������ּ���.\n\"���ǻ���\"�� '��'�� �ٷ� ���� \"�������α׷�\" �������� Ŭ�����ּ���.\n�Ӽ�-�۲�-ũ�⸦ 8���� ����\n\n�� ���α׷��� 1920*1080 100������ ����ȭ �Ǿ��ֽ��ϴ�.\n125���������� ȣȯ�Ǵ°��� Ȯ�� �Ͽ����� 100�������� �����ϴ°��� ��õ�帳�ϴ�.\n������Ű-����-�ý���-���÷���-�ؽ�Ʈ, �� �� ��Ÿ �׸��� ũ�� ���� -> 100%���� ����\n\n�ƹ�Ű�� ���� ����ϼ���!");
	_getch();
	system("mode 256, 97");
	_setmode(_fileno(stdout), _O_U16TEXT);
	for(height = 143;height <= 158;height++)
	{
		for(width = 0;width <= 15;width++)
		{
			E_Location[height][width] = 1;
		}
	}
	for(height = 0;height < HEIGHT;height++)
	{
		for(width = 0;width < WIDTH;width++)
		{
			wprintf(L"\u2580");
		}
		wprintf(L"\n");
	}
}

void MovE(int value)
{
	int width, height, check = 1;
	for(height = 0;height < HEIGHT * 2;height += 2)
	{
		for(width = FULL_WIDTH;width >= 0;width--)
		{
			E_Location_B[height][width] = E_Location[height][width];
		}
	}
	if(value > 0)
	{
		for(height = 0;height < HEIGHT * 2;height += 2)
		{
			for(width = FULL_WIDTH;width >= 0;width--)
			{
				if(E_Location[height][width] == 1)
				{
					if(check)
					{
						if(array_high[height / 2][width + value] != 0)
						{
							E_Location[height][width + value] = 1;
							E_Location[height][width] = 0;
						}
						else
						{
							for(height = 0;height < HEIGHT * 2;height += 2)
							{
								for(width = WIDTH;width >= 0;width--)
								{
									E_Location[height][width] = E_Location_B[height][width];
								}
							}
							return;
						}
					}
					else
					{
						if(array_low[height / 2][width + value] != 0)
						{
							E_Location[height][width + value] = 1;
							E_Location[height][width] = 0;
						}
						else
						{
							for(height = 0;height < HEIGHT * 2;height += 2)
							{
								for(width = WIDTH;width >= 0;width--)
								{
									E_Location[height][width] = E_Location_B[height][width];
								}
							}
							return;
						}
					}
				}
			}
			check = !check;
		}
	}
	else if(value < 0)
	{
		for(height = 0;height < HEIGHT * 2;height += 2)
		{
			for(width = 0;width < FULL_WIDTH;width++)
			{
				if(E_Location[height][width] == 1)
				{
					if(check)
					{
						if(array_high[height / 2][width + value] != 0 && width + value >= MapMovX)
						{
							E_Location[height][width + value] = 1;
							E_Location[height][width] = 0;
						}
						else
						{
							for(height = 0;height < HEIGHT * 2;height += 2)
							{
								for(width = WIDTH;width >= 0;width--)
								{
									E_Location[height][width] = E_Location_B[height][width];
								}
							}
							return;
						}
					}
					else
					{
						if(array_low[height / 2][width + value] != 0 && width + value >= MapMovX)
						{
							E_Location[height][width + value] = 1;
							E_Location[height][width] = 0;
						}
						else
						{
							for(height = 0;height < HEIGHT * 2;height += 2)
							{
								for(width = WIDTH;width >= 0;width--)
								{
									E_Location[height][width] = E_Location_B[height][width];
								}
							}
							return;
						}
					}
				}
			}
			check = !check;
		}
	}
	for(height = 0;height < HEIGHT * 2;height += 2)
	{
		for(width = 0;width < FULL_WIDTH;width++)
		{
			if(E_Location[height][width] == 1)
			{
				if(width - MapMovX > WIDTH / 2)
				{
					MapMovX++;
				}
			}
		}
	}
}

void Input(void *args)
{
	int i;
	while(1)
	{
		i = clock();
		if(GetAsyncKeyState('W') & 0x8000)
		{
			
		}
		if(GetAsyncKeyState('A') & 0x8000)
		{
			//MapMovX = (MapMovX <= 0 ? MapMovX : MapMovX - 1);
			MovE(-1);
		}
		if(GetAsyncKeyState('S') & 0x8000)
		{
			
		}
		if(GetAsyncKeyState('D') & 0x8000)
		{
			//MapMovX++;
			MovE(1);
		}
		while(clock() - i <= 3)
		{
			Sleep(1);
		}
	}
}