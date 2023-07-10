/*
 * analyzer.h
 *
 *  Created on: Sep 11, 2020
 *      Author: hasan
 */

#ifndef SRC_ANALYZER_H_
#define SRC_ANALYZER_H_

#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>


class Analyzer {
public:
	typedef struct {
		int seq;
		double sendTime;
		double receiveTime;
		double detectionDuration;
	} rep_t;


	Analyzer(const std::string & filename, int listSize=100);
	void UpdateDetectionTime(const std::string& filename);

private:
	std::vector<rep_t> rep;

	inline double get_time(std::string& str) {
			std::vector<std::string> res;
			boost::split(res, str, boost::is_any_of(":"));
			double sec = stod(res[1])*60.0;
			std::vector<std::string> r;
			boost::split(r, res[2], boost::is_any_of("_"));
			sec += stod(r[0]);
			//stof(r[1])/1000000);
			auto usec_by_mil = stod(r[1])/1000000.;

			return sec+usec_by_mil;
		};

	void Print();



};

#endif /* SRC_ANALYZER_H_ */
