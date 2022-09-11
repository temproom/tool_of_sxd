#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <time.h>
#include "locker.h"

template <class T>
class block_queue
{
public:
	block_queue(int max_size = 1000)
	{
		if (max_size <= 0)
		{
			exit(-1);
		}

		m_max_size = max_size;
		m_array = new T[max_size];
		m_size = 0;
		m_front = -1;
		m_back = -1;
	}

	void clear()
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		m_mutex.lock();
		m_size = 0;
		m_front = -1;
		m_back = -1;
	}

	~block_queue()
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		if (m_array != NULL)
			delete[] m_array;
	}
	//判断队列是否满了
	bool full()
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		if (m_size >= m_max_size)
		{
			return true;
		}
		return false;
	}
	//判断队列是否为空
	bool empty()
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		if (0 == m_size)
		{
			return true;
		}
		return false;
	}
	//返回队首元素
	bool front(T &value)
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		if (0 == m_size)
		{
			return false;
		}
		value = m_array[m_front];
		return true;
	}
	//返回队尾元素
	bool back(T &value)
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		if (0 == m_size)
		{
			return false;
		}
		value = m_array[m_back];
		return true;
	}

	//目前队列大小
	int size()
	{
		int tmp = 0;

		std::unique_lock<std::mutex>lock(m_mutex);
		tmp = m_size;

		return tmp;
	}

	int max_size()
	{
		int tmp = 0;

		std::unique_lock<std::mutex>lock(m_mutex);
		tmp = m_max_size;

		return tmp;
	}
	//往队列添加元素，需要将所有使用队列的线程先唤醒
	//当有元素push进队列,相当于生产者生产了一个元素
	//若当前没有线程等待条件变量,则唤醒无意义
	bool push(const T &item)
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		if (m_size >= m_max_size)
		{

			m_cond.notify_all();
			return false;
		}

		m_back = (m_back + 1) % m_max_size;
		m_array[m_back] = item;

		m_size++;

		m_cond.notify_all();
		return true;
	}
	//pop时,如果当前队列没有元素,将会等待条件变量
	bool pop(T &item)
	{		
		//m_mutex.lock();
		std::unique_lock<std::mutex>lock(m_mutex);
		m_cond.wait(lock, [&]() { return m_size > 0; });

		m_front = (m_front + 1) % m_max_size;
		item = m_array[m_front];
		m_size--;
		//m_mutex.unlock();
		return true;
	}

	//增加了超时处理
	bool pop(T &item, int ms_timeout)
	{
		struct timespec t = { 0, 0 };
		struct timeval now = { 0, 0 };
		gettimeofday(&now, NULL);
		std::unique_lock<std::mutex>lock(m_mutex);
		if (m_size <= 0)
		{
			t.tv_sec = now.tv_sec + ms_timeout / 1000;
			t.tv_nsec = (ms_timeout % 1000) * 1000;
			if (!m_cond.timewait(m_mutex.get(), t))
			{
				return false;
			}
		}

		if (m_size <= 0)
		{
			std::unique_lock<std::mutex>lock(m_mutex);
			return false;
		}

		item = m_array[m_front];
		m_front = (m_front + 1) % m_max_size;		
		m_size--;
		std::unique_lock<std::mutex>lock(m_mutex);
		return true;
	}

private:
	std::mutex m_mutex;
	std::condition_variable m_cond;

	T *m_array;
	int m_size;		//当前队列大小
	int m_max_size;	//最大队列大小
	int m_front;	//队首下标2
	int m_back;		//队尾下标
};

#endif
