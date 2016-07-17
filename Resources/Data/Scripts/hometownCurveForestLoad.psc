//----------Spawning Curvelinear Forest Blocker-------------- 

if (getSaveEntry("professorDead")==1)
	spawnTrainer("CurveForest/curveForestBlocker2.tnr",340,218,2);

else
	spawnNPC("CurveForest/curveForestBlocker1.npc",339,218,2);

//-----------------Spawning Officer-----------------------------------


if (getSaveEntry("professorDead")==1)
	spawnNPC("Hometown/officer2.npc",344,261,3);

elif (getSaveEntry("ReturntoProf")==1)
	spawnNPC("Hometown/officerHometown.npc",343,281,1);

else
	spawnNPC("Hometown/officer1.npc",344,261,0);

//--------------Spawning Sketchy Character---------------------------

if (getSaveEntry("professorDead")==1)
	spawnNPC("Hometown/sketchyCharacter2.npc",343,261,3);

else
	spawnNPC("Hometown/sketchyCharacter1.npc",338,229,2);

//-----------Spawning Hometown Thugs & Curve Forest thugs-------------------

if (getSaveEntry("professorDead")==1)
{
	spawnNPC("curveForestThug2.npc",315,193,2);
	spawnTrainer("curveForestThug1.tnr",369,239,2);
}

elif (getSaveEntry("professorDead")!=1 && getSaveEntry("aster1Done")==1)
{
spawnNPC("Hometown/hometownThug1.npc",370,239,0);
spawnNPC("Hometown/hometownThug2.npc",369,239,0);
}

//--------------Spawning Aster-------------------------

if (getSaveEntry("aster1Done")!=1)
	spawnTrainer("Hometown/aster1.tnr",335,269,3);
