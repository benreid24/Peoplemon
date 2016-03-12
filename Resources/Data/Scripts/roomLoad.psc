print("Room load script running");
if (getSaveEntry("momTalked")!=1)
{
	setPlayerLock(1);
	messageBox("Mom: Wake up "+playerName()+"! We still need to finish packing for our move!");
	addSaveEntry("momTalked",1);
	sleep(5000);
	setPlayerLock(0);
}
