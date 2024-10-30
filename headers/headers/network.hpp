#pragma once

#include <SDL_net.h>

struct responseData{
    int pID;
    UDPsocket udpSocket;
    TCPsocket tcpSocket;
};


void connectToServer(const char* ip, responseData &r );
void receivePlayerStates(UDPsocket clientSocket);
void testSendUDP( UDPsocket udpSocket);


extern const char* HOST;