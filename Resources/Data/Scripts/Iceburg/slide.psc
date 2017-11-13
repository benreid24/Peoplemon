print("slide2\n");
int dir = playerDir();
int x = playerXWorldPos()/32;
int y = playerYWorldPos()/32;
/*if (spaceFree(x,y,dir,1))
{
	while (movePlayer(dir,0,0)==0)
	{
		sleep(12);
	}
}*/
movePlayer(dir,0,0);
print("moved\n");