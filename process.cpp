#include "process.h"

#include "../oak/boost_inc.h"
#include "../oak/block_factory.h"
#include "../oak/block_queue.h"

// 帮助信息.
const std::string HELP_MESSAGE =
" commands and usage as below : \n"
"    -h                 help, print usage message\n"
// "    -h=<block_name>    help on the block, print usage message of the block\n"
"    -b=<block_name> prop=value ...    add block with properties and values\n";


const std::string BLOCK_MESSAGE[2] = 
{
	" + Create block Ok    : %s \n",
	" ! Create block Error : %s \n"
};

const std::string PARAM_MESSAGE[2] =
{
	" + Set param Ok    : %s = %s \n",
	" ! Set param Error : %s = %s \n"
};

const std::string WORK_MESSAGE[2] =
{
	" + Blocks work Ok \n",
	" ! Blocks work Error \n"
};


void ParseArgs(int argc, char * argv[], string_array & commands, string_array & blocks)
{
	for (int i = 1; i < argc; i++)
	{
		std::string temp(argv[i]);
		if (temp.size() < 2)
			continue;

		if (temp[0] != '-')
			blocks.push_back(temp);
		else
		{
			if (temp[1] == 'b')
				blocks.push_back(temp);
			else
				commands.push_back(temp);
		}
	}
}

bool ProcessCommands(const string_array & commands)
{
	std::map<std::string, std::string> cmds;

	// 1. 解析为命令组
	string_array strs;
	for (unsigned int i = 0; i < commands.size(); i++)
	{
		algo::split(strs, commands[i], boost::is_any_of("="));
		
		string_pair tempcmd;
		if (strs.size() >= 1)
			tempcmd.first = strs[0].substr(1, std::string::npos);
		if (strs.size() >= 2)
			tempcmd.second = strs[1];

		cmds[tempcmd.first] = tempcmd.second;
	}
	
	// 2. 执行命令组.
	if (cmds.find("h") != cmds.end())
	{
		std::cout << HELP_MESSAGE;
		return false;
	}

	return true;
}

bool ProcessBlocks(const string_array & blocks)
{
	if (blocks.empty())
		return true;

	std::vector<string_pair> blkInfos;

	// 1. 解析为命令组
	string_array strs;
	for (unsigned int i = 0; i < blocks.size(); i++)
	{
		algo::split(strs, blocks[i], boost::is_any_of("="));

		string_pair tempinfo;
		if (strs.size() >= 1)
			tempinfo.first = strs[0];
		if (strs.size() >= 2)
			tempinfo.second = strs[1];

		blkInfos.push_back(tempinfo);
	}

	// 2.构建模块.
	oak::BlockFactory::Instance()->Register(oak::CreateInnerBlocks);
	
	oak::BlockQueue bq;
	for (auto it = blkInfos.begin(); it != blkInfos.end(); it ++)
	{
		if (it->first == "-b")
		{
			bool ret = bq.Append(oak::BlockFactory::Instance()->CreateBlock(it->second));
			std::cout << (boost::format(BLOCK_MESSAGE[ret? 0 : 1]) % it->second).str();
			if (! ret)
				return false;
		}
		else
		{
			oak::BlockBase * block = bq.GetBlock(-1);
			bool ret = (block != nullptr && block->SetParam(it->first, it->second));

			std::cout << (boost::format(PARAM_MESSAGE[ret ? 0 : 1]) % it->first % it->second).str();
			if (!ret)
				return false;
		}
	}

	// 3.执行模块.
	int ret = bq.Work();
	std::cout << WORK_MESSAGE[ret == oak::BWR_OK_FINISH ? 0 : 1];

	return ret == oak::BWR_OK_FINISH;
}