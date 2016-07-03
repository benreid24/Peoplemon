#include "Network/Network.hpp"
using namespace sf;
using namespace std;

void Network::update()
{
	Uint16 type;
	while (running)
	{
		if (state==Connected)
		{
			Packet data;
			Socket::Status s = connection.receive(data);

			if (s==Socket::Error)
			{
				state = Error;
				eType = Other;
				break;
			}
			if (s==Socket::Disconnected)
			{
                state = Error;
                eType = UnexpectedDisconnect;
                break;
			}

			DataPacket dp(data);
			data >> type;

			switch (type)
			{
			case 2: //disconnect
				state = Disconnected;
				connection.disconnect();
				break;

			default: //application data
				lock.lock();
				gamePackets.push(dp);
				lock.unlock();
			}
		}
		lock.lock();
		while (outgoingPackets.size()>0)
		{
			connection.send(outgoingPackets.front());
			outgoingPackets.pop();
		}
		lock.unlock();

		sleep(milliseconds(80));
	}
}

Network::Network(Mode m) : runner(&Network::update, this)
{
	running = true;
	mode = m;
	eType = None;
	connection.setBlocking(true);
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

	runner.launch();
}

Network::~Network()
{
	if (state==Connected) //attempt to send formal disconnect signal
	{
		Packet signal;
		signal << Uint16(2);
		connection.send(signal);
		connection.disconnect();
	}
	lock.unlock(); //to ensure runner isn't stuck
	running = false;
	runner.wait();
}

bool Network::connect(IpAddress addr, int port)
{
	Socket::Status s = connection.connect(addr, port, seconds(15));
	if (s!=Socket::Done)
	{
		state = Error;
		eType = FailedToConnect;
		return false;
	}
    else
	{
		Packet p, r;
		Uint16 t;
		Uint32 c;
		p << Uint16(1) << Uint32(837565);
		connection.send(p);
		connection.receive(r);
        r >> t >> c;
        if (t==1 && c==837565)
			state = Connected;
		else
		{
			state = Error;
			eType = AuthFailure;
			return false;
		}
	}
	return true;
}

bool Network::checkClientConnected()
{
	if (state!=Listening) //what?
		return false;

	Socket::Status s = listener.accept(connection);
	if (s==Socket::Done)
	{
		Packet p, r;
		Uint16 t;
		Uint32 c;
		p << Uint16(1) << Uint32(837565);
		connection.send(p);
		connection.receive(r);
        r >> t >> c;
        if (t==1 && c==837565)
			state = Connected;
		else
		{
			state = Error;
			eType = AuthFailure;
			return false;
		}
		return true;
	}
	else if (s==Socket::Error)
	{
		state = Error;
        eType = FailedToAccept;
	}
	return false;
}

Network::State Network::getState()
{
	return state;
}

Network::ErrorType Network::getLastError()
{
	return eType;
}

DataPacket Network::pollPacket()
{
	if (gamePackets.size()>0)
	{
		lock.lock();
		DataPacket ret = gamePackets.top();
		gamePackets.pop();
		lock.unlock();
		return ret;
	}
	return DataPacket();
}

void Network::sendPacket(Packet p)
{
	lock.lock();
	outgoingPackets.push(p);
	lock.unlock();
}
