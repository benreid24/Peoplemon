#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "SFML.hpp"
#include "Peoplemon/Peoplemon.hpp"
#include "DataPacket.hpp"
#include "Packing.hpp"
#include <stack>
#include <queue>

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
		AuthFailure,
		FailedToListen,
		FailedToConnect,
		FailedToAccept,
		UnexpectedDisconnect,
		Other
	};

private:
	Mode mode;
	State state;
	ErrorType eType;

    sf::TcpListener listener;
    sf::TcpSocket connection;
    std::stack<DataPacket> gamePackets;
    std::queue<sf::Packet> outgoingPackets;

    sf::Thread runner;
    sf::Mutex lock;
    bool running;

    /**
     * This is the update function that the data parsing thread lives in
     */
	void update();

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
     */
    void sendPacket(sf::Packet p);

    /**
     * Sends a signal of the given type to the remote peer
     *
     * \param s The type of signal to send
     */
	void sendSignal(DataPacket::Type s);

	/**
	 * Overload of sendSignal for sending the ActionChoice
	 *
	 * \param c The ActionChoice to send
	 */
	void sendSignal(DataPacket::Choice c);

	/**
	 * Overload of sendSignal for sending the ActionConfirmation
	 *
     * \param c The ActionConfirmation to send
     */
	void sendSignal(DataPacket::Confirmation c);
};

#endif // NETWORK_HPP
