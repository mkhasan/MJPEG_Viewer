/*
 * custom_detection_sender.cpp
 *
 *  Created on: Sep 8, 2020
 *      Author: hasan
 */


#include "custom_detection_sender.h"

#include "Config.h"

#ifndef _WIN32
#include <unistd.h>
#endif

#include <iostream>

using namespace std;

image_proc_utils::CustomDetectionSender::CustomDetectionSender(const string & _hostname, int _port)
	: AbsDetectionSender(_hostname, _port)
{

	CreateConnector(TRY_COUNT, false);
	
}

image_proc_utils::CustomDetectionSender::~CustomDetectionSender() {

}

void image_proc_utils::CustomDetectionSender::CreateConnector(int tryCount, bool reconnect) {
	if (reconnect) {
		connector.release();
	}
	int i=0;
	for (i=0; i<tryCount; ++i) {
		try {
			connector = std::make_unique<Connector>(hostname, port);
		}
		catch (const exception& e) {
#ifdef _WIN32			
			Sleep(1000);
#else
			sleep(1);
#endif
			continue;
		}

		break;
	}

	if (i == tryCount) {
		THROW_EX(RobotException, "Unable to connect");
	}


}

void image_proc_utils::CustomDetectionSender::SendMessage(const vector<DetectionInfo> & detections) {

	//string str = std::format("Hello {}!\n", "world");
	static char buff[MAX_DETECTION_MSG_SIZE];

	buff[0] = 0;
	int len = 0;
	//DetectionInfo temp;

	bool  flag = true;
	try {
		for(int i=0; i<detections.size(); i++) {
			char *p = (char *)&detections[i].Rep();
			memcpy(&buff[0], p, sizeof(DetectionInfo::rep_t));
			/*int ret = snprintf(&buff[len], MAX_DETECTION_MSG_SIZE-len, "|%s", detections[i].ToString().c_str());
			len += ret;
			if (len >= MAX_DETECTION_MSG_SIZE) {
				THROW_EX(RobotException, "too much data to be sent");
			}
			if (len < 0) {
				THROW_EX(RobotException, "Unknown error");
			}
			*/
			connector->Send(buff, sizeof(DetectionInfo::rep_t));
			cout << "robot id: "<< detections[i]->robotNum << " detection class is: " << detections[i]->detectionClass << endl;
		}
	}
	catch(const exception& e) {

		flag = false;
	}
 
	if (!flag) {
		cout << "error in sending ... trying to reconnect" << endl;
		CreateConnector();
	}



}
