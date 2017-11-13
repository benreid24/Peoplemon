if (getSaveEntry("MarsalatoTown")!=1)
	{
	 spawnNPC("Ghettopolis/Marsala.npc",391,111,0,0);
	 spawnNPC("Ghettopolis/FoodLady.npc",414,164,3,0);
	}

if (getSaveEntry("MarsalatoTown")==1)//Broken Somewhere here...
	{
	 spawnNPC("Ghettopolis/Marsala2.npc",420,190,0,0);
	 if (getSaveEntry("MarsalaFend")!=1)
		{
		 spawnNPC("Ghettopolis/ThugJimbo.npc",416,181,2,0);
		 spawnNPC("Ghettopolis/ThugJay.npc",417,181,2,0);
		 spawnNPC("Ghettopolis/ThugJacob2.npc",418,181,2,0);
		}
		
	 if (getSaveEntry("MarsalaFend")==1)
		{
		 spawnNPC("Ghettopolis/ThugJimbo.npc",421,189,2,0);
		 spawnNPC("Ghettopolis/ThugJay.npc",419,189,2,0);
		 spawnNPC("Ghettopolis/ThugJacob2.npc",421,189,2,0);
		}
		
	
	}
//-----------------Spawning Boat Guys-------------------
spawnNPC("Ghettopolis/boatGuy3.npc",413,133,3,0);
if (getSaveEntry("boatGuy1") == 1)
	{
	 spawnNPC("Ghettopolis/boatGuy2.npc",445,184,3,0);
	}


//--------------------------Spawning Thugs---------------------	
	 spawnNPC("Ghettopolis/ThugJimmy.npc",435,180,2,0);//Boathouse Door

if (getSaveEntry("DockTrigger")!=1)
	{
	 spawnNPC("Ghettopolis/ThugJacky.npc",435,181,0,0);//Near Boat Guy Island 1 (Leader)
	 spawnNPC("Ghettopolis/ThugJacob.npc",434,180,1,0);//Near Boat Guy Island 1
	 spawnNPC("Ghettopolis/ThugJustin.npc",436,181,3,0);//Near Boat Guy Island 1
	}

if (getSaveEntry("DockTrigger")==1 && getSaveEntry("MandP")!=1)
	{
	 spawnNPC("Ghettopolis/ThugJacob.npc",444,185,3,0);//Near Boat Guy Island 1
	 spawnNPC("Ghettopolis/ThugJustin.npc",444,186,0,0);//Near Boat Guy Island 1 
	 spawnNPC("Ghettopolis/ThugJacky.npc",443,184,1,0);//Near Boat Guy Island 1 (Leader)
	}
	
if (getSaveEntry("DockThugsLeave")!=1 && getSaveEntry("MandP")==1)
	{
	 spawnTrainer("Ghettopolis/ThugJacky.tnr",443,184,3,0);//Near Boat Guy Island 1 (Leader)
	 spawnNPC("Ghettopolis/ThugJacob.npc",444,185,2,0);//Near Boat Guy Island 1
	 spawnNPC("Ghettopolis/ThugJustin.npc",444,186,0,0);//Near Boat Guy Island 1 
	}
if (getSaveEntry("MarsalatoTown")!=1)	
	{
	 spawnNPC("Ghettopolis/ThugJerry.npc",401,172,2,0);//Lab Door
	 spawnNPC("Ghettopolis/ThugJin.npc",407,175,0);//Sandcastle thug chasing kid
	 spawnNPC("Ghettopolis/RowdyKid.npc",408,175,0);//Running away from Jin
	 spawnNPC("Ghettopolis/ThugJoe.npc",418,182,0);//Looking for trash guy
	 spawnNPC("Ghettopolis/ThugJoJo.npc",416,169,1,0);//JuiceConvo
	 spawnNPC("Ghettopolis/ThugJil.npc",417,169,3,0);//JuiceConvo
	 spawnNPC("Ghettopolis/ThugJaq.npc",421,173,0,0);//JakeCommentary
	 if(getSaveEntry("DockThugsLeave")==1)
		{
		 spawnNPC("Ghettopolis/ThugJacky3.npc",397,185,2,0);
		 spawnNPC("Ghettopolis/ThugJustin2.npc",398,185,3,0);
		 spawnNPC("Ghettopolis/ThugJoe2.npc",396,185,1,0);
		}
	}
	
if (getSaveEntry("MarsalatoTown")==1)	
	spawnTrainer("Ghettopolis/ThugJerry2.tnr",401,172,2,0);//Lab Door
	
//
	