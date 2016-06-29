#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "SFML.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "DataPacket.hpp"
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
	 * An enum for storing the current state of the network
	 */
	enum State
	{
		NotConnected,
		Listening,
		Connected,
		Disconnected,
		Error
	};

	/**
	 * An enum for storing the current error type of the network, if any
	 */
	enum ErrorType
	{
		None,
		FailedToListen,
		FailedToConnect,
		UnexpectedDisconnect
	};

private:
	Mode mode;
	State state;
	ErrorType eType;

    sf::TcpListener listener;
    sf::TcpSocket connection;
    std::stack<sf::Packet> gamePackets;

public:
	/**
     * Initializes the internal state, but does not open any connections. Opens a listener if the specified mode is Host
     */
    Network(Mode m = Host);

    /**
     * Terminates any open connections
     */
    ~Network();

    /**
     * Attempts to connect to the remote peer at the given address
     *
     * \param addr The address of the remote peer
     * \param port The port to connect to
     * \return Whether or not the connection was successful
     */
    bool connect(sf::IpAddress addr, int port);

    /**
     * Checks to see if a client has connected
     *
     * \return True if a client has connected, false otherwise
     */
    bool checkClientConnected();

    /**
     * Returns the current state of the network
     *
     * \return The state of the network
     */
    State getState();

    /**
     * Returns the type of error last encountered
     *
     * \return The last type of error that occurred
     */
	ErrorType getLastError();

    /**
     * Checks to see if an application packet has been received and returns it if there is one in the queue
     *
     * \return The packet that was received. Empty if there was nothing waiting
     */
    DataPacket pollPacket();

    /**
     * Sends the given packet to the remote peer
     *
     * \param p The packet to send
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
};

#endif // NETWORK_HPP
