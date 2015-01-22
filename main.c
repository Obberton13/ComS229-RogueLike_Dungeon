#include <stdio.h>
#include <stdlib.h>
#include "generator.h"

int **map;

int main(int argc, char *argv[])
{
	map = (int**) malloc(sizeof(int*)*160);
	int x;
	for(x=0;x<160;x++)
	{
		map[x] = (int*) malloc(sizeof(int)*96);
	}
	generateDungeon(map);
	free(map);
	return 0;
}
