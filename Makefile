DungeonGame: main.o generator.o
	gcc -Wall main.o generator.o

main.o: generator.h main.c
	gcc -Wall -Werror -ggdb main.c -c

generator.o: generator.h generator.c
	gcc -Wall -Werror -ggdb generator.c -c

clean:
	rm -f *.0 DungeonGame
