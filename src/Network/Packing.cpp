#include "Network/Packing.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "Battle/Turn.hpp"
#include "World/People/Player.hpp"
#include <vector>
using namespace sf;
using namespace std;

namespace Packing
{
	template<>
	Packet pack(PeoplemonRef obj)
	{
		Packet ret;
		ret << Uint16(7);
		ret << Uint16(obj.id);
		ret << obj.name;
		ret << Uint16(obj.level);
		ret << Uint16(obj.holdItem);
		for (int i = 0; i<4; ++i)
			ret << obj.moves[i].id;
		obj.evs.pack(ret);
		obj.ivs.pack(ret);
		return ret;
	}

	/**
	 * Helper function for pack specialization for Player
	 *
	 * \param p A reference to the Packet to pack into
	 * \param obj The vector to pack
	 */
	void packVector(Packet& p, vector<int>& obj)
	{
		Uint32 s = obj.size();
		p << s;
		for (unsigned int i = 0; i<obj.size(); ++i)
			p << Int32(obj[i]);
	}

	template<>
	Packet pack(Turn& obj)
	{
		Packet ret;
		ret << Uint16(8);
		ret << Uint8(obj.type);
		ret << Uint16(obj.id);
		return ret;
	}

	template<>
	Packet pack(Player& obj)
	{
		Packet ret;
		ret << Uint16(6);
		ret << obj.getName();
		ret << Uint8((obj.getGender()=="Boy")?(1):(2));
        ret << Uint32(obj.getMoney());
        packVector(ret, *obj.getItems());
        return ret;
	}

	template<>
	bool unpack(DataPacket& dp, PeoplemonRef& obj)
	{
		if (dp.getType()!=DataPacket::Peoplemon)
		{
			cout << "WARNING: Specialization of unpack() for Peoplemon called on DataPacket that doesn't contain a Peoplemon!" << endl;
			return false;
		}
		Uint16 temp;
		Packet& p = dp.getData();

		p >> temp;
		obj.id = temp;
		p >> obj.name;
		p >> temp;
		obj.level = temp;
		p >> temp;
		obj.holdItem = temp;
		for (int i = 0; i<4; ++i)
		{
			p >> temp;
			obj.moves[i].id = temp;
		}
		obj.evs.unpack(p);
		obj.ivs.unpack(p);

		return true;
	}

	/**
	 * Helper function for unpack specialization for PlayerInfo
	 *
	 * \param dp A reference to the DataPacket to load from
	 * \param v A reference to the vector to unpack into
	 */
	void unpackVector(DataPacket& dp, vector<int>& obj)
	{
		Uint32 s;
		Int32 d;
		Packet& p = dp.getData();

		p >> s;
		obj.resize(s);
		for (unsigned int i = 0; i<s; ++i)
		{
			p >> d;
			obj[i] = d;
		}
	}

	template<>
	bool unpack(DataPacket& dp, Turn& obj)
	{
		if (dp.getType()!=DataPacket::Turn)
		{
			cout << "WARNING: Specialization of unpack() for Turn called on DataPacket that doesn't contain a Turn!" << endl;
			return false;
		}
		Packet& p = dp.getData();
		Uint8 t;
		Uint16 id;
		p >> t;
		p >> id;
		obj.type = Turn::Type(t);
		obj.id = id;
		return true;
	}

	template<>
	bool unpack(DataPacket& dp, PlayerInfo& obj)
	{
		if (dp.getType()!=DataPacket::PlayerInfo)
		{
			cout << "WARNING: Specialization of unpack() for PlayerInfo called on DataPacket that doesn't contain PlayerInfo!" << endl;
			return false;
		}
		Packet& p = dp.getData();
		Uint16 t;
		Uint32 m;

		p >> obj.name;
		p >> t;
		obj.gender = t;
		p >> m;
		obj.money = m;
		unpackVector(dp,obj.items);

		return true;
	}
}
