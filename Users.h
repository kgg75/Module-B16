#pragma once
#include <string>
#include <vector>
#include "User.h"


class Users {	// для типов bool возвращаемое значение true в случае успеха, false при неудачном заваершении
	private:
		std::vector<User> user;	// вектор с данными пользователей
		int usersCount = 0;	// количество пользователей
		int idsCount = 0; // количество использованных идентификаторов
		int currentUser = idsCount; // порядковый номер текущего пользователя (начинается с 1)
		bool isChanged = false;	// флаг наличия изменений в данных пользователей
		SHA1PwdArray shaPwdArray;	// временное хранилище хэша пароля
// функции
		void CheckArraySize();	// проверка наличия свободного места в векторе и его увеличение при необходимости
		int FindLogin(const std::string& _login, const bool _show_message = true) const;	// проверка соответствия введённого login существующим; возврат порядкового номера в случае успеха
		bool SetName(std::string& _name);
		bool SetLogin(std::string& _login);
		bool SetPassword(std::string& _password);
	public:
		enum RegDataType {
			__name,
			__login,
			__pwd
		};
		Users();
		//~Users();
		bool NewUser();	// регистрация нового пользователя
		bool Login();	// вход пользователя в чат
		void Logout();	// выход пользователя из чата
		int ChooseUser();	// выбор получателя для отправки сообщения; возвращает id пользователя
		bool Open();	// чтение данных пользователей из внешнего файла
		bool Save();	// сохранение данных пользователей во внешний файл
		bool DeleteUser();	// удаление учётной записи пользователя
		bool ChangeRegData(Users::RegDataType _type);	// изменение учётной записи пользователя
		void Show(const int _user_index = -1);	// отображение данных указанного пользователя; по умолчанию - текущего
		void List();	// вывод списка всех пользователей
// getters
		int GetCurrentUser() const;	// порядковый номер текущего пользователя
		int GetUserId(const int _user_index = -1) const;	// id указанного пользователя; по умолчанию - текущего
		std::string GetUserLogin(const int _id = -1) const;	// login пользователя, указанного по id; по умолчанию - текущего
		bool IsChanged() const;	// возврат флага наличия изменений в данных пользователей
};

