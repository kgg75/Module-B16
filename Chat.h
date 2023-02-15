#pragma once
#include <string>
//#include <cstdlib>
//#include <windows.h>
#include "Users.h"
#include "Messages.h"
#include "Constants.h"


void print_help(const CommandSpace* _commandSpace);	// ������ ������� �� ��������
void AtStart();	// ��������� ���������
//int _get_command(const CommandSpace* _commandSpace);	// ��������� ������� � �������, ������ ��������������� ���� ��������
//int get_command(const CommandSpace* _commandSpace);	// ��������� ������� � �������, ������ ��������������� ���� ��������
int get_command(const CommandSpace* _commandSpace, const std::string _msg, const std::string& _user_name = "");	// ��������� ������� � �������, ������ ��������������� ���� ��������
int Chat(Users& _users, Messages& _messages);
