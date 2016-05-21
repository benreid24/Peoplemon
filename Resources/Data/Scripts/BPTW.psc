sleep(1000);
setPlayerLock(1);


		if(playerYWorldPos==5)
		{
		npcToLocation("BPTW",7,4);
		moveNPC("BPTW",3);	
		sleep(2000);
		npcToLocation("BPTW",7,5);
		moveNPC("BPTW",3);
		movePlayer(1,0,1);
		interact;		
		}

		if(playerXWorldPos==5)
		{
		
		npcToLocation("BPTW",6,5);
		moveNPC("BPTW",0);
		sleep(2000);
		npcToLocation("BPTW",5,5);
		moveNPC("BPTW",0);
		movePlayer(2,0,1);
		interact;
		}
	else
	{
	npcToLocation("BPTW",6,5);
	moveNPC("BPTW",0);
	sleep(2000);
	npcToLocation("BPTW",7,5);
	moveNPC("BPTW",0);
	movePlayer(2,0,1);
	interact;
	}
setPlayerLock(0);
