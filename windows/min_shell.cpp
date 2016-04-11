#include "min_shell.h"
#define MAX_LEN 200
min_shell::min_shell() {
	cmdType = NORMAL_T;
}

void min_shell::run() {
	do {
		show_prompt();
		cmdType = get_command(command);
		handle_command(command, cmdType);
	} while (1);
}

string min_shell::ExePath() {
    char buffer[MAX_LEN];
    GetModuleFileName( NULL, buffer, MAX_LEN);
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}

void min_shell::show_prompt() {
	const time_t t = time(NULL);
	struct tm* current_time = localtime(&t);

    cout << green << ExePath() << "\t" << blue 
		<< "[" << current_time->tm_hour
        << ":" << current_time->tm_min
        << ":" << current_time->tm_sec << "]\n$ " << white;
}

CmdType min_shell::get_command(string & command) {
	CmdType res = NORMAL_T;
	getline(cin, command);
	
	if (command.length() >= 3) {
		string prefix = command.substr(0, 2);
		if (prefix == "-f") {
			command = command.substr(3);
			res = FILE_T;
		} else if (prefix == "-i") {
			command = command.substr(3);
			res = IMMEDIATE_T;
		}
	}
	return res;
}

void min_shell::handle_command(const string& command, const CmdType& cmdType) {
	if (command == "exit") {
		cout << yellow << "最后祝您，生活愉快!\n" << white;
		exit(0);
	}

	if (cmdType == NORMAL_T) {
		system(command.c_str());
	} else if (cmdType == IMMEDIATE_T) {
		execute_single(command);
	} else if (cmdType == FILE_T) {
		execute_file(command);
	}
}

void min_shell::execute_single(const string& command) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	char command_buf[MAX_LEN];
	sprintf(command_buf, "%s", command.c_str());

	if (!CreateProcess(NULL,
		command_buf,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi)) {
		fprintf(stderr, "Create Process Failed, please check the path\n");
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

void min_shell::execute_file(const string& filePath) {
	ifstream ifile(filePath.c_str(), ios::in);
	if (!ifile) {
		cout << "Open file Failed, please check the path\n";
	} else {
		string content;
		while (getline(ifile, content)) {
			cout << content << endl;
			execute_single(content);
		}
	}
	ifile.close();
}

