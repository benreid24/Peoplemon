print("slide2\n");
int dir = playerDir();
int x = playerXWorldPos()/32;
int y = playerYWorldPos()/32;
if (spaceFree(x,y,dir,2))
{
	while (movePlayer(dir,0,0)==0)
	{
		sleep(12);
	}
}
print("moved\n");