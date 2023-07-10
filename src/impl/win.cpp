

#include "ImageProcUtils.h"

#include "../custom_detection_sender.h"

//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;

image_proc_utils::CustomDetectionSender::Connector::Connector(const string& hostname, int port) : pSocket(NULL) {
    WSADATA wsaData;
    static SOCKET SendingSocket;
    pSocket = &SendingSocket;

    // Server/receiver address
    int  RetCode;

    SOCKADDR_IN          ServerAddr, ThisSenderInfo;

    // Server/receiver port to connect to

    unsigned int         Port = 7171;

   
    int iResult;
    const int MSG_LEN = 100;
    char msg[MSG_LEN];

    SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        
        snprintf(msg, MSG_LEN, "WSAStartup failed with error: %d", iResult);
        THROW_EX(RobotException, msg);
    }

    SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (SendingSocket == INVALID_SOCKET)

    {

        snprintf(msg ,MSG_LEN, "Client: socket() failed! Error code: %ld\n", WSAGetLastError());

        // Do the clean up

        WSACleanup();

        // Exit with error
        THROW_EX(RobotException, msg);

    }

    // IPv4

    ServerAddr.sin_family = AF_INET;

    // Port no.

    ServerAddr.sin_port = htons(unsigned int (port));

    // The IP address

    ServerAddr.sin_addr.s_addr = inet_addr(hostname.c_str());



    // Make a connection to the server with socket SendingSocket.

    iResult = connect(SendingSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));

    if (iResult != 0) {
        snprintf(msg, MSG_LEN, "Client: connect() failed! Error code: %ld\n", WSAGetLastError());

        // Close the socket

        closesocket(SendingSocket);

        // Do the clean up

        WSACleanup();

        THROW_EX(RobotException, msg);
    }

}

image_proc_utils::CustomDetectionSender::Connector::~Connector() {

    
    if (pSocket == NULL) {
        THROW_EX(RobotException, "already closed");

    }

    SOCKET& SendingSocket = *((SOCKET*)pSocket);

    const int MSG_LEN = 100;
    char msg[MSG_LEN];

    if (closesocket(SendingSocket) != 0)

        snprintf(msg, MSG_LEN, "Client: Cannot close \"SendingSocket\" socket. Error code: %ld\n", WSAGetLastError());
    THROW_EX(RobotException, msg);


    // When your application is finished handling the connection, call WSACleanup.

    if (WSACleanup() != 0)

        THROW_EX(RobotException, "Client: WSACleanup() failed!...");

}

void image_proc_utils::CustomDetectionSender::Connector::Send(const char* buff, int len) {

    SOCKET& SendingSocket = *((SOCKET*)pSocket);
    int BytesSent = send(SendingSocket, buff, len, 0);

    if (BytesSent != len) {
        const int MSG_LEN = 100;
        char msg[MSG_LEN];
        snprintf(msg, MSG_LEN, "Errro in sending data len=%d, ret=%d", len, BytesSent);
        THROW_EX(RobotException, msg);
    }
    
}