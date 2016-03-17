print("Checking door...\n");
if (getSaveEntry("doorOpenedDemo")=="open")
{
	setCollisions(41,31,1,1,0);
	print("Door already open\n");
}
elif (playerDir()==0)
{
	print("Unlocking\n");
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