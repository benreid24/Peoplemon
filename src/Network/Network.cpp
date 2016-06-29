#include "Network/Network.hpp"
using namespace sf;
using namespace std;

Network::Network(Mode m)
{
	mode = m;
	eType = None;
	connection.setBlocking(false);
	if (m==Host)
	{
		listener.setBlocking(false);
		Socket::Status s = listener.listen(TcpListener::AnyPort);
		if (s!=Socket::Done)
		{
            state = Error;
            eType = FailedToListen;
		}
		else
			state = Listening;
	}
	else
		state = NotConnected;
}

Network::~Network()
{
	if (state==Connected) //attempt to send formal disconnect signal
	{
		Packet signal;
		signal << Uint16(2);
		connection.send(signal);
		sleep(milliseconds(50)); //try and buy it time
		connection.disconnect();
	}
}

bool Network::connect(IpAddress addr, int port)
{
	Socket::Status s = connection.connect(addr, port, seconds(15));
	if (s!=Socket::Done)
	{
		state = Error;
		eType = FailedToConnect;
	}
    else
		state = Connected;
}
