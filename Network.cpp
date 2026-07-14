#include"Network.h"
#include<DxLib.h>

static IPDATA connectIP;

bool Network::Host(int port) {
	socketHandle = MakeUDPSocket(port);
	return socketHandle != -1;
}

bool Network::Connect(const char* ip, int port) {

	

	socketHandle = MakeUDPSocket(-1);

	if (socketHandle == -1) {
		return false;
	}

	GetHostIPbyName(ip, &connectIP);

	connectPort = port;

	return true;

}

void Network::Send(void* data, int size) {
	
	NetWorkSendUDP(socketHandle, connectIP, connectPort, data, size);

}

int Network::Receive(void* data, int size) {

	IPDATA ip;
	int port;

	return NetWorkRecvUDP(socketHandle, &connectIP, &connectPort, data, size, FALSE);

}

void Network::Close() {

	if (socketHandle != -1) {

		DeleteUDPSocket(socketHandle);
		socketHandle = -1;
	}

}