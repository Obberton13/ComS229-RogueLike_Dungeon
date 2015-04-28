Project 1.10

I think the main things for attacking and defending changes can be found in push_monster(). It's what makes monsters go around each other if they need to, and attack the player. For the player, it would be move_player.

PC.cpp is the file that you want to go to for the item stuff. There are also a couple of functions in Dungeon.cpp that would help with this, like the drop, expunge, equip, and unequip actions.
