setPlayerLock(1);
npcToLocation("BPTW",playerXWorldPos()/32,(playerYWorldPos()/32)+1);
sleep(5500);
movePlayer(2,0,1);
print("Interact\n");
setPlayerLock(0);
interact();

