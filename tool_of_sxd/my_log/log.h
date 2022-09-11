#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdarg.h>
#include <thread>
#include "block_queue.h"
#include <common.h>
#include <boost/format.hpp>

class Log
{
public:
	//C++11�Ժ�,ʹ�þֲ������������ü���
	static Log *get_instance()
	{
		static Log instance;
		return &instance;
	}

	static void work_log()
	{
		Log::get_instance()->async_write_log();
	}
	static void work_txt()
	{
		Log::get_instance()->async_write_txt();
	}
	//��ѡ��Ĳ�������־�ļ�����־��������С����������Լ����־������
	bool init(const std::string& player_name, int close_log, int max_queue_size = 100);

	//hwnd=0,�������Ϣ�����ڣ�hwnd!=0,�����Ϣ�����ڡ�hwnd=1,������������.log_fileд����־��time����ʱ��
	void write_log(const std::string& name, const std::string& message, int hwnd = -1, bool log_file = true, bool time = true);

	void flush(void);

private:
	Log();
	virtual ~Log();
	void async_write_log()
	{
		std::string single_log;
		//������������ȡ��һ����־string��д���ļ�
		while (m_log_queue->pop(single_log))
		{
			m_mutex.lock();
			ofile_log << boost::str(boost::format("%1%  %2%\r\n") % m_log_count% single_log.c_str()) << std::flush;
			m_log_count++;
			m_mutex.unlock();
		}
	}
	void async_write_txt()
	{
		std::string single_txt;
		//������������ȡ��һ����־string��д���ļ�
		while (m_txt_queue->pop(single_txt))
		{
			m_mutex.lock();
			ofile_txt << boost::str(boost::format("%1%  %2%\r\n") % m_txt_count% single_txt.c_str()) << std::flush;
			m_txt_count++;
			m_mutex.unlock();
		}
	}

private:
	long long m_log_count;  //log������¼
	long long m_txt_count;  //txt������¼
	block_queue<std::string> *m_log_queue; //��������,log
	block_queue<std::string> *m_txt_queue; //��������,txt
	bool m_is_async;                  //�Ƿ�ͬ����־λ
	std::mutex m_mutex;
	int m_close_log; //�ر���־
	std::ofstream ofile_log;
	std::ofstream ofile_txt;
};

#define LOG_DEBUG(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(0, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_INFO(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(1, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_WARN(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(2, format, ##__VA_ARGS__); Log::get_instance()->flush();}
#define LOG_ERROR(format, ...) if(0 == m_close_log) {Log::get_instance()->write_log(3, format, ##__VA_ARGS__); Log::get_instance()->flush();}

#endif#pragma once
