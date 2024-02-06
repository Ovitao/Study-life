Game uses the FLTK library.
	Input through text box.
	The map of the cave is schlegel diagram of dodecahedron.
	As you play it draws part of cave that has already been discovered.
	You can mark visible rooms with commands. (The help section is included at the start of the game).
	With the command "show_cave" you can ask the game to draw the entire cave;
	You can see the location of the hazards with the command "state_of_cave" (this will be displayed in the console);

	Features: If you enter a room with a bottomless pit and a Giant Bat is there, it will drag you to another room and you won't die;
		If whole cave is drawn, you can mark any room.
		If you shoot in room that is not nearby.  The arrow will ricochet into a random room and a suitable message will be shown. 
		After the giant bat catches you, it will fly to another room. the room where the bat caught you will not be marked as discovered.
		The shooting range is up to 3 rooms;