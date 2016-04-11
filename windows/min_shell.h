#ifndef MIN_SHELL_H
#define MIN_SHELL_H
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include "ConsoleColor.h"

using namespace std;

enum CmdType {
	NORMAL_T,
	IMMEDIATE_T,
	FILE_T
};

class min_shell {
public:
	void run();
	min_shell();
private:
	CmdType cmdType;
	string command;
	string ExePath();
	void show_prompt();
	CmdType get_command(string&);
	void handle_command(const string& , const CmdType&);
	void execute_single(const string&);
	void execute_file(const string&);
};
#endif

