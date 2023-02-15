#pragma once
#include <string>
#include "SHA1.h"


struct User {
	int id;	// уникальный идентификатор
	std::string name;	// полное имя
	std::string login;	// учётное имя (login)
	SHA1PwdArray pass_sha1_hash;	// хэш пароля
	bool locked = false;	// флаг блокировки пользователя (зарезервирован)
	bool logged = false;	// флаг, указывающий на вход пользователя

	User(
		const int _id,
		const std::string& _name,
		const std::string& _login,
		const SHA1PwdArray& _pass_sha1_hash,
		const bool locked = false,
		const bool logged = false
	);
};

