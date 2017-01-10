if (getSaveEntry("gameStarted")!=1)
{
	setPlayerLock(1);
	sleep(600);
	spawnNPC("Hometown/mom1.npc",7,12,0,1);
	sleep(200);
	npcToLocation("mom1",4,4,1);
	movePlayer(1,0,0);
	interact();
	//Player conversation will run a script to unlock player & add save entry above
}