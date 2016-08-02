/*
 * ClientReader.h
 *
 *  This class gets a UDP socket and opens a new thread that receives messages
 */

#ifndef CLIENTREADER_H_
#define CLIENTREADER_H_

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

class ClientReader: public MThread {
	UDPSocket* udpPeer;
	bool running;

public:

	ClientReader(UDPSocket* udpPeer);


	void run();

	void stop();
};

} /* namespace npl */

#endif /* CLIENTREADER_H_ */
