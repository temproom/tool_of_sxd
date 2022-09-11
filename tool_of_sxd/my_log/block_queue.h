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
	//�ж϶����Ƿ�����
	bool full()
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		if (m_size >= m_max_size)
		{
			return true;
		}
		return false;
	}
	//�ж϶����Ƿ�Ϊ��
	bool empty()
	{
		std::unique_lock<std::mutex>lock(m_mutex);
		if (0 == m_size)
		{
			return true;
		}
		return false;
	}
	//���ض���Ԫ��
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
	//���ض�βԪ��
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

	//Ŀǰ���д�С
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
	//���������Ԫ�أ���Ҫ������ʹ�ö��е��߳��Ȼ���
	//����Ԫ��push������,�൱��������������һ��Ԫ��
	//����ǰû���̵߳ȴ���������,����������
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
	//popʱ,�����ǰ����û��Ԫ��,����ȴ���������
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

	//�����˳�ʱ����
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
	int m_size;		//��ǰ���д�С
	int m_max_size;	//�����д�С
	int m_front;	//�����±�2
	int m_back;		//��β�±�
};

#endif
