/*
 * mqtt_detection_sender.h
 *
 *  Created on: Sep 7, 2020
 *      Author: hasan
 */

#ifndef SRC_MQTT_DETECTION_SENDER_H_
#define SRC_MQTT_DETECTION_SENDER_H_


#include "detection_sender.h"
#include "mosquittopp.h"

#include <memory>

namespace image_proc_utils {


class MqttDetectionSender : public DetectionSender {


public:
	MqttDetectionSender(const std::string & hostname, int port);
	std::unique_ptr<mosqpp::mosquittopp> client;
	~MqttDetectionSender();

	void SendMessage(const std::vector<DetectionInfo> & detections) override;

private:
	const std::string TOPIC = "FIRE_SPARK_DETECTOR";
	const std::string id = "my_test";

};

}



#endif /* SRC_MQTT_DETECTION_SENDER_H_ */
