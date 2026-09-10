#pragma once

#include <string>

#include <time.h>

class CurrentTime {
	private:
		struct tm * localTime;

		void updateTime();

	public:
		CurrentTime();

		static std::string getTimeStamp();

		int	getYear();
		int	getMonth();
		int	getDay();
		int	getHour();
		int	getMinute();
		int	getSecond();
};
