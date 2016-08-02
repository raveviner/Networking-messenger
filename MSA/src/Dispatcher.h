
#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

#include <algorithm>
#include <vector>
#include <map>
#include "Brocker.h"
#include "MultipleTCPSocketListener.h"
#include "TCPMessengerProtocol.h"
#include "ChatRoom.h"
#include "Users.h"

namespace npl {

class Dispatcher: public MThread,
		ChatRoom::ChatRoomHandler,
		Brocker::BrockerHandler {

	vector<TCPSocket*> peers;
	vector<Brocker*> brockers;
	vector<ChatRoom*> chatRooms;
	map<string, string> registeredUsers; //users who are registered
	map<string, TCPSocket*> loggedInUsers; //logged in and connected
	MultipleTCPSocketListener* listener;
	Users* users;
	bool running;
	void login(TCPSocket* peer, const string& data);
	void registerUser(TCPSocket* peer, const string& data);
	void enterRoom(TCPSocket* peer, const string& data);
	void openSession(TCPSocket* peer, const string& data);
	void listUsers(TCPSocket* peer, const string& data);
	void listConnectedUsers(TCPSocket* peer, const string& data);
	void listRooms(TCPSocket* peer, const string& data);
	void listRoomUsers(TCPSocket* peer, const string& data);
	void exit(TCPSocket* peer, const string& data);
public:
	Dispatcher();

	void listUsers();
	void listConnectedUsers();
	void listSessions();
	void listRooms();
	void listRoomUsers(const string& roomName);

	void add(TCPSocket* peer);
	void removePeer(TCPSocket* peer);
	void run();
	void close();

	//client make an exit from brocker or chatroom
	virtual void onClientExit(Brocker * brocker, TCPSocket* peerA,TCPSocket* peerB);
	virtual void onClientExit(ChatRoom* chatRoom, TCPSocket* peer);

	//client make disconnect from a chatroom
	virtual void onClientDisconnect(ChatRoom* chatRoom, TCPSocket* peer);

	//closing chatroom or brocker
	virtual void onClose(ChatRoom* chatRoom, map<string, TCPSocket*> peersMap);
	virtual void onClose(Brocker * brocker, TCPSocket* peerA, TCPSocket* peerB);


	bool isLoggedIn(TCPSocket *sock);

};

} /* namespace npl */

#endif /* SRC_DISPATCHER_H_ */
