// tool_of_sxd.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "sxd.h"
#include <windows.h>
using namespace std;

int main(int argc, char *argv[])
{
	//common::setfont(20);
	std::srand((unsigned)std::time(NULL));
	argc = 1;
	sxd::sql_pool();
	if (argc >= 2)
		sxd::run(argv[1], true);
	else
		sxd::run("menu", false);

	return 0;
}

