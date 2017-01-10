//----------------------------------------Professor talks
	 sleep(1200);
   	 moveNPC("professor3",2);
	 sleep(200);
	 messageBox("You're back already?");
	 sleep(600);
	 moveNPC("professor3",1);
	 sleep(800);
	 moveNPC("professor3",3);
	 sleep(800);
	 moveNPC("professor3",2);
	 sleep(600);
	 messageBox("I don't see Aster.");
	 sleep(150);
	 choiceBox("Where is he?","Coming now","No idea","about 3 o'clock");
	 sleep(150);
	 messageBox("Oh, well that's good I guess.");
	 sleep(150);
	 messageBox("I actually have another favor to ask of you.");
	 sleep(200);
	 npcToLocation("professor3",3,4,1);
	 sleep(1500);
	 moveNPC("professor3",1);
	 messageBox("Well, come on.");
	 moveNPC("professor3",3);
	 setPlayerLock(0);
	 playerToLocation(3,5,1);
	 setPlayerLock(1);
	 sleep(200);
	 moveNPC("professor3",2);
	 messageBox("I want you to take the other two peoplemon from the counter over here and take care of them for me.", "They deserve to be let out every once in a while and I never seem to have the time.","Just don't forget to feed them!","Speaking of which, I'm going to make myself a sandwich.");
	 sleep(200);
	 npcToLocation("professor3",17,4,1);
	 moveNPC("professor3",0);
	 sleep(800);

//--------------------------------------Explosion!
	 drawAnim("explosion.anim",480,64,0);
	 removeNPC("professor3");
	 editTile(16,1,2,0);
	
//------------------------------Thug Appearance!!
	 sleep(1500);
	 spawnNPC("Hometown/profLabThug1.npc",10,13,1);
	 npcToLocation("profLabThug1",11,13,1);
	 sleep(500);
	 spawnNPC("Hometown/profLabThug2.npc",10,13,1);
	 sleep(1000);
	 moveNPC("profLabThug1",3);
	 messageBox("I guess it worked.");
	 sleep(1000);
	 moveNPC("profLabThug2",1);
	 messageBox("It worked?!","The stars are in perfect alignment for the Local Thugs!");
	 sleep(600);
	 messageBox("The stars? I thought it was the fridge bomb we placed that killed him.");
	 sleep(600);
	 messageBox("Of course it was you imbecile.","Just watch the door. I'm going to get these rare peoplemon and we can high-tail it out of here.");
	 npcToLocation("profLabThug1",10,13,1);
	 moveNPC("profLabThug1",2);
	 
//---------------------------Thugs Take Peopleballs
	if (getSaveEntry("Vince")!=1
	{
	 npcToLocation("profLabThug2",2,6,1);
	 moveNPC("profLabThug2",3);
	 editTile(1,6,3,0);
	 sleep(100);
	}
	
	if (getSaveEntry("Nick")!=1
	{
	 npcToLocation("profLabThug2",2,5,1);
	 moveNPC("profLabThug2",3);
	 sleep(100);
	 editTile(1,5,3,0);
	} 
	
	if (getSaveEntry("Joe"))!=1
	{
	 npcToLocation("profLabThug2",2,4,1);
	 moveNPC("profLabThug2",3);
	 sleep(100);
	 editTile(1,4,3,0);
	}

//-----------------------------------------Thugs Bounce!
	npcToLocation("profLabThug2",10,12,1);
	moveNPC("profLabThug2",2);
	sleep(200);
	messageBox("Hey");
	sleep(500);
	moveNPC("profLabThug1");
	messageBox("I got them. Let's go meet up with the others.");
	sleep(500);
	removeNPC("profLabThug1");
	npcToLocation("profLabThug2",10,13,1);
	removeNPC("profLabThug2");
	setPlayerLock(0);