#pragma once

#include "../oak/std_inc.h"

typedef std::vector<std::string> string_array;
typedef std::pair<std::string, std::string> string_pair;

// 解析参数.
void ParseArgs(int argc, char * argv[], string_array & commands, string_array & blocks);

/**
 * 执行命令
 * @return true表示继续，false表示退出.
 */
bool ProcessCommands(const string_array & commands);

/**
 * 构建模块并执行.
 */
bool ProcessBlocks(const string_array & blocks);
