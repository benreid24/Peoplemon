if(controlPressed(5))
{
	if (playerDir()==0)
	{
		setPlayerLock(1);
		messageBox("I'm gonna heal your peoplemon, no question about it.");
		healPeoplemon();
		messageBox("There you go, all set");
		messageBox("Try not to be so abusive to your peoplemon next time");
		messageBox("Have a nice day");	
		setPlayerLock(0);
		sleep(250); //this is to prevent the script from restarting too quickly
	}
}