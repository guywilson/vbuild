#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "currenttime.h"

CurrentTime::CurrentTime() {
	updateTime();
}

void CurrentTime::updateTime() {
	time_t t;

	t = time(0);
	this->localTime = localtime(&t);
}

std::string CurrentTime::getTimeStamp() {
	time_t t = time(0);
	struct tm * now = localtime(&t);

	char szTime[20];

	snprintf(
		szTime,
		20,
		"%d-%02d-%02d %02d:%02d:%02d",
		now->tm_year + 1900,
		now->tm_mon + 1,
		now->tm_mday,
		now->tm_hour,
		now->tm_min,
		now->tm_sec);

	return std::string(szTime);
}

int CurrentTime::getYear() {
	return localTime->tm_year + 1900;
}

int CurrentTime::getMonth() {
	return localTime->tm_mon + 1;
}

int CurrentTime::getDay() {
	return localTime->tm_mday;
}

int CurrentTime::getHour() {
	return localTime->tm_hour;
}

int CurrentTime::getMinute() {
	return localTime->tm_min;
}

int CurrentTime::getSecond() {
	return localTime->tm_sec;
}
