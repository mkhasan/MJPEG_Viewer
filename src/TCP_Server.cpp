/*
 * TCP_Server.cpp
 *
 *  Created on: Sep 8, 2020
 *      Author: hasan
 */


#include "Config.h"

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>

#define MAX MAX_DETECTION_MSG_SIZE

#define SA struct sockaddr

using namespace std;
using namespace image_proc_utils;
const int PORT = DETECTION_READER_CUSTOM_PORT;
// Function designed for chat between client and server.

void Process(char * buff, int len);
void * func(void *ptr)
{
	int sockfd = *(int *)ptr;

	static char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		int ret = read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents

		//cout << "buff value is: " << buff << endl;
		if (ret) {

			Process(buff, ret);
			//cout << "return value is " << ret << endl;
			//cout << buff << endl;
		}

		else {
			//cout << "return value is " << ret << endl;
			break;
		}

	}

	delete ptr;
	return NULL;
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	while(1) {
		connfd = accept(sockfd, (SA*)&cli, (unsigned int *)&len);
		if (connfd < 0) {
			printf("server acccept failed...\n");
			exit(0);
		}
		else
			printf("server acccept the client...\n");
			int * p = new int;
			*p = connfd;
			connfd = 0;
			pthread_t pth;
			pthread_create(&pth, NULL, func, p);

		}

	// Function for chatting between client and server
	//func(connfd);

	// After chatting close the socket
	close(sockfd);
}

#include "DetectionInfo.h"
#include <vector>

using namespace std;
using namespace image_proc_utils;

void Process(char * buff, int len) {

	vector<DetectionInfo> detections;

	char sep[2];
	sep[0] = '|';
	sep[1] = 0;
	char * token = strtok(buff, sep);

	while(token != NULL) {
		//
		//cout << token << endl;
		detections.push_back(DetectionInfo(token));
		token = strtok(NULL, sep);
	}
	cout << "Size of detections is " << detections.size() << endl;
	for (int i=0; i<detections.size(); i++) {
		cout << "info: " << i << ":" << detections[i].ToString() << endl;
	}
}
