if (getSaveEntry("gotStarter")!=1 && playerDir()==3 && controlPressed(5)==1)
{
	setPlayerLock(1);
	messageBox("Nick, the Sassy Peoplemon.");
	string c = choiceBox("Is this the Peoplemon for you?","Take Nick","Keep Looking");
    
	if (c=="Take Nick")
	{
		givePeoplemon("starterNick.ppl");
		addSaveEntry("gotStarter",1);
	}
	setPlayerLock(0);
	sleep(250);
}