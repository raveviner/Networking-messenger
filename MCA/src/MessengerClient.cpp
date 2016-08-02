
#include <map>
#include "MessengerClient.h"

using namespace npl;

MessengerClient::MessengerClient() {
	this->clientStatus = DISCONNECTED;
	this->currentRoomName = "";
	this->peersInRoom = new map<string, string>();
	this->peerInSeesion = NULL;
	this->udpPeer = NULL;
	myConnectionPort=0;
	this->udpReader=NULL;
	this->mainServer =NULL;
}
void MessengerClient::run() {

	while (clientStatus != DISCONNECTED) {
		int command;
		string str;
		readFromServer(command, str, mainServer);
		if (command > 0) {
			if (command == SUCCESS_LOGIN) {

				std::istringstream splitter(str);
				string myUser;
				string myIp;
				int myPort;
				splitter >> myUser;
				std::getline(splitter, myIp, ':');
				splitter >> myPort;

				this->clientStatus = LOGGED_IN;
				this->currentUserName = myUser;
				this->myConnectionPort = myPort;
				cout << "Logged in as " << myUser << endl;
			} else if (command == SUCCESS_REGISTER) {

				std::istringstream splitter(str);
				string myUser;
				string myIp;
				int myPort;
				splitter >> myUser;
				std::getline(splitter, myIp, ':');
				splitter >> myPort;

				this->clientStatus = LOGGED_IN;
				this->currentUserName = myUser;
				this->myConnectionPort = myPort;

				cout << "registered & logged as" << myUser << endl;
			} else if (command == LOGIN_REFUSE) {
				cout << "invalid username or password " << endl;
			} else if (command == REGISTER_REFUSE) {
				cout << "username exist - try another" << endl;
			} else if (command == SESSION_ESTABLISHED) {
				//Expecting input in format: <user> <ip>:<port>

				this->clientStatus = IN_SESSION;

				std::istringstream splitter(str);
				string peerUser;
				string peerIpAndPort;
				splitter >> peerUser;
				splitter >> peerIpAndPort;

				cout << "peer is connected" << endl;
				this->peerInSeesion = new pair<string, string>(peerUser,
						peerIpAndPort);

				this->udpPeer = new UDPSocket(this->myConnectionPort);

				//listening in a new thread for incoming udp messages
				this->udpReader = new ClientReader(this->udpPeer);
				this->udpReader->start();

			} else if (command == SUCCESS_ENTER_ROOM) {
				//Expecting input in format: <roomName>

				this->clientStatus = IN_ROOM;
				this->currentRoomName = str;

				this->udpPeer = new UDPSocket(this->myConnectionPort);

				this->udpReader = new ClientReader(this->udpPeer);
				this->udpReader->start();

				cout << "Entered room [" << str << "]" << endl;
			} else if (command == SESSION_REFUSED) {
				//Expecting input in format: <user>
				cout << SESSION_REFUSED_TEXT << "[" << str << "]" << endl;
			} else if (command == CLOSE_SESSION_WITH_PEER) {
				//Expecting input in format: NONE
				cout << CLOSE_SESSION_WITH_PEER_TEXT << "["
						<< this->peerInSeesion->first << "]" << endl;

				delete this->peerInSeesion;
				if (this->udpReader != NULL) {
					this->udpReader->stop();
					//this->udpReader->waitForThread();
				}
				this->udpPeer->close();

				this->clientStatus = LOGGED_IN;
			} else if (command == CLIENT_DISCONNECTED_FROM_ROOM) {
				//Expecting input in format: <user>
				for (map<string, string>::iterator peer = peersInRoom->begin();
						peer != peersInRoom->end(); ++peer) {
					if (peer->first == str) {
						this->peersInRoom->erase(peer->first);
						cout << "Client left the room" << endl;
						break;
					}
				}
			} else if (command == CLIENT_ENTERED_ROOM) {
				//Expecting input in format: <user> <ip>:<port>

				std::istringstream splitter(str);
				string peerUser;
				string peerIpAndPort;
				splitter >> peerUser;
				splitter >> peerIpAndPort;

				this->peersInRoom->insert(make_pair(peerUser, peerIpAndPort));
				cout << "User" << str << " join the room" << endl;
			} else if (command == ADMIN_LEAVE_ROOM) {
				if (this->udpReader != NULL) {
					this->udpReader->stop();
				}
				this->udpPeer->close();
				this->clientStatus = LOGGED_IN;
				cout << "you close the room" << endl;
			} else if (command == CHAT_CLOSED_BY_ADMIN
					or command == DISCONNECT_FROM_ROOM_RESPONSE) {
				this->peersInRoom->clear();

				if (this->udpReader != NULL) {
					this->udpReader->stop();

				}
				this->udpPeer->close();
				this->clientStatus = LOGGED_IN;
				if (command == CHAT_CLOSED_BY_ADMIN)
					cout << "room closed by the administrator" << endl;
				else if (command == DISCONNECT_FROM_ROOM_RESPONSE)
					cout << "you disconnected from the room" << endl;
			} else if (command == LIST_USERS_RESPONSE) {
				cout << "registered User list:\n" << str << "--" << endl;
			} else if (command == LIST_CONNECTED_USERS_RESPONSE) {
				cout << "connected User list:\n" << str << "--" << endl;
			} else if (command == LIST_ROOMS_RESPONSE) {
				cout << "rooms list:\n" << str << "--" << endl;
			} else if (command == LIST_ROOM_USERS_RESPONSE) {
				cout << "room Users list:\n" << str << "--" << endl;
			}
		} else {
			cout << "server disconnected" << endl;
			this->close();
		}

	}
}

void MessengerClient::close() {
	cout << "closing All \n";

	this->currentRoomName = "";
	this->currentUserName = "";
	this->myConnectionPort = 0;
	if (this->peerInSeesion != NULL)
		delete this->peerInSeesion;
	if (this->peersInRoom != NULL)
		this->peersInRoom->clear();

	if (this->udpReader != NULL) {
		this->udpReader->stop();
		delete this->udpReader;
	}
	if (this->udpPeer != NULL) {
		this->udpPeer->close();
	}
	if (this->clientStatus != DISCONNECTED) {
		this->clientStatus = DISCONNECTED;
	}
	if (this->mainServer != NULL) {
		this->mainServer->close();
	}
	cout << "done." << endl;

	::exit(0);
}

void MessengerClient::connect(const string& ip) {
	if (clientStatus == DISCONNECTED) {
		mainServer = new TCPSocket(ip, MSNGR_PORT);
		if (mainServer != NULL) {
			clientStatus = CONNECTED;
			start();
		}
	} else {
		cout << "you are already connected to a server" << endl;
	}
}

void MessengerClient::login(const string& user, const string& password) {
	if (clientStatus == CONNECTED) {
		sendToServer(LOGIN, user + " " + password, mainServer);
	} else {
		cout << "you are not connected to a server" << endl;
	}
}
void MessengerClient::registerUser(const string& user, const string& password) {
	if (clientStatus == CONNECTED) {
		sendToServer(REGISTER, user + " " + password, mainServer);
	} else {
		cout << "you are not connected to a server" << endl;
	}
}

void MessengerClient::listUsers() {
	if (clientStatus == LOGGED_IN || clientStatus == IN_ROOM
			|| clientStatus == IN_SESSION) {
		sendToServer(LIST_USERS, " ", mainServer);
	} else {
		cout << "you are not logged in" << endl;
	}
}
void MessengerClient::listConnectedUsers() {
	if (clientStatus == LOGGED_IN || clientStatus == IN_ROOM
			|| clientStatus == IN_SESSION) {
		sendToServer(LIST_CONNECTED_USERS, " ", mainServer);
	} else {
		cout << "you are not logged in" << endl;
	}
}
void MessengerClient::listRooms() {
	if (clientStatus == LOGGED_IN || clientStatus == IN_ROOM
			|| clientStatus == IN_SESSION) {
		sendToServer(LIST_ROOMS, " ", mainServer);
	} else {
		cout << "you are not logged in" << endl;
	}
}
void MessengerClient::listRoomUsers(const string& roomName) {
	if (clientStatus == LOGGED_IN || clientStatus == IN_ROOM
			|| clientStatus == IN_SESSION) {
		sendToServer(LIST_ROOM_USERS, roomName, mainServer);
	} else {
		cout << "you are not logged in" << endl;
	}
}
void MessengerClient::enterRoom(const string& roomName) {
	if (clientStatus == LOGGED_IN)
		sendToServer(OPEN_OR_CONNECT_TO_ROOM, roomName, mainServer);
	else if (clientStatus == IN_SESSION)
		cout << "you are already in a session" << endl;
	else if (clientStatus == IN_ROOM)
		cout << "you are already in a room" << endl;
	else
		cout << "you are not logged in" << endl;

}
void MessengerClient::printClientStatus() {
	if (clientStatus == IN_ROOM)
		cout << "client in room [" << this->currentRoomName << "]" << endl;
	else if (clientStatus == IN_SESSION)
		cout << "client is in session" << endl;
	else if (clientStatus == LOGGED_IN)
		cout << "client is logged in to server" << endl;
	else if (clientStatus == CONNECTED)
		cout << "client is connected to server" << endl;
	else if (clientStatus == DISCONNECTED)
		cout << "client is not connected to any server" << endl;
}
void MessengerClient::closeCurrentRoom() {
	if (clientStatus == IN_ROOM) {
		sendToServer(CLOSE_ROOM, this->currentRoomName, mainServer);
	} else {
		cout << "You are not in a room" << endl;
	}
}

void MessengerClient::openSession(const string& peerUser) {
	if (clientStatus == LOGGED_IN)
		sendToServer(OPEN_SESSION_WITH_PEER, peerUser, mainServer);
	else if (clientStatus == IN_SESSION)
		cout << "you are already in a session" << endl;
	else if (clientStatus == IN_ROOM)
		cout << "you are already in a room" << endl;
	else
		cout << "you are not logged in" << endl;
}
void MessengerClient::closeSessionOrExitRoom() {
	if (clientStatus == IN_SESSION)
		sendToServer(CLOSE_SESSION_WITH_PEER, " ", mainServer);
	else if (clientStatus == IN_ROOM)
		sendToServer(DISCONNECT_FROM_ROOM, this->currentRoomName, mainServer);
	else
		cout << "not in session or room" << endl;
}
void MessengerClient::disconnect() {
	if (clientStatus != DISCONNECTED) {
		sendToServer(EXIT, " ", mainServer);
		close();
		cout << "you have disconnected from server" << endl;
	} else
		cout << "client is already disconnected" << endl;

}
void MessengerClient::exit() {
	if (clientStatus != DISCONNECTED) {
		sendToServer(EXIT, " ", mainServer);
	}
	close();
	cout << "you have disconnected from server" << endl;
}
void MessengerClient::sendToServer(int command, const string& data,
		TCPSocket* mainServer) {
	TCPMessengerProtocol::sendToServer(command, data, mainServer);
}
void MessengerClient::readFromServer(int & command, string& data,
		TCPSocket* mainServer) {
	TCPMessengerProtocol::readFromServer(command, data, mainServer);
}
void MessengerClient::send(const string & msg) {

	string peerIp;
	int peerPort;

	if (clientStatus == IN_SESSION) {

		istringstream splitter(this->peerInSeesion->second);
		getline(splitter, peerIp, ':');
		splitter >> peerPort;

		//send peer in session the msg
		this->udpPeer->sendTo(">[" + currentUserName + "] " + msg, peerIp,
				peerPort);
	} else if (clientStatus == IN_ROOM) {

		istringstream splitter;

		//iterate over all peers in room and send msg
		for (map<string, string>::iterator peer = peersInRoom->begin();
				peer != peersInRoom->end(); ++peer) {
			splitter.clear();
			splitter.str(peer->second);
			getline(splitter, peerIp, ':');
			splitter >> peerPort;

			this->udpPeer->sendTo(">[" + currentUserName + "] " + msg, peerIp,
					peerPort);
		}
	} else {
		cout << "you are not in a room or a session" << endl;
	}
}


