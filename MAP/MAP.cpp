#include <stdio.h>

int main()
{
	int i, j;
	FILE *out;
	out = fopen("MAP_1.bin", "wb");
	for(i = 0;i < 96;i++)
	{
		for(j = 0;j < 3584;j++)
		{
			if(fputc(array_[i][j], out) == EOF)
			{
				return 1;
			}
		}
	}
	fclose(out);
	printf("%d %d\n", i, j);
	return 0;
}