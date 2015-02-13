DungeonGame: main.o generator.o
	gcc -Wall -ggdb main.o generator.o -o DungeonGame

main.o: dungeon.h main.c
	gcc -Wall -Werror -ggdb main.c -c

generator.o: dungeon.h generator.c
	gcc -Wall -Werror -ggdb generator.c -c

clean:
	rm -f *.o DungeonGame

