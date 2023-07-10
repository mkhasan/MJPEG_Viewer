/*
 * analyzer.cpp
 *
 *  Created on: Sep 11, 2020
 *      Author: hasan
 */

#include "analyzer.h"


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>
#include <string.h>

using namespace std;
using namespace boost;


Analyzer::Analyzer(const string& filename, int listSize) {

	string line;
	ifstream myfile(filename);

	float startTime = 0;
	if (myfile.is_open()) {
		int count=0;
		while(getline(myfile,line) && count < listSize) {
			rep_t record;
			vector<string> result;
			boost::split(result, line, boost::is_any_of("\t "), boost::token_compress_on);
			if (result.size() < 7)
				continue;

			//cout << result[1] << endl;
			//cout << line << endl;




			/*
			if (count == 0) {
				assert(result[2] == "send" && result[3] == "time:");
				cout << result[4] << endl;
				//cout << "value is " << get_time(result[4]) << endl;
				//printf("value is %f\n", get_time(result[4]));
				startTime = get_time(result[4]);
			}
			*/

			//printf("before %f\n", get_time(result[4]));
			//printf("before %f\n", get_time(result[4]));

			auto get_seq = [](const std::string& str) {

					auto len = str.length();
					auto temp = str.substr(0, len-1);
					return stoi(temp);

				};
			record.seq = get_seq(result[1]);
			record.sendTime = get_time(result[4]);
			record.receiveTime = -1;
			if (result.size() >= 8 && result[7].length() > 1)
				record.receiveTime = get_time(result[7]);

			if (count == 0 || 1) {
				//printf("strt time: %f \n", startTime);
				//printf("%d \t %f \t %f %f \n", record.seq, record.sendTime, record.receiveTime, get_time(result[4]));
			}

			record.detectionDuration = -1.0;
			rep.push_back(record);

			//if (count == 0)
				//cout << line << endl;
			count ++;

		}
	}
	else {
		cout << "cannot open file " << filename << endl;
	}


	//Print();



}

void Analyzer::Print()
{
	int count = 0;
	double sum = 0.0;
	int elem = 0;
	int total = 0;
	int expected = 200;
	double detection_time;
	double overhead=0;
	for (vector<rep_t>::const_iterator it = rep.begin(); it!=rep.end(); ++it) {

		//cout << it->seq << endl;
		//break;

		if (it->seq >= 300 && it->seq < 2300) {
			if (it->detectionDuration > 0) {
				//sum += ((it->receiveTime-it->sendTime)-it->detectionDuration);
				sum += (it->receiveTime-it->sendTime);

				overhead += ((it->receiveTime-it->sendTime)-it->detectionDuration);

				detection_time += it->detectionDuration;
				elem ++;
			}

			printf("seq %06d \t \t delay: %16f \t \t deteion duration % 16f \t \t communication delay %16f\n", it->seq, (it->receiveTime < 0.0 ? -1.0 : (it->receiveTime-it->sendTime))
					, it->detectionDuration, (it->receiveTime < 0 ? -1 : (it->receiveTime-it->sendTime)-it->detectionDuration));



			total ++;

		}

		if (it->seq != expected) {
			cout << "expected " << expected << endl;
			break;
		}

		expected ++;


	}


	printf("avg detection time %lf\n", detection_time/elem);
	printf("frame %d received out of %d\n ", elem, total);

	printf("avg processing time %lf \n", sum/elem);
	printf("avg communication delay %16f\n", overhead/elem);
}

void Analyzer::UpdateDetectionTime(const string& filename) {

	string line;
	double currTime = -1;
	int currSeq = -1;

	ifstream myfile(filename);
	if (myfile.is_open()) {
		int count=0;
		while(getline(myfile,line)){
			if (line.substr(0,3) != "seq")
				continue;
			//if (count == 0)
				//cout << line << endl;
			vector<string> result;
			boost::split(result, line, boost::is_any_of("\t "), boost::token_compress_on);

			if (result[2].substr(0,strlen("Before")) == "Before") {
				auto seq = stoi(result[1]);
				if (seq > currSeq) {
					currSeq = seq;
					currTime = get_time(result[4]);
				}
			}
			else if (result[2].substr(0,strlen("After")) == "After") {
				auto seq = stoi(result[1]);
				if (seq == currSeq) {
					auto time = get_time(result[4]);
					auto update_rep = [&](double duration) {
						if (rep[0].seq > currSeq)
							return;
						int i=0;
						for (; i<rep.size(); i++) {
							if (rep[i].seq == currSeq)
								break;
						}
						if (i == rep.size())
							return;
						rep[i].detectionDuration = duration;


						//rep[0].detectionDuration = duration;
					};

					update_rep(time-currTime);

				}
			}

			if (count < 0) {

				//cout << result[1] << endl;
				printf("index%d: curr seq %d curr time %lf detection duration %f\n", count, currSeq, currTime, rep[0].detectionDuration);
			}

			count ++;


		}
	}
	else {
		cout << "cannot open file" << endl;
	}

	for (vector<rep_t>::const_iterator it = rep.begin(); it!=rep.end(); ++it) {
		assert((it->receiveTime < 0 && it->detectionDuration < 0) || (it->receiveTime > 0 && it->detectionDuration > 0));
	}
	Print();

}

int main(int argc, char *argv[]) {

	Analyzer analyzer("/home/hasan/MyProjects/DS_ImageProcessing/ImageProcessor/raw_data/delay0_prev1.txt", 3000);

	analyzer.UpdateDetectionTime("/home/hasan/MyProjects/DS_ImageProcessing/ImageProcessor/raw_data/info.txt");

	return 0;
}
