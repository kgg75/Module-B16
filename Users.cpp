#include <iostream>
#include <fstream>
#include <exception>
#include <nlohmann/json.hpp>
#include "Users.h"
#include "ServiceFunc.h"
#include "Converter.h"
#include "Constants.h"

using json = nlohmann::json;
//using namespace std;

//private:
void Users::CheckArraySize() {	// проверка наличия свободного места в векторе и его увеличение при необходимости
	if (user.capacity() <= usersCount)
		user.reserve(user.capacity() * 2);	//	при заполнении вектора каждый раз увеличиваем его ёмкость вдвое
}


int Users::FindLogin(const std::string& _login, const bool _show_message) const {	// проверка соответствия введённого login существующим; возврат порядкового номера в случае успеха
	for (auto i = 1; i <= usersCount; i++)
		if (_login == user[i].login)
			return i;

	if (_show_message)
		std::cout << "Пользователь с таким именем не существует!\n";
	return 0;
}


bool Users::SetName(std::string& _name) {
	std::cout << "Фамилия, имя (длина " << MIN_NAME_LENGTH << "-" << MAX_NAME_LENGTH << " символов, пробелы разрешены): "; //cin >> _name;
	_name = get_string_from_console();
	if (!check_name(_name, "имя", MIN_NAME_LENGTH, MAX_NAME_LENGTH))
		return false;
	return true;
}


bool Users::SetLogin(std::string& _login) {
	std::cout << "Учётное имя (login) (длина " << MIN_LOGIN_LENGTH << "-" << MAX_LOGIN_LENGTH << " символов, пробелы не допускаются): ";
	_login = get_string_from_console();
	if (!check_name(_login, "login", MIN_LOGIN_LENGTH, MAX_LOGIN_LENGTH) || !check_spaces(_login, "login"))	// проверка синтаксиса
		return false;
	if (lowercase_s(_login) == SERVICE_LOGIN) {	// попытка регистрации под служебным login
		std::cout << "Регистрироваться под таким именем нельзя!\n";
		return false;
	}
	if (FindLogin(_login, false) > 0) {	// такой login уже есть
		std::cout << "Пользователь с таким login уже зарегистрирован!\n";
		return false;
	}
	return true;
}


bool Users::SetPassword(std::string& _password) {
	std::cout << "Пароль: ";
	_password = get_value_from_console();
	if (!check_name(_password, "пароль", MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH) || !check_spaces(_password, "пароль")) {
		_password = "";
		return false;
	}
	std::cout << "Повторите пароль: ";
	std::string password2 = get_value_from_console();
	if (_password != password2) {
		std::cout << "Пароли не совпадают!\n";
		_password = "";
		return false;
	}
	sha1(shaPwdArray, _password.c_str(), _password.length());	// рассчитываем хэш пароля и сохраняем его в shaPwdArray
	return true;
}

//public:
Users::Users() {
	user.reserve(INITIAL_COUNT);
	user.emplace_back(0, SERVICE_NAME, SERVICE_LOGIN, shaPwdArray);	//	передаём в конструктор пустой shaPwdArray
}


//Users::~Users() {}

bool Users::NewUser() {	// регистрация нового пользователя
	CheckArraySize();
	bool result = false;
	std::string name, login, password;
	std::cout << "Для регистрации введите следующие данные пользователя:\n";

	if (SetName(name) && SetLogin(login) && SetPassword(password)) {
		idsCount++;
		usersCount++;
		currentUser = usersCount;
		user.emplace_back(idsCount, name, login, shaPwdArray, false, true);
		isChanged = true;
		result = true;
	}
	return result;
}


bool Users::Login() {	// вход пользователя в чат
	std::string login;
	std::cout << "Укажите условное имя (login): ";
	std::cin >> login;
	if (!check_empty_name(login, "имя"))
		return false;
	int findedUser = FindLogin(login);
	if (findedUser == 0)
		return false;

	std::string password;
	std::cout << "Укажите пароль: ";
	std::cin >> password;

	sha1(shaPwdArray, password.c_str(), password.length());
	if (user[findedUser].pass_sha1_hash != shaPwdArray) {
		std::cout << "Неверный пароль!\n";
		return false;
	}
	currentUser = findedUser;
	user[currentUser].logged = true;
	std::cout << "Пользователь " << user[currentUser].login << " вошёл в чат.\n";
	return true;
}


void Users::Logout() {	// выход пользователя из чата
	user[currentUser].logged = false;
	std::cout << "Пользователь " << user[currentUser].login << " вышел из чата.\n";
	currentUser = 0;	// текущий служебный пользователь 'all'
}


int Users::ChooseUser() {	// выбор получателя для отправки сообщения; возвращает id пользователя
	std::string login;
	std::cout << "Укажите условное имя (login) получателя: ";
	std::cin >> login;
	if (!check_empty_name(login, "имя"))
		return -1;
	int findedUser = FindLogin(login);
	if (findedUser == 0)	// введённый login не найден
		return -1;
	return user[findedUser].id;
}


bool Users::DeleteUser() {	// удаление учётной записи пользователя
	user.erase(user.begin() + currentUser);
	usersCount--;
	currentUser = 0;
	isChanged = true;
	return true;
}


bool Users::ChangeRegData(Users::RegDataType _type) {	// изменение учётной записи пользователя
	std::string str;
	switch (_type) {
		case __name:
			if (!SetName(str))
				return false;
			user[GetCurrentUser()].name = str;
			break;
		case __login:
			if (!SetLogin(str))
				return false;
			user[GetCurrentUser()].login = str;
			break;
		case __pwd:
			if (!SetPassword(str))
				return false;
			user[GetCurrentUser()].pass_sha1_hash = shaPwdArray;	// копируем хэш нового пароля
			break;
	}
	isChanged = true;
	return true;
}


bool Users::Open() {	// чтение данных пользователей из внешнего файла
	std::fstream users_data;
	users_data.open(USER_DATA_FILE_NAME, std::ios::in);
	if (!users_data.is_open()) {
		std::cout << "Неизвестная ошибка при загрузке данных пользователей!\n";
		return false;
	}
	bool result = false;
	json jsonObj;
	CheckArraySize();
	try {	// попытка создания json-объекта и чтения из него
		users_data >> jsonObj;
		for (auto i = 0; i < jsonObj.size(); i++) {
			for (auto j = 0; j < SHA1_HASH_LENGTH_UINTS; j++)
				shaPwdArray[j] = jsonObj[i]["sha1"][j];	// загрузка хэша пароля в переменную shaPwdArray
			user.emplace_back(	// помещение учётных данных в vector
				jsonObj[i]["id"],
				utf8_to_cp1251(jsonObj[i]["name"]),	// перекодирование строк из UTF-8
				utf8_to_cp1251(jsonObj[i]["login"]),	// перекодирование строк из UTF-8
				shaPwdArray,
				jsonObj[i]["locked"],
				false
			);
		}
		usersCount = (int)jsonObj.size();
		idsCount = user[usersCount].id;
		result = true;
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
		std::cout << "Ошибка чтения файла данных!\n";	// ошибка будет при первом запуске чата, т.к. данные ещё не ни разу сохранялись
	}
	users_data.close();
	return result;
}

bool Users::Save() {	// сохранение данные пользователей во внешний файл
	std::fstream users_data;
	users_data.open(USER_DATA_FILE_NAME, std::ios::out);
	if (!users_data.is_open()) {
		std::cout << "Неизвестная ошибка при сохранении данных пользователей!\n";
		return false;
	}
	bool result = false;
	json jsonObj;
	try {	// попытка создания json-объекта и записи в него
		for (auto i = 0; i < usersCount; i++) {
			jsonObj[i]["id"] = user[i + 1].id;
			jsonObj[i]["name"] = cp1251_to_utf8(user[i + 1].name);	// перекодирование строк в UTF-8
			jsonObj[i]["login"] = cp1251_to_utf8(user[i + 1].login);	// перекодирование строк в UTF-8
			jsonObj[i]["sha1"] = user[i + 1].pass_sha1_hash.data;	// jsonObj работает только с публичными data
			jsonObj[i]["locked"] = user[i + 1].locked;
		}
		users_data << std::setw(4) << jsonObj;
		result = true;
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
		std::cout << "Ошибка записи данных в файл!\n";
	}
	users_data.close();
	return result;
}


void Users::Show(const int _user_index) {	// отображение данных указанного пользователя
	int index = (_user_index == -1) ? currentUser : _user_index;	// если индекс не указан, то возвращаем данные текущего пользователя
	std::cout << "Информация о пользователе:\n";
	std::cout << "\tid \t= " << user[index].id << '\n';
	std::cout << "\tname\t= " << user[index].name << '\n';
	std::cout << "\tlogin\t= " << user[index].login << '\n';
	std::cout << "\tlocked\t= " << user[index].locked << '\n';
	std::cout << "\tlogged\t= " << user[index].logged << '\n';
}


void Users::List() {	// вывод списка всех пользователей
	std::cout << "Список всех пользователей:\n\tid\tимя\tlogin\n"; 
	for (auto i = 1; i <= usersCount; i++)
		std::cout << '\t' << user[i].id << '\t' << user[i].name << '\t' << user[i].login << '\n';
}


int Users::GetCurrentUser() const {	// порядковый номер текущего пользователя
	return currentUser;
}


int Users::GetUserId(const int _user_index) const {	// id указанного пользователя
	return user[(_user_index == -1) ? currentUser : _user_index].id;	// если индекс не указан, то возвращаем id текущего пользователя
}


std::string Users::GetUserLogin(const int _id) const {	// login пользователя, указанного по id
	if (_id == -1)	// если id не указан, то возвращаем login текущего пользователя
		return user[currentUser].login;
	else {
		for (auto i = 0; i < usersCount; i++)
			if (user[i].id == _id)
				return user[i].login;
		return "";
	}
}


bool Users::IsChanged() const {
	return isChanged;
}