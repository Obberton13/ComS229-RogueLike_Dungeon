DungeonGame: Dungeon.o Item.o DungeonTile.o NPC.o Dice.o NPCdef.o ItemDef.o PC.o
	g++ -Wall Dungeon.o Item.o DungeonTile.o NPC.o Dice.o NPCdef.o ItemDef.o PC.o -o DungeonGame -lncurses -lboost_regex

Dungeon.o: Dungeon.h Dungeon.cpp
	g++ -Wall -Werror -ggdb Dungeon.cpp -c

Item.o: Item.h Item.cpp
	g++ -Wall -Werror -ggdb Item.cpp -c

DungeonTile.o: DungeonTile.cpp DungeonTile.h
	g++ -Wall -Werror -ggdb DungeonTile.cpp -c

NPC.o: NPC.cpp NPC.h
	g++ -Wall -Werror -ggdb NPC.cpp -c

Dice.o: Dice.cpp Dice.h
	g++ -Wall -Werror -std=c++11 -ggdb Dice.cpp -c

NPCdef.o: NPCdef.cpp NPCdef.h
	g++ -Wall -Werror -std=c++11 -ggdb NPCdef.cpp -c

ItemDef.o: ItemDef.cpp ItemDef.h
	g++ -Wall -Werror -std=c++11 -ggdb ItemDef.cpp -c

PC.o: PC.h PC.cpp
	g++ -Wall -Werror -std=c++11 -ggdb PC.cpp -c

clean:
	rm -f *.o DungeonGame
