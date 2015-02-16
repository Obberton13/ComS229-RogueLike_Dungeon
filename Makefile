DungeonGame: main.o generator.o pathfinding.o bheap.o
	gcc -Wall main.o generator.o pathfinding.o bheap.o -o DungeonGame

main.o: dungeon.h main.c
	gcc -Wall -Werror -ggdb main.c -c

generator.o: dungeon.h generator.c
	gcc -Wall -Werror -ggdb generator.c -c

pathfinding.o: pathfinding.c bheap.h dungeon.h pathfinding.h
	gcc -Wall -Werror -ggdb pathfinding.c -c

bheap.o: bheap.c bheap.h
	gcc -Wall -Werror -ggdb bheap.c -c

clean:
	rm -f *.o DungeonGame
