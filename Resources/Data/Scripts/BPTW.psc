sleep(1000);
setPlayerLock(1);
npcToLocation("BPTW",(playerXWorldPos()/32)+1,(playerYWorldPos()/32)+2);
setNPCLock("BPTW", 1);
movePlayer(2,0,1);
sleep(6000);
print("Interact\n");
setPlayerLock(0);
interact();

