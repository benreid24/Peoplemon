//check saved strings and remove npcs/tiles
if (getSaveEntry("puzzles:educationOpened") == 1)
	runScript("Puzzles/removeEducation.psc", 0);