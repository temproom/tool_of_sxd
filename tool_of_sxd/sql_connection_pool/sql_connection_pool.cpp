#include "stdafx.h"
#include "sqlite3.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <thread>
#include <iostream>
#include "sql_connection_pool.h"

using namespace std;

connection_pool::connection_pool()
{
	m_CurConn = 0;
	m_FreeConn = 0;
}

connection_pool *connection_pool::GetInstance()
{
	static connection_pool connPool;
	return &connPool;
}

//�����ʼ��
void connection_pool::init(int MaxConn, int close_log)
{
	m_close_log = close_log;

	for (int i = 0; i < MaxConn; i++)
	{
		database *con = new database();
		if (con == NULL)
		{
			Log::get_instance()->write_log("sql_pool", "fail to creat sql_pool!!");
		}
		connList.push_back(con);
		++m_FreeConn;
	}

	reserve.init(m_FreeConn);

	m_MaxConn = m_FreeConn;
}


//��������ʱ�������ݿ����ӳ��з���һ���������ӣ�����ʹ�úͿ���������
database *connection_pool::GetConnection()
{
	database *con = NULL;

	if (0 == connList.size())
		return NULL;

	reserve.wait();

	lock.lock();

	con = connList.front();
	connList.pop_front();

	--m_FreeConn;
	++m_CurConn;

	lock.unlock();
	return con;
}

//�ͷŵ�ǰʹ�õ�����
bool connection_pool::ReleaseConnection(database *con)
{
	if (NULL == con)
		return false;

	lock.lock();

	connList.push_back(con);
	++m_FreeConn;
	--m_CurConn;

	lock.unlock();

	reserve.post();
	return true;
}

//�������ݿ����ӳ�
void connection_pool::DestroyPool()
{

	lock.lock();
	if (connList.size() > 0)
	{
		list<database *>::iterator it;
		for (it = connList.begin(); it != connList.end(); ++it)
		{
			database *con = *it;
			delete con;
			con = NULL;
		}
		m_CurConn = 0;
		m_FreeConn = 0;
		connList.clear();
	}

	lock.unlock();
}

//��ǰ���е�������
int connection_pool::GetFreeConn()
{
	return this->m_FreeConn;
}

connection_pool::~connection_pool()
{
	DestroyPool();
}

connectionRAII::connectionRAII(database **SQL, connection_pool *connPool)
{
	*SQL = connPool->GetConnection();

	conRAII = *SQL;
	poolRAII = connPool;
}

connectionRAII::~connectionRAII()
{
	poolRAII->ReleaseConnection(conRAII);
}

