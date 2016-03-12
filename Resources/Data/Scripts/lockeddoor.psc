if (getSaveEntry("doorOpenedDemo")=="open")
{
	setCollisions(playerXWorldPos(),playerYWorldPos()-1,1,1,1);
}

elif (playerDir==0)

{
	elif (hasItem(121))
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