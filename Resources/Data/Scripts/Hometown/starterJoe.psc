//Concurrent
if (getSaveEntry("gotStarter")!=1 && playerDir()==3 && controlPressed(5)==1)
{
	setPlayerLock(1);
	messageBox("Joe, the Orange Haired Peoplemon");
	print("poop");
	string c = choiceBox("Is this the Peoplemon for you?","Take Joe","Keep Looking");
    
	if (c=="Take Joe")
	{
		givePeoplemon("starterJoe.ppl");
		addSaveEntry("gotStarter",1);
		editTile()
	}
	setPlayerLock(0);
	sleep(250);
}