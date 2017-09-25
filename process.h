#pragma once

#include "../oak/std_inc.h"

typedef std::vector<std::string> string_array;
typedef std::pair<std::string, std::string> string_pair;

// ��������.
void ParseArgs(int argc, char * argv[], string_array & commands, string_array & blocks);

/**
 * ִ������
 * @return true��ʾ������false��ʾ�˳�.
 */
bool ProcessCommands(const string_array & commands);

/**
 * ����ģ�鲢ִ��.
 */
bool ProcessBlocks(const string_array & blocks);
