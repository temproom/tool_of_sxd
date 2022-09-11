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
	database* GetConnection();				 //��ȡ���ݿ�����
	bool ReleaseConnection(database* conn); //�ͷ�����
	int GetFreeConn();					 //��ȡ����
	void DestroyPool();					 //������������

										 //����ģʽ
	static connection_pool *GetInstance();

	void init(int MaxConn, int close_log);

private:
	connection_pool();
	~connection_pool();

	int m_MaxConn;  //���������
	int m_CurConn;  //��ǰ��ʹ�õ�������
	int m_FreeConn; //��ǰ���е�������
	std::mutex lock;
	list<database*> connList; //���ӳ�
	sem reserve;

public:
	int m_close_log;	//��־����
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