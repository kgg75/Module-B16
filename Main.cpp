#include <iostream>
#include <string>
#include <windows.h>
#include "Users.h"
#include "Messages.h"
#include "ServiceFunc.h"
#include "Chat.h"

//using namespace std;

int main() {
	system("chcp 1251");
	Users users;
	if (!users.Open() && QuestionYN("Завершить программу?"))	// загружаем данные пользователей из внешнего файла
		return 1;	// аварийный выход по желанию пользователя
	Messages messages;
	messages.Open();	// загружаем сообщения из внешнего файла
	int result = Chat(users, messages);	// запуск работы чата; можно получить результат (зарезервировано)
	return result;
}