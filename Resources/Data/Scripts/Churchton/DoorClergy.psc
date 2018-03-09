string padZeros(int time) {
	string padded = intToString(time);
	if (time<10)
		padded = "0" + padded;
	return padded;
}

string minutes = padZeros(getTimeMinutes());
string hours = padZeros(getTimeHours());
setPlayerLock(1);
messageBox("The church is closed now. You can't go in until our service at 8:00.", " It's " + hours + ":" + minutes);
setPlayerLock(0);