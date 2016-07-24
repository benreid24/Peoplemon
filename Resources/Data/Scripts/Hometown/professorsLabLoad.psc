//----------------------Spawning the Professor---------------------------

if (getSaveEntry("aster1Done")==1 && getSaveEntry("professorDead")!=1)
	spawnNPC("Hometown/professor3.npc",12,4,0);

elif (getSaveEntry("Delivered")==1 && getSaveEntry("professorDead")!=1)
	spawnNPC("Hometown/professor2.npc",8,4,2);

elif (getSaveEntry("professorDead")!=1)
	spawnNPC("Hometown/professor1.npc",8,4,2);

//---------------------Spawning the Starters-------------------------------

if (getSaveEntry("gotStarter")!=1)

{
editTile(1,6,3,326);
editTile(1,5,3,326);
editTile(1,4,3,326);
}