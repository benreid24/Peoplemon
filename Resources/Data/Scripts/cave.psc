setLightingOverride(254);
addLight(playerXWorldPos()+16,playerYWorldPos()+22,250);

while (1)
{
	addLight(playerXWorldPos()+16,playerYWorldPos()+22,250);
	removeLight(playerXWorldPos()+16,playerYWorldPos()+22);
	sleep(9);
}