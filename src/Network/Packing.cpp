#include "Network/Packing.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Battle/Turn.hpp"
#include <vector>
using namespace sf;
using namespace std;

namespace Packing
{
	template<>
	Packet pack(PeoplemonRef obj)
	{
		Packet ret;
		//pack
		return ret;
	}

	template<>
	Packet pack(vector<int> obj)
	{
		Packet ret;
		Uint32 s = obj.size();
		ret << s;
		for (unsigned int i = 0; i<obj.size(); ++i)
			ret << Int32(obj[i]);
		return ret;
	}

	template<>
	Packet pack(Turn& obj)
	{
		//
	}

	template<>
	bool unpack(DataPacket& dp, PeoplemonRef& obj)
	{
		//
	}

	template<>
	bool unpack(DataPacket& dp, vector<int>& obj)
	{
		//
	}

	template<>
	bool unpack(DataPacket& dp, Turn& obj)
	{
		//
	}
}
