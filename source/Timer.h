#ifndef TIMER_H
#define TIMER_H

#include "Clock.h"

namespace dbb {

	class Timer
	{
	 public:
		 Timer(std::function<bool()> callable);
		 virtual bool start(unsigned int period);
		 virtual bool stop();
		 virtual ~Timer();
	 protected:
		 Clock clock;
		 bool active = false;
		 std::function<bool()> callable;
		 std::thread thrd;
		 std::future<bool> fut;
	};

};
#endif
