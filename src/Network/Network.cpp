#include "Network/Network.hpp"
using namespace sf;
using namespace std;

HostSettings::HostSettings(sf::IpAddress addr, int p, std::string nm)
{
	ip = addr;
	port = p;
	name = nm;
}

void Network::update()
{
	Uint16 type;
	while (running)
	{
		if (state==Connected)
		{
			connection.setBlocking(false);
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
			if (s==Socket::Done)
			{
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
		}
		else if (state==Listening && mode==Host)
		{
			Packet p;
			p << name;
			p << IpAddress::getLocalAddress().toString();
			p << listener.getLocalPort();
			udp.send(p, IpAddress::Broadcast, 32768);
		}

		sleep(milliseconds(80));
	}
}

Network::Network(Mode m, string nm) : runner(&Network::update, this)
{
	running = true;
	mode = m;
	name = nm;
	eType = None;
	connection.setBlocking(true);
	udp.setBlocking(false);
	if (m==Host)
	{
		udp.bind(Socket::AnyPort);
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
	{
		state = NotConnected;
		udp.bind(32768);
	}

	udpWaiter.add(udp);
	tcpWaiter.add(connection);
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

int Network::getServerPort()
{
	if (mode==Client)
		return 0;
	return listener.getLocalPort();
}

IpAddress Network::getLocalIp()
{
	return IpAddress::getLocalAddress();
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
		DataPacket ret = gamePackets.front();
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

void Network::sendSignal(DataPacket::Type s)
{
	Packet p;
	p << Uint16(s);
	sendPacket(p);
}

void Network::sendSignal(DataPacket::Choice c)
{
	Packet p;
	p << Uint16(4);
	p << Uint8(c);
	sendPacket(p);
}

void Network::sendSignal(DataPacket::Confirmation c)
{
	Packet p;
	p << Uint16(5);
	p << Uint8(c);
	sendPacket(p);
}

vector<HostSettings> Network::pollLocalHosts()
{
	vector<HostSettings> ret;

	if (!udpWaiter.wait(milliseconds(200)))
		return ret;

	Packet p;
	IpAddress remoteAddr;
	unsigned short remotePort;
	string name, ip;
	Uint16 port;

    while (udp.receive(p, remoteAddr, remotePort)==Socket::Done)
	{
		p >> name;
		p >> ip;
		p >> port;
		for (unsigned int i = 0; i<ret.size(); ++i)
		{
			if (ret[i].name==name)
				goto noAdd;
		}
		ret.push_back(HostSettings(ip,port,name));
		noAdd:;
	}

	return ret;
}
