#include<stdio.h>
#include"generator.h"

int map[160][96];

int main(int argc, char *argv[])
{
	generateDungeon(map);
	return 0;
}
