Usage: ./DungeonGame [-s <seed value>]

Saving and loading:
all of the important methods are in main.c:
open_file() opens a file,
save_dungeon() saves the dungeon.
load_dungeon() loads the dungeon.

I noticed that two cells in the lower right corner were behaving strangely when loading from the sample maps. I made sure to make sure that didn't happen any more.
