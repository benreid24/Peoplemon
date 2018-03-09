if ((getDaytime()) > 660 || (getDaytime()) < 480)                               //after 11am
	{
	 spawnNPC("Churchton/DoorClergy.npc",249,301,2,1);
	 spawnNPC("Churchton/DoorClergy2.npc",250,301,2,1);
	 sleepToTime(8,0);
	}

while(1)
	{
	 removeNPC("DoorClergy");
	 removeNPC("DoorClergy2");
	 sleepToTime(11,0);
	 
	 if (playerXWorldPos()/32 == 301 && (playerYWorldPos()/32 == 249 || playerYWorldPos()/32 == 250 ))
		{
		 shiftPlayer(0,2);
		}
 	 spawnNPC("Churchton/DoorClergy.npc",249,301,2,1);
	 spawnNPC("Churchton/DoorClergy2.npc",250,301,2,1);
	 sleepToTime(8,0); 
	 }