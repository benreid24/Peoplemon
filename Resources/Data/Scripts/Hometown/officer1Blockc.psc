if (getSaveEntry("professorDead")!=1 && getSaveEntry("Delivered")!=1)
{
	setPlayerLock(1);
	npcToLocation("officer1",345,261,0);
	moveNPC("officer1",0);
	setPlayerLock(0);
}