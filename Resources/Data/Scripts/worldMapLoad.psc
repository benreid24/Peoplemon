//----------Spawning Curvelinear Forest Blocker-------------- 

if (getSaveEntry("professorDead")==1)
	spawnTrainer(Hometown/curveForestBlocker2.tnr,340,219,2);

else
	spawnNPC(Hometown/curveForestBlocker1.npc,340,219,2);

//-----------------Spawning Officer-----------------------------------


if (getSaveEntry("professorDead")==1)
	spawnNPC(Hometown/officer2.npc,344,261,3);

elif (getSaveEntry("Delivered")==1);
	spawnNPC(Hometown/officerHometown.npc,343,281,1);
else
	spawnNPC(Hometown/officer1.npc,336,240,0);

//--------------Spawning Sketchy Character---------------------------

if (getSaveEntry("professorDead")==1)
	spawnNPC(Hometown/sketchyCharacter2.npc,343,261,3);

else
	spawnNPC(Hometown/sketchyCharacter1.npc,338,229,2);

//-----------Spawning Hometown Thugs & Curve Forest thugs-------------------

if (getSaveEntry("professorDead")==1)
	spawnNPC(curveForestThug2.npc,315,193,2);
	spawnTrainer(curveForestThug1.tnr,369,239,2);

elif (getSaveEntry("defeatedAster1")==1) && (getSaveEntry("professorDead")!=0)
	spawnNPC(Hometown/hometownThug1.npc,369,239,0);
	spawnNPC(Hometown/hometownThug2.npc,370,239,0);

//--------------Spawning Aster-------------------------

if (getSaveEntry("defeatedAster1")!=1)
	spawnTrainer(Hometown/aster1.tnr,335,269,3);
