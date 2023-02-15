#pragma once
#include <iostream>
#include <string>
#include "Users.h"


void lowercase(std::string&);	// преобразование строки в нижний регистр
std::string lowercase_s(const std::string& _str);	// преобразование строки в нижний регистр
bool check_empty_name(const std::string& _str, const std::string& _subject);	// проверки строки на пустоту
bool check_spaces(const std::string& _str, const std::string& _subject);	// проверки строки на пробелы
bool check_name(const std::string& _str, const std::string& _subject, const size_t _minLength, const size_t _maxLength);	// проверки строки на пустоту, минимальную/максимальную длину
std::string get_value_from_console();	// получение значения с консоли
std::string get_string_from_console();	// получение строки с консоли и её очистка от начальных и конечных пробелов
std::string GetTime();	// получение текущего времени
bool QuestionYN(const std::string& _message);	// вопрос с двумя вариантами ответа (да/нет)