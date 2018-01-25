	 spawnTrainer("Ghettopolis/ThugBossJordan.tnr",10,7,1,1);
	 spawnNPC("Ghettopolis/GProf.npc",11,7,3,1);
     playerToLocation(8,9,0);
	 
		sleep(2000);
		messageBox("For the last time, GIVE ME THE RECIPE OLD MAN!");
		sleep(2000);
		moveTrainer("ThugBossJordan",2);
		messageBox("How'd you get in here?");
		string answer = choiceBox("Whatever. Just scram before you get hurt kid.","No","Okay");
		if (answer == "No")
		{
		 sleep(1000);
		 trainerToLocation("ThugBossJordan",8,11,0);
		 moveTrainer("ThugBossJordan",2);
		 messageBox("Okay kid but it's your funeral");
		 startTrainerBattle("Ghettopolis/ThugBossJordan.tnr");
		}
		if (answer == "Okay")
		{
		addSaveEntry("Okay",1);
		setPlayerLock(0);
		}