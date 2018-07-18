//-------Spawn Auspicious Boat----------
if(getSaveEntry("AuspiciousDelivery")==1 && getSaveEntry("AuspiciousBoatComplete")!=1) {
 editTile(296,18,5,106);
 runScript("AuspiciousIsland/BoatToAuspiciousIsland.psc",0);
}

//--------Spawn Boat Guy---------------
if (getSaveEntry("ThugSuit")!=1)
 spawnNPC("CurveForest/boatGuy1.npc",346,174,3,0);

if (getSaveEntry("ThugSuit")==1 && getSaveEntry("AuspiciousDelivery")!=1) {
 spawnNPC("CurveForest/boatGuyT.npc",347,174,2,0);
 spawnNPC("Hometown/Exchamp9.npc",346,174,2,0);
}

//-----------Jake Blocker Anim---------------------
if (getSaveEntry("JakeUnblocked")!=1)
	{
	 setCollisions(315,275,1,1,0);
	}

spawnNPC("JakeBlocker.npc",316,275,0,0);

if (getSaveEntry("JakeUnblocked")==1)
	 editTile(316,275,5,0);
//----------Spawning Curvelinear Forest Blocker-------------- 

if (getSaveEntry("professorDead")!=1)
	spawnNPC("CurveForest/curveForestBlocker1.npc",339,218,2);

if (getSaveEntry("thugsRunAway")==1 && getSaveEntry("boatGuy1")!=1)
	spawnNPC("CurveForest/curveForestBlocker2.npc",347,193,1,0);

if (getSaveEntry("boatGuy1")==1 && getSaveEntry("Blocker4Talked")!=1)
	spawnNPC("CurveForest/curveForestBlocker3.npc",337,219,2,0);
	
if (getSaveEntry("Blocker4Talked")==1)
	spawnNPC("Hometown/Blocker4.npc",346,256,2,0);
	
//-----------------Spawning Officer (Hometown) -----------------------------------

if (getSaveEntry("boatGuy1")==1 && getSaveEntry("MarsalatoTown")!=1 && getSaveEntry("TalkedBoatGuy")!=1) //Officer next to Thug
	{
	 spawnNPC("CurveForest/Officer3.npc",317,195,3,0);
	}

	elif (getSaveEntry("professorDead") == 1 && getSaveEntry("boatGuy1") != 1)
		{
		 spawnNPC("Hometown/officer2.npc",338,234,0);
		}
	
	elif (getSaveEntry("ReturntoProf")==1 && getSaveEntry("professorDead")!=1 )
		{
		 spawnNPC("Hometown/officerHometown.npc",343,281,1);
		 if (getSaveEntry("Delivered")==1)
		}

	elif (getSaveEntry("boatGuy1")!=1 )
		{
		 spawnNPC("Hometown/officer1.npc",344,261,0);
		 setCollisions(343,261,3,1,1);
		}
	
//--------------Spawning Sketchy Character (Hometown) ---------------------------

if (getSaveEntry("professorDead")==1 && getSaveEntry("boatGuy1") != 1)
	{
	 spawnNPC("Hometown/sketchyCharacter2.npc",338,233,0);
	}
	
	elif (getSaveEntry("boatGuy1") != 1)
		{
		 spawnNPC("Hometown/sketchyCharacter1.npc",338,229,2);
		}
	
//-----------Spawning Hometown Thugs & Curve Forest thugs-------------------
if (getSaveEntry("professorDead")==1 && getSaveEntry("thugsRunAway")!=1) 
	 spawnNPC("Hometown/hometownThug1.npc",369,246,2,0);

if (getSaveEntry("thugsRunAway")==1 && getSaveEntry("MarsalatoTown")!=1 && getSaveEntry("TalkedBoatGuy")!=1) //Trainer Thug in Forest
	{
	 spawnTrainer("CurveForest/Thug1.tnr",316,195,2,0);
	}

if (getSaveEntry("professorDead")!=1 && getSaveEntry("aster1Done")==1 && getSaveEntry("boatGuy1")!=1) //Thugs outside of Lab
	{
	 spawnNPC("Hometown/hometownThug1.npc",370,239,0);
	 spawnNPC("Hometown/hometownThug2.npc",369,239,0);
	}

//--------------Spawning Aster-------------------------
if (getSaveEntry("aster1Done")!=1)
	{
	 spawnTrainer("Hometown/aster1.tnr",335,269,3);
	}
	
if (getSaveEntry("BeatAster3")!=1)
	{
	 spawnTrainer("CurveForest/aster3.tnr",319,170,1,0);
	}


//--------------Lab Door & Town Blocking----------------------
if (getSaveEntry("professorDead")==1 && getSaveEntry("boatGuy1")!=1)
	{
	 setCollisions(368,238,1,1,0);
	 setCollisions(334,238,5,1,0);
	 print("Collisions Set");
	}

//---------------------Spawning Exchamp---------------
if (getSaveEntry("thugsRunAway")==1 && getSaveEntry("Exchamp1")!=1)
	spawnNPC("Hometown/Exchamp1.npc",345,225,1,0);

if (getSaveEntry("DadBeat")==1 && getSaveEntry("Exchamp6Talked")!=1)
	spawnNPC("Hometown/Exchamp6.npc",361,269,2,0);
	
if (getSaveEntry("BeatExChamp7")!=1 && getSaveEntry("Crap")==1)
	spawnTrainer("Hometown/Exchamp7.tnr",361,269,2,0);

if (getSaveEntry("JustBeatM")==1 && getSaveEntry("ExchampReveal")!=1)
 spawnNPC("Hometown/exChamp5.npc",344,270,0,0);
 
//---------------------Spawning Aster return battle 1-----
if (getSaveEntry("thugsRunAway")==1 && getSaveEntry("AsterBeat2")!=1)
	spawnTrainer("Hometown/Aster2.tnr",331,274,2,0);

//----------------------Spawning Dad----------------------
if (getSaveEntry("AridityLeaderBeaten")!=1)
	spawnNPC("Hometown/Dad.npc",350,282,2,0);

if (getSaveEntry("DadEnters")!=1 && getSaveEntry("AridityLeaderBeaten")==1)
	spawnNPC("Hometown/Dad2.npc",350,282,2,0);

//----------------------Spawning JakeGuy.npc--------------------
	spawnNPC("Hometown/JakeGuy.npc",318,279,0,0);
	
	
//-----------------------Thugs Parent Nap----------------------------

if(getSaveEntry("AllChampsBeaten")==1) {
  spawnNPC("Hometown/Mailbox.npc",334,256,2,0);
  spawnNPC("Hometown/MailboxGuy.npc",333,256,1,0);
 }