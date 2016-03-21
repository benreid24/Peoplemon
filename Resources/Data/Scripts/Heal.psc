if(controlPressed(5))
{
	if (playerDir(0))
	{
	setPlayerLock(1);
	messageBox("I'm gonna heal your peoplemon, no question about it.");
	heal;
	sleep(3000);
	messageBox("There you go. All set.");
	messageBox("Try not to be so abusive to your peoplemon next time");
	messageBox("Have a nice one");	
	setPlayerLock(0);
	}
}