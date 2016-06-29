#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "SFML.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include <stack>

/**
 * \defgroup Network
 * \brief All classes related to networking are in this module
 */

/**
 * Defines an interface for model classes to easily interact with the network
 *
 * \ingroup Network
 */
class Network
{
    sf::TcpListener listener;
    sf::TcpSocket connection;
    std::stack<sf::Packet> gamePackets;

public:
	/**
	 * An enum for storing the current connection mode
	 */
	enum Mode
	{
		Host,
		Client
	};

    /**
     * Initializes the internal state, but does not open any connections
     */
    Network(Mode m = Host);

    /**
     * Terminates any open connections
     */
    ~Network();

    /**
     * Attempts to connect to the remote node at the given address
     *
     * \param addr The address of the remote node
     * \return Whether or not the connection was successful
     */
    bool connect(sf::IpAddress addr);

    /**
     * Waits for an incoming connection. Times out after 30 seconds
     *
     * \return Whether or not the connection was successful
     */
    bool waitConnection();

    /**
     * Tells whether or not the connection is still good
     *
     * \return Whether or not the client is still connected
     */
    bool connectionGood();

    /**
     * Checks to see if an application packet has been receieved and returns it if there is one in the queue
     *
     * \return The packet that was received. Empty if there was nothing waiting
     */
    sf::Packet pollPacket(); //TODO - received packets are first interpreted by the Network then put in the queue if non network related

    /**
     * Waits for a packet to come in. Only returns early if the connection fails
     *
     * \return The packet that was received. Empty if the connection failed
     */
    sf::Packet waitPacket();

    /**
     * Sends the given packet to the remote node
     *
     * \param p The packket to send
     * \return Whether or not the packet was sent
     */
    bool sendPacket(sf::Packet p);

    /**
     * Use this function to pack a peoplemon into a packet
     *
     * \param pk The packet to put data into
     * \param p The peoplemon whose data should be added
     */
    static void packPeoplemon(sf::Packet& pk, PeoplemonRef p);

    /**
     * Extracts peoplemon data from the given packet
     *
     * \param pk The packet to extract from
     * \param p The peoplemon to put the data into
     */
    static void unpackPeoplemon(sf::Packet& pk, PeoplemonRef& p);
    //TODO - more helper functions
};

#endif // NETWORK_HPP
