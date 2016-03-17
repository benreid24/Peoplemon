if ("butt"=="penis")
	print("String equality error");
if (getSaveEntry("doorOpenedDemo")=="open")
{
	print(getSaveEntry("doorOpenedDemo")+" is equal to open");
	setCollisions(41,31,1,1,0);
}
elif (playerDir()==0)
{
	if (hasItem(121))
	{
		setPlayerLock(1);
		messageBox("The key opened the door!");
		setCollisions(41,31,1,1,0);
		setPlayerLock(0);
		addSaveEntry("doorOpenedDemo","open");
	}

	else
	{
		setPlayerLock(1);
		messageBox("The door is locked!");
		setPlayerLock(0);
	}
}