// biu.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#pragma comment(lib, "oak.lib")

#include "process.h"

int main(int argc, char * argv[])
{
	string_array commands;
	string_array blocks;

	ParseArgs(argc, argv, commands, blocks);

	if (commands.empty() && blocks.empty())
		commands.push_back("-h");

	if (!ProcessCommands(commands))
		return -1;

	if (!ProcessBlocks(blocks))
		return -1;
    
	return 0;
}

