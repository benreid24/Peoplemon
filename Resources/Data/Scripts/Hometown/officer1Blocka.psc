if (getSaveEntry("professorDead")!=1 && getSaveEntry("Delivered")!=1)
{
	setPlayerLock(1);
	npcToLocation("officer1",343,261,1);
	moveNPC("officer1",0);
	sleep(50);
	setPlayerLock(0);
}