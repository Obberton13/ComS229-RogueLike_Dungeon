DungeonGame: main.o generator.o pathfinding.o
	gcc -Wall -ggdb main.o generator.o pathfinding.o -o DungeonGame

main.o: dungeon.h main.c
	gcc -Wall -Werror -ggdb main.c -c

generator.o: dungeon.h generator.c
	gcc -Wall -Werror -ggdb generator.c -c

pathfinding.o: dungeon.h pathfinding.c
	gcc -Wall -Werror -ggdb pathfinding.c -c

clean:
	rm -f *.o DungeonGame

