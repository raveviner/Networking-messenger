/*
 * ClientReader.cpp
 *
 *  Created on: Jul 30, 2016
 *      Author: user
 */

#include "ClientReader.h"

namespace npl {
ClientReader::ClientReader(UDPSocket* udpPeer) {
	running=true;
	this->udpPeer = udpPeer;

}

void ClientReader::run() {
	char buffer[100];
	bzero(buffer, sizeof(buffer));
	while (running) {
		udpPeer->read(buffer, 100);
		cout << buffer << endl;
		bzero(buffer, sizeof(buffer));
	}
	cout << "Stopped reading from peer" << endl;
}
void ClientReader::stop() {
	running = false;
}

} /* namespace npl */
