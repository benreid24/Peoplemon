if (hasItem(137) && playerDir()==2) {
	if (random(1,100) < 34) {
		messageBox("A Peoplemon noticed you had an Old Fishing Rod and threw itself at you!");
		//start battle
	}
	else if (random(0,10) < 4) {
		messageBox("You can see all kinds of Peoplemon in the water but they seem to be uninterested in your rod");
		messageBox("Maybe it's not big enough?");
	}
	else if (random(0,10) < 4) {
		messageBox("Nothing is biting. There must be something wrong with your stance");
	}
	else if (random(0,10) < 4) {
		messageBox("You feel something tugging at you!");
		messageBox("It's not a Peoplemon...");
		messageBox("Turns out it's your constant sense of worthlessness. You should have done the dishes before you left home");
	}
	else {
		messageBox("Something's on the line!");
		messageBox("Hey wait a minute");
		giveItem(44, playerName() + " got a log!");
	}
}
setPlayerLock(0);