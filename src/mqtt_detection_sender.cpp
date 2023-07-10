/*
 * mqtt_detection_sender.cpp
 *
 *  Created on: Sep 7, 2020
 *      Author: hasan
 */


#include "mqtt_detection_sender.h"
#include <iostream>

using namespace std;

image_proc_utils::MqttDetectionSender::MqttDetectionSender(const std::string & hostname, int port)
	: DetectionSender(hostname, port)
{

	mosqpp::lib_init();		// Mandatory initialization for mosquitto library
	client = make_unique<mosqpp::mosquittopp>(id.c_str());



}

image_proc_utils::MqttDetectionSender::~MqttDetectionSender() {
	mosqpp::lib_cleanup();
}

void image_proc_utils::MqttDetectionSender::SendMessage(const std::vector<DetectionInfo> & detections) {
	int keepalive = 60;
	for (int i=0; i<detections.size(); i++) {
		client->connect(hostname.c_str(), port, keepalive);
		cout << "Detection " << i << ":" << detections[i].ToString() << endl;
		client->publish(NULL, TOPIC.c_str(), detections[i].ToString().length(), detections[i].ToString().c_str());
		//Sleep(10);
		//usleep(10000);

	}

}
