

#ifndef CPPPROJECT_COPY_CHATROOM_H
#define CPPPROJECT_COPY_CHATROOM_H

#include <vector>
#include <map>
#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketListener.h"

namespace npl {

class ChatRoom:public MThread {
public:
    class ChatRoomHandler{
    public:
        virtual void onClose(ChatRoom* chatRoom, map<string,TCPSocket*> peersMap)=0;
        virtual void onClientExit(ChatRoom* chatRoom, TCPSocket* peer)=0;
        virtual void onClientDisconnect(ChatRoom* chatRoom, TCPSocket* peer)=0;
    };
private:
    map<string,TCPSocket*> peers;
    TCPSocket * admin;
    ChatRoomHandler* handler;
    string roomName;
    MultipleTCPSocketListener listener;
    bool active;
    void exit(TCPSocket* sock);
    void disconnect(TCPSocket* sock,const string& data);
public:
    ChatRoom(ChatRoomHandler* handler, const string& roomName,const string& peerName,TCPSocket* peerA);
    void run();
    void close();
    void closeByPeer(TCPSocket* peer);
    void disconnectByPeer(TCPSocket* peer);
    void addUser(string userName,TCPSocket* peer);
    void sendByLoop(int command,const string& data,TCPSocket* sender);
    string getRoomName();
    string getUsers();

};
} /* namespace npl */

#endif //CPPPROJECT_COPY_CHATROOM_H
