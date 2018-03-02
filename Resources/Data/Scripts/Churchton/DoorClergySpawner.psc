if ((getDaytime()) > 660)                               //after 11am
	{
	 print("It's After 11 am");
	 print("It's " +getDaytime()/60);
	 spawnNPC("Churchton/DoorClergy.npc",249,301,2,1);
	 spawnNPC("Churchton/DoorClergy2.npc",250,301,2,1);
	 sleep((1920 - getDaytime())*1000);                  //(hour 24 - current time) + 8h sleep
	 removeNPC("DoorClergy");
	 removeNPC("DoorClergy2");
	}
elif ((getDaytime()) < 480) //Before Service			//before 8am
	{
	 print("It's Before 8am");
	 print("It's" +getDaytime()/60);
	 spawnNPC("Churchton/DoorClergy.npc",249,301,2,1);
	 spawnNPC("Churchton/DoorClergy2.npc",250,301,2,1);
	 sleep((480 - getDaytime())*1000);					//(8h - current time) sleep
	 removeNPC("DoorClergy");
	 removeNPC("DoorClergy2");
	 }

print("It's Between 8 and 11");
print("It's" +getDaytime()/60);

while(1)
	{
	 print("While Loop Started Door Npcs");
	 sleep((660-(getDaytime()))*1000);					//(11h- current time) sleep
	 print("It's after 11");
	 print("It's" +getDaytime()/60);
	 if ((playerYWorldPos()/32) == 301 && ((playerXWorldPos()/32) == 249 || (playerXWorldPos()/32) == 250))
		shiftPlayer(0,2);
	 spawnNPC("Churchton/DoorClergy.npc",249,301,2,1);
	 spawnNPC("Churchton/DoorClergy2.npc",250,301,2,1);
	 sleep(1260*1000);
	 removeNPC("DoorClergy");
	 removeNPC("DoorClergy2");
	 print("It's 8am");
	 print("It's" +getDaytime()/60);
	}