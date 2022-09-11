#define _CONNECTION_POOL_
#pragma once
#include <stdio.h>
#include <list>
//#include <mysql/mysql.h>
#include <database.h>
//#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "locker.h"
#include "my_log/log.h"

using namespace std;

class connection_pool
{
public:
	database* GetConnection();				 //获取数据库连接
	bool ReleaseConnection(database* conn); //释放连接
	int GetFreeConn();					 //获取连接
	void DestroyPool();					 //销毁所有连接

										 //单例模式
	static connection_pool *GetInstance();

	void init(int MaxConn, int close_log);

private:
	connection_pool();
	~connection_pool();

	int m_MaxConn;  //最大连接数
	int m_CurConn;  //当前已使用的连接数
	int m_FreeConn; //当前空闲的连接数
	std::mutex lock;
	list<database*> connList; //连接池
	sem reserve;

public:
	int m_close_log;	//日志开关
};

class connectionRAII
{

public:
	connectionRAII(database **con, connection_pool *connPool);
	~connectionRAII();

private:
	database *conRAII;
	connection_pool *poolRAII;
};

/*extern connection_pool *m_sql_pool;*/