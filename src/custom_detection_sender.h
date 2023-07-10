/*
 * custom_detection_sender.h
 *
 *  Created on: Sep 8, 2020
 *      Author: hasan
 */

#ifndef SRC_CUSTOM_DETECTION_SENDER_H_
#define SRC_CUSTOM_DETECTION_SENDER_H_

#include "abs_detection_sender.h"
#include "Config.h"

#include <string>
#include <vector>

namespace image_proc_utils {


class CustomDetectionSender : public AbsDetectionSender {

	const char SEPARATOR = '|';

public:
	CustomDetectionSender(const std::string & hostname, int port= DETECTION_READER_CUSTOM_PORT);
	~CustomDetectionSender();
	void SendMessage(const std::vector<DetectionInfo> & detections) override;

private:

	const int TRY_COUNT = 5;
	void CreateConnector(int tryCount=1, bool reconnect=true);
	class Connector {
	public:
		Connector(const std::string & hostname, int port);
		~Connector();
		void Send(const std::string & buff) {
			Send(buff.c_str(), buff.length());
		}

		void Send(const char * buf, int len);

		void *pSocket;


	};

	std::unique_ptr<Connector> connector;


};

}


#endif /* SRC_CUSTOM_DETECTION_SENDER_H_ */
