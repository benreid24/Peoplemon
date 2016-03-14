print("Checking door...\n");
if (getSaveEntry("doorOpenedDemo")=="open")
{
	setCollisions(playerXWorldPos(),playerYWorldPos()-1,1,1,1);
	print("Door already open\n");
}
elif (playerDir()==0)
{
	print("Unlocking\n");
	if (hasItem(121))
	{
		messageBox("The key opened the door!");
		setCollisions(playerXWorldPos(),playerYWorldPos()-1,1,1,1);
		addSaveEntry("doorOpenedDemo","open");
	}

	else
	{
		messageBox("The door is locked!");
	}
}