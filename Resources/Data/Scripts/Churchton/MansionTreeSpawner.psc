if (getDaytime() < 1380 && getDaytime() > 240 ) //not Between the hours of 23 (11pm) and 4
	{
	 editTile(252,329,5,1);
	 editTile(252,331,5,1);
	 editTile(255,329,5,1);
	 editTile(255,331,5,1);
	 editTile(259,330,5,33);
	 editTile(261,330,5,33);
	 editTile(261,328,5,33);
	 editTile(263,328,5,33);
	 editTile(261,332,5,33);
	 editTile(263,330,5,33);
	 sleep((1380 - getDaytime())*1000); 			//Wait time until no trees (=hour23-current time)
	}
while(1)
	{
	 print("While Loop Started Trees");
	 editTile(252,329,5,0);
	 editTile(252,331,5,0);
	 editTile(255,329,5,0);
	 editTile(255,331,5,0);
	 editTile(259,330,5,0);
	 editTile(261,330,5,0);
	 editTile(261,328,5,0);
	 editTile(263,328,5,0);
	 editTile(261,332,5,0);
	 editTile(263,330,5,0);
	 if (getDaytime() <= 1440)
		sleep((1680-getDaytime())*1000);
	 else
		sleep((240-getDaytime())*1000);
	 if ((playerYWorldPos()/32 <= 333 && playerYWorldPos()/32 >= 330) && (playerXWorldPos()/32 >= 252 && playerXWorldPos()/32 <= 265))
		playerToLoacation(249,331,0);
	 editTile(252,329,5,1);
	 editTile(252,331,5,1);
	 editTile(255,329,5,1);
	 editTile(255,331,5,1);
	 editTile(259,330,5,33);
	 editTile(261,330,5,33);
	 editTile(261,328,5,33);
	 editTile(263,328,5,33);
	 editTile(261,332,5,33);
	 editTile(263,330,5,33);
	 sleep(1140*1000);
	}