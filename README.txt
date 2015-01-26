Usage: ./DungeonGame [-s <seed value>]

All of the Dungeon Generation stuff is in "generator.c".

The basic method I used is make all of the rooms first, and add them to a list. Then I connected them by going to the first room I generated, declaring it as connected, then connecting the closest room to that, declaring it as connected. Then I would find the next closest unconnected room, and connected to the closest connected room to that unconnected room. The whole thing results in a bunch of rooms that are connected to each other. 

Important methods:
	generateRoom() generates each room
	connectAllRooms() connects each generated room together to get a dungeon.
		This was probably the hardest to figure out.
