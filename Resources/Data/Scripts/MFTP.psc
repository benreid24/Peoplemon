print("MFTP running\n");
setPlayerLock(1);
npcToLocation("MFTP",46,28);
messageBox(playerName()+" stood in shock", "'How dare he exploit me!' "+playerName()+" thought","You quickly forget your anger as the thought of tacos reenter your mind");
removeNPC("MFTP");
setPlayerLock(0);