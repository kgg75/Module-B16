#include <iostream>
#include "Chat.h"
#include "ServiceFunc.h"

//using namespace std;

void print_help(const CommandSpace* _commandSpace) {	// печать справки по командам
	for (int i = 0; _commandSpace[i].command[0] != 0; i++)
		std::cout << "\t" << _commandSpace[i].command << "\t- " << _commandSpace[i].help << '\n';
}


void AtStart() {	// начальные сообщения
	std::cout.clear();
	std::cout << "Здесь будет заголовок для чата!\n";
	std::cout << SEPARATOR;
	std::cout << "Выберите действие:\n";
	print_help(GLOBAL_COMMANDS);
	std::cout << SEPARATOR;
	std::cout << "Чат запущен " << GetTime() << '\n';
	std::cout << SEPARATOR;
}


int get_command(const CommandSpace* _commandSpace, const std::string _msg, const std::string& _user_name) {	// получение команды с консоли, выдача соотвествующего кода действия
	std::cout << _user_name << _msg;	// вывод обращения
	std::string command = get_value_from_console();
	lowercase(command);	//	преобразование текста команды в нижний регистр
	for (int i = 0; _commandSpace[i].command[0] != 0; i++)	// поиск кода команды
		if (command == _commandSpace[i].command || command == _commandSpace[i].command2)
			return i;
	return -1;
}


int Chat(Users& _users, Messages& _messages) {	// главный обработчик чата
	AtStart();
	bool doing = true;
	while (doing) {
		switch (get_command(GLOBAL_COMMANDS, "Введите команду >> ")) {	// получаем глобальную команду с консоли
			case 0:	// команда reg - зарегистрироваться
				std::cout << (_users.NewUser() ? "Вы успешно зарегистрированы!\n" : "Произошла ошибка регистрации.\n");	// создаём нового пользователя
				break;
			case 1:	// команда login - вход
				if (_users.Login()) {
					int unReadedMsgs = _messages.FindUnReaded(_users.GetUserId());	// подсчёт числа непрочитанных сообщений
					if (unReadedMsgs > 0) {
						std::cout << "Есть непрочитанные сообщения (" << unReadedMsgs << "). ";
						if (QuestionYN("Показать?"))
							_messages.Read(_users.GetUserId(), true);	// вывод непрочитанных сообщений
					}
					// начало локального пространства команд
					while (_users.GetCurrentUser() != 0) {	// пока пользователь не вышел из чата
						switch (get_command(LOCAL_COMMANDS, ", введите команду >> ",  _users.GetUserLogin())) {	// получаем локальную команду с консоли
							case 0: {	// локальная команда send - отправить сообщение
									int receiverId = _users.ChooseUser();	// выбор получателя с возвратом его id
									if (receiverId != -1) {
										if (receiverId != _users.GetUserId()) {
											if (_messages.Add(_users.GetUserId(), _users.GetUserLogin(), receiverId, _users.GetUserLogin(receiverId)))
												std::cout << "Сообщение отправлено; id=" << _messages.GetLastMsgId() - 1 << '\n';
										}
										else
											std::cout << "Нельзя отправлять сообщения самому себе.\n";
									}
								}
								break;
							case 1:	// локальная команда read - прочитать сообщения
								_messages.Read(_users.GetUserId(), QuestionYN("Показать только непрочитанные (y) или все сообщения (n)?"));
								break;
							case 2:	// локальная команда info - вывести информацию о пользователе
								_users.Show();
								break;
							case 3:	// локальная команда list - вывести список всех пользователей
								_users.List();
								break;
							case 4:	// локальная команда delete - удалить учётную запись пользователя
								if (QuestionYN("Информация о пользователе будет удалена. Вы уверены?"))
									_users.DeleteUser();
								break;
							case 5: {	// локальная команда change - изменить учётные данные пользователя
									bool loop = true;
									while (loop) {
										std::cout << "Вы находитесь в меню изменения учётных данных пользователя " << _users.GetUserLogin() << '\n';
										print_help(SUBLOCAL_COMMANDS);
										switch (auto cmd_code = get_command(SUBLOCAL_COMMANDS, ", выберите действие или команду >> ", _users.GetUserLogin())) {	// получаем локальную команду с консоли
											case 0:	// локальная команда name - изменить имя пользователя (cmd_code == __name)
											case 1:	// локальная команда log - изменить учётное имя (login) пользователя (cmd_code == __login)
											case 2:	// локальная команда pwd - изменить пароль пользователя (cmd_code == __pwd)
												if (_users.ChangeRegData((Users::RegDataType)cmd_code)) {
													std::cout << "Учётные данные были успешно изменены:\n";
													_users.Show();
												}
												break;
											case 3:	// локальная команда help - вывести справку по командам
												print_help(SUBLOCAL_COMMANDS);
												break;
											case 4:	// локальная команда exit - выйти из этого меню
												loop = false;
												std::cout << "Вы вышли из меню изменения учётных данных.\n";
												break;
											default:
												std::cout << MSG_ERR_CMD;
										}
									}
								}	// конец локального пространства команды change - изменить учётные данные пользователя
								break;
							case 6:	// локальная команда help - вывести справку по командам
								print_help(LOCAL_COMMANDS);
								break;
							case 7:	// локальная команда logout - выход
								_users.Logout();
								break;
							default:
								std::cout << MSG_ERR_CMD;
						}
					}// конец локального пространства команд
				}
				break;
			case 2:	// команда help - вывести справку по командам
				print_help(GLOBAL_COMMANDS);
				break;
			case 3:	// команда quit - закрыть чат
				doing = false;
				if (_users.IsChanged() && !_users.Save())	// если есть изменения, сохраняем данные пользователей во внешний файл
					return 1;
				if (_messages.IsChanged())	// если есть изменения, сохраняем сообщения во внешний файл
					_messages.Save();
				break;
			default:
				std::cout << MSG_ERR_CMD;
		}
	}
	return 0;
}
