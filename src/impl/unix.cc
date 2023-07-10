/*
 * unix.cc
 *
 *  Created on: Sep 8, 2020
 *      Author: hasan
 */


#include "custom_detection_sender.h"

#include "ImageProcUtils.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define SA struct sockaddr

using namespace std;

image_proc_utils::CustomDetectionSender::Connector::Connector(const string & hostname, int port) {

	pSocket = new int;
	int &sockfd = *((int *)pSocket);
	int connfd;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		THROW_EX(RobotException, "Error in connecting to the server");
	}


	bzero(&servaddr, sizeof(servaddr));

	    // assign IP, PORT
	    servaddr.sin_family = AF_INET;
	    servaddr.sin_addr.s_addr = inet_addr(hostname.c_str());
	    servaddr.sin_port = htons(port);

	    // connect the client socket to server socket
	    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
	    	THROW_EX(RobotException, "Error in connecting to the server");
	    }



}

image_proc_utils::CustomDetectionSender::Connector::~Connector() {

	int &sockfd = *((int *)pSocket);

	close(sockfd);

	delete pSocket;
}

void image_proc_utils::CustomDetectionSender::Connector::Send(const char *buff, int len) {

	int &sockfd = *((int *)pSocket);
	int ret = write(sockfd, buff, len);
	if (ret != len) {
		char msg[256];
		sprintf(msg, "Errro in sending data len=%d, ret=%d", len, ret);
		THROW_EX(RobotException, msg);
	}

	cout << "sent " << len <<  " bytes" << endl;
}



