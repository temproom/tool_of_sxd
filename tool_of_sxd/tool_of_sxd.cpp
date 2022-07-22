// tool_of_sxd.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "sxd.h"
#include <windows.h>
using namespace std;

int main(int argc, char *argv[]) {
	std::srand((unsigned)std::time(NULL));
	argc = 1;
	if (argc >= 2)
		sxd::run(argv[1], true);
	else
		sxd::run("menu", false);

	return 0;
}

