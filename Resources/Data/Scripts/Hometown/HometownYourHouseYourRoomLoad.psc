if (getSaveEntry("gameStarted")!=1)
{
	setPlayerLock(1);
	sleep(600);
	spawnNPC("Hometown/mom1.npc",7,13,0);
	sleep(200);
	npcToLocation("mom1",4,4,1);
	movePlayer(1,0,0);
	interact();
}