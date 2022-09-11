#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <thread>
#include <mutex>
#include <condition_variable>

class sem
{
public:
	sem(long temp=0):count(temp){}

	void init(long temp)
	{
		count = temp;
	}
	void wait()
	{
		std::unique_lock<std::mutex>lock(mx);
		cond.wait(lock, [&]() { return count > 0; });
		--count;
	}
	void post()
	{
		std::unique_lock<std::mutex>lock(mx);
		++count;
		cond.notify_one();
	}
	//sem(sem& c_sem):count(c_sem.count){}

private:
	long count;
	std::mutex mx;
	std::condition_variable cond;	
};
#endif#pragma once
