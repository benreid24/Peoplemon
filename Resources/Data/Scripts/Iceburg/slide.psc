int slide(int dir) {
	setPlayerLock(1);
	int px = playerXWorldPos()/32;
	int py = playerYWorldPos()/32;
	int dist = 2;
	int traveled = 0;
	
	while (spaceFree(px, py, dir, dist)) {
		dist = dist + 1;
	}
	
	while (traveled < dist -1) {
		movePlayerBlocking(dir,0,0);
		traveled = traveled + 1;
	}
	setPlayerLock(0);
}


int lastX = playerXWorldPos();
int lastY = playerYWorldPos();

while (1) {
	int x = playerXWorldPos();
	int y = playerYWorldPos();
	
	if (lastX != x || lastY != y) {
		slide(playerDir());
	}
	
	lastX = playerXWorldPos();
	lastY = playerYWorldPos();
	
	sleep(10);
}