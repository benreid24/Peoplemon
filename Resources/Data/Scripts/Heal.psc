if(controlPressed(5))
{
	if (playerDir()==0)
	{
		setPlayerLock(1);
		messageBox("I'm gonna heal your peoplemon, no question about it.","TODO: play animation/sound on screen to make this more natural");
		healPeoplemon();
		sleep(1500);
		messageBox("There you go, all set", "Try not to be so abusive to your peoplemon next time","Have a nice day");	
		setPlayerLock(0);
		sleep(250); //this is to prevent the script from restarting too quickly
	}
}