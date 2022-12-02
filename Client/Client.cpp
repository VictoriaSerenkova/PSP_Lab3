#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

void main(void) {
	char PCName[30], ServerName[30], Message[200], result[200], IP[16] = { "192.168.56.1" };
	WSAData WSADat; // Свойства WinSock (результат функции WSAStartup)
	sockaddr_in sin; // Свойства(адрес) создаваемого сокета
	SOCKET Sock; // Клиентский сокет

	// Ввод IP-адреса сервера

	WSAStartup(0x0202, &WSADat); // Инициализация WinSock
	// 0x0202 - версия WinSock. Может быть 1.0, 1.1, 2.0, 2.2
	// WSADat - структура, куда будут занесены рез. инициализации
	gethostname(PCName, 30); // Получение имени текущего ПК

	sin.sin_family = AF_INET; // Тип адреса
	sin.sin_addr.s_addr = inet_addr(IP); // IP-адрес сервера (пори создании сервера можно 0)
	sin.sin_port = htons(2803); // Номер порта сервера

	Sock = socket(AF_INET, SOCK_STREAM, 0); // Создание сокета

	// ***** Подключение к серверу
	printf("Connecting to server...\n");

	if (connect(Sock, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		printf("Connection error!\n");
	}
	send(Sock, PCName, strlen(PCName) + 1, 0); // Отправка имени этого компьютера (клиента)
	recv(Sock, ServerName, 30, 0); // Получение имени компьютера сервера

	printf("Connected to \"%s\"!\n", ServerName);

	printf("Enter message: ");
	scanf_s("%199s", Message, 200);
	send(Sock, Message, strlen(Message) + 1, 0);
	printf("Sent!\n");
	recv(Sock, result, 200, 0);
	printf("%s\n", result);
	closesocket(Sock);
	WSACleanup();
	_gettch();
}