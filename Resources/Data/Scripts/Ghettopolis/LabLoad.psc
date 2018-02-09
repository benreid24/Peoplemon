 print("NotLine1");
if (getSaveEntry("ReadyForBoss1")==1 && getSaveEntry("BeatBoss1")!=1)
	{
	 print("NotLine2");
	 spawnTrainer("Ghettopolis/ThugBossJordan.tnr",10,7,1,0);
	 spawnNPC("Ghettopolis/GProf.npc",11,7,3,0);
	 setTrainerBeaten("ThugJerry2",0);
	 setPlayerLock(1);
	 print("NotLine3");
	 runScript("Ghettopolis/LabLoad2.psc",1);
	}