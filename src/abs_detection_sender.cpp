/*
 * DetectionSender.cpp
 *
 *  Created on: Sep 7, 2020
 *      Author: hasan
 */

using namespace std;

#include "abs_detection_sender.h"

image_proc_utils::AbsDetectionSender::AbsDetectionSender(const string & hostname, int port)
	: hostname(hostname)
	, port(port)
{

}

image_proc_utils::AbsDetectionSender::~AbsDetectionSender() {

}
