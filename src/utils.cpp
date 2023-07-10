/*
 * utils.cpp
 *
 *  Created on: Sep 4, 2020
 *      Author: hasan
 */


#include "utils.h"
#include <time.h>
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <stdio.h>


using namespace std;

#ifndef _WIN32
void image_proc_utils::print_time(char *str) {

	struct timeval tv;
	struct timezone tz;
	struct tm *tm;
	gettimeofday(&tv, &tz);
	tm=localtime(&tv.tv_sec);
	sprintf(str, "%d:%02d:%02d_%06d", tm->tm_hour, tm->tm_min,
		  tm->tm_sec, tv.tv_usec);
}

const char *image_proc_utils::print_time() {
	static char str[256];
	print_time(str);
	return str;

}
#endif
vector<string> image_proc_utils::split(const string& s, char seperator)
{
   vector<string> output;

    string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word

    return output;
}



int image_proc_utils::ServiceContainer::mLastTypeId = 0;

image_proc_utils::ServiceContainer * image_proc_utils::ServiceContainer::Instance() {
	static ServiceContainer serviceContainer;
	return &serviceContainer;
}





