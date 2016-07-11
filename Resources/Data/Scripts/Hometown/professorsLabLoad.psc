//----------------------Spawning the Professor---------------------------

if (getSaveEntry("defeatedAster1")==1)&&(getSaveEntry("professorDead")!=1)
	spawnNPC("Hometown/professor3.npc",12,4,0);

elif (getSaveEntry("Delivered")==1)&&(getSaveEntry("professorDead")!=1)
	spawnNPC("Hometown/professor2.npc",8,4,2);

elif (getSaveEntry("professorDead")!=1)
	spawnNPC("Hometown/professor1.npc",8,4,2);

//----------------------------------------------------------------------------