
#ifndef SRC_MESSENGERCLIENT_H_
#define SRC_MESSENGERCLIENT_H_

#define DISCONNECTED  	0
#define CONNECTED    	1
#define LOGGED_IN    	2
#define IN_SESSION   	3
#define IN_ROOM      	4

#include <iostream>
#include <strings.h>
#include "TCPMessengerProtocol.h"
#include "MThread.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include <sstream>
#include <algorithm>
#include <map>
#include "ClientReader.h"


using namespace std;
namespace npl {

class MessengerClient: public MThread {

private:
	//used in opening a new thread for reading
	ClientReader* udpReader;

	int clientStatus;

	string currentUserName;
	string currentRoomName;
	int myConnectionPort;

	TCPSocket* mainServer;

	UDPSocket* udpPeer;


	//maps user,IP:PORT
	map<string, string>* peersInRoom;
	//pair of user,IP:PORT
	pair<string, string>* peerInSeesion;

public:
	MessengerClient();
	void connect(const string& ip);
	void login(const string& user, const string& password);
	void registerUser(const string& user, const string& password);
	void listUsers();
	void listConnectedUsers();
	void listRooms();
	void listRoomUsers(const string& roomName);
	void enterRoom(const string& roomName);
	void printClientStatus();
	void closeCurrentRoom();
	void openSession(const string& ipAndPort);
	void sendToServer(int command, const string& data, TCPSocket* sock);
	void readFromServer(int & command, string& data, TCPSocket* sock);
	void send(const string & msg);
	void closeSessionOrExitRoom();
	void disconnect();
	void run();
	void exit();
private:
	void close();
};

} /* namespace npl */

#endif /* SRC_MESSENGERCLIENT_H_ */
