DungeonGame: main.o generator.o
	gcc -Wall main.o generator.o -o DungeonGame

main.o: dungeon.h main.c
	gcc -Wall -Werror -ggdb main.c -c

generator.o: dungeon.h generator.c
	gcc -Wall -Werror -ggdb generator.c -c

heaptest: heaptest.o bheap.o
	gcc -Wall heaptest.o bheap.o -o heaptest.out

heaptest.o: heaptest.c bheap.h
	gcc -Wall -Werror -ggdb heaptest.c -c

bheap.o: bheap.c bheap.h
	gcc -Wall -Werror -ggdb bheap.c -c
clean:
	rm -f *.o DungeonGame

