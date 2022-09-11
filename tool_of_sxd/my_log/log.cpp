#include "stdafx.h"
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "log.h"
#include <thread>
#include <windows.h>

Log::Log()
{
	m_txt_count = 1;
	m_log_count = 1;
	m_is_async = false;
}

Log::~Log()
{
	if (ofile_log.is_open())
	{
		ofile_log.close();
	}
	if (ofile_txt.is_open())
	{
		ofile_txt.close();
	}
}
//�첽��Ҫ�����������еĳ��ȣ�ͬ������Ҫ����
bool Log::init(const std::string& player_name, int close_log, int max_queue_size)
{
	//���������max_queue_size,������Ϊ�첽
	if (max_queue_size >= 1)
	{
		m_is_async = true;
		m_log_queue = new block_queue<std::string>(max_queue_size);
		m_txt_queue = new block_queue<std::string>(max_queue_size);
		std::thread tid_log(work_log);
		tid_log.detach();
		std::thread tid_txt(work_txt);		
		tid_txt.detach();
	}

	m_close_log = close_log;

	std::time_t now = std::time(0);
	std::string path = "log";
	if (!boost::filesystem::exists(path))  //�ƶ��ļ��д�����  
	{
		//�ļ��в�����;   
		boost::filesystem::create_directory(path);  //�ļ��в����ڡ����� 
	}

	//����txt�ļ�����¼���
	std::string patht = path + "\\" + player_name + common::to_string(now, "%Y-%m-%d.txt");
	
	m_mutex.lock();
	ofile_txt.open(patht, std::ios::binary | std::ios::out | std::ios::app);
	//std::ofstream ofilet(patht, std::ios::binary | std::ios::out | std::ios::app);
	if (!ofile_txt.is_open())
	{
		std::string str_txt = "����txt�ļ�ʧ�ܣ�\n";
		std::cout << str_txt << std::flush;
		write_log(player_name, str_txt);
		m_mutex.unlock();
		return false;
	}
	m_mutex.unlock();

	if (!m_close_log)
	{
		//����log�ļ�����¼��־
		std::string pathl = path + "\\" + player_name + common::to_string(now, "%Y-%m-%d.log");
		m_mutex.lock();
		ofile_log.open(pathl, std::ios::binary | std::ios::out | std::ios::app);
		std::ofstream ofilel(pathl, std::ios::binary | std::ios::out | std::ios::app);
		if (!ofile_log.is_open())
		{
			std::string str_log = "����log�ļ�ʧ�ܣ�\n";
			std::cout << str_log << std::flush;
			write_log(player_name, str_log);
			m_mutex.unlock();
			return false;
		}
		m_mutex.unlock();
	}
	
	return true;
}

void Log::write_log(const std::string& name,const std::string& message, int hwnd, bool log_file , bool time )
{
	std::time_t now = std::time(0);
	std::string log_str;
	std::string txt_str;

	m_mutex.lock();
	if (hwnd)
	{
		if (time)
		{
			if (hwnd < 0)
			{
				std::cout << common::to_string(now, "%H:%M:%S") << std::flush;
			}			
			else
			{
				SendMessage((HWND)hwnd, EM_SETSEL, SendMessage((HWND)hwnd, WM_GETTEXTLENGTH, 0, 0), SendMessage((HWND)hwnd, WM_GETTEXTLENGTH, 0, 0));
				SendMessage((HWND)hwnd, EM_REPLACESEL, 0, (LPARAM)(common::to_string(now, "%H:%M:%S") + " ").c_str());
			}
			txt_str += common::to_string(now, "%H:%M:%S");
		}
		if (hwnd < 0)
			std::cout << boost::str(boost::format("  ��%1%�� ��%2%\r\n") % name %message) << std::flush;
		else
		{
			SendMessage((HWND)hwnd, EM_SETSEL, SendMessage((HWND)hwnd, WM_GETTEXTLENGTH, 0, 0), SendMessage((HWND)hwnd, WM_GETTEXTLENGTH, 0, 0));
			SendMessage((HWND)hwnd, EM_REPLACESEL, 0, (LPARAM)(message + "\r\n").c_str());
		}
		txt_str += boost::str(boost::format("  ��%1%�� ��%2%\r\n") % name %message);
	}
	if (log_file)
	{
		if (time)
			log_str += common::to_string(now, "%H:%M:%S");
		log_str += boost::str(boost::format("  ��%1%�� ��%2%\r\n") % name %message);
	}
	m_mutex.unlock();
	if (m_is_async && !m_log_queue->full())
	{
		m_txt_queue->push(txt_str);
		m_log_queue->push(log_str);
	}
	else
	{
		m_mutex.lock();
		ofile_log << log_str << std::flush;
		ofile_txt << txt_str << std::flush;
		m_mutex.unlock();
	}
}

void Log::flush(void)
{
	m_mutex.lock();
	//ǿ��ˢ��д����������
	ofile_log << std::flush;
	ofile_txt << std::flush;
	m_mutex.unlock();
}