#pragma once
#include <iostream>
#include <string>
#include "Users.h"


void lowercase(std::string&);	// �������������� ������ � ������ �������
std::string lowercase_s(const std::string& _str);	// �������������� ������ � ������ �������
bool check_empty_name(const std::string& _str, const std::string& _subject);	// �������� ������ �� �������
bool check_spaces(const std::string& _str, const std::string& _subject);	// �������� ������ �� �������
bool check_name(const std::string& _str, const std::string& _subject, const size_t _minLength, const size_t _maxLength);	// �������� ������ �� �������, �����������/������������ �����
std::string get_value_from_console();	// ��������� �������� � �������
std::string get_string_from_console();	// ��������� ������ � ������� � � ������� �� ��������� � �������� ��������
std::string GetTime();	// ��������� �������� �������
bool QuestionYN(const std::string& _message);	// ������ � ����� ���������� ������ (��/���)