if (getSaveEntry("gameStarted")!=1)
{
	setPlayerLock(1);
	sleep(600);
	spawnNPC("Hometown/mom1.npc",7,12,0);
	sleep(200);
	print("Moving...");
	npcToLocation("mom1",4,4,1);
	print("done\n");
	movePlayer(1,0,0);
	interact();
}