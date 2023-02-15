#pragma once
#include <string>
//#include <cstdlib>
//#include <windows.h>
#include "Users.h"
#include "Messages.h"
#include "Constants.h"


void print_help(const CommandSpace* _commandSpace);	// печать справки по командам
void AtStart();	// начальные сообщения
//int _get_command(const CommandSpace* _commandSpace);	// получение команды с консоли, выдача соотвествующего кода действия
//int get_command(const CommandSpace* _commandSpace);	// получение команды с консоли, выдача соотвествующего кода действия
int get_command(const CommandSpace* _commandSpace, const std::string _msg, const std::string& _user_name = "");	// получение команды с консоли, выдача соотвествующего кода действия
int Chat(Users& _users, Messages& _messages);
