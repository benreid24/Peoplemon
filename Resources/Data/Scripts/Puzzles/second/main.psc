int main() {
	int px = playerXWorldPos()/32;
	int py = playerYWorldPos()/32 - 1;

	sleep(1200);
	movePlayer(1, 0, 1);

	if (getSaveEntry("puzzles:secondDone") == 1) {
		messageBox("What are you doing back here kid?", "We're waiting for our next potential victim, get lost");
		return 0;
	}
	else {
		messageBox("Hello, my name is Juan", "This puzzle requires that you stand very still");
		messageBox("Don't worry about the temptation of moving, I've already frozen your legs in place");
		messageBox("You will be asked a series of questions by my associates here");
		messageBox("Answer them all correctly and you will gifted enough strength to lift exactly one dumbbell. No more no less");
		messageBox("Answer wrong... and POW! You're gone");
		messageBox("Don't worry though, you can come back and try again");
		messageBox("Now get ready. Question 1 Asker, approach!");
		sleep(600);
		
		int nx = npcXPos("Q1");
		int ny = npcYPos("Q1");
		npcToLocation("Q1", px, py, 1);
		sleep(750);
		moveNPC("Q1", 2);
		movePlayer(0, 0, 1);
		
		messageBox("Question 1");
		if (choiceBox("What town do you live in?", "Littleroot", "HomeTown", "AwayTown", "Ghettopolis", "Iceburg") != "HomeTown") {
			messageBox("FALSE! You're done for kid");
			return 0;
		}
		messageBox("Correct. Next question!");
		npcToLocation("Q1", nx, ny, 1);
		sleep(750);
		moveNPC("Q1", 1);
	}
	
	return 0;
}

main();
setPlayerLock(0);
loadMap("Puzzles/mainRoom", 3);