#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

HANDLE hEvent;
double number;
double answer;
string s;
double Teilor(double x);

DWORD WINAPI ThreadFunc(LPVOID context)
{
	hEvent = (HANDLE)context;
	answer = Teilor(number);
	std::cout << "ln|"<< number <<"| = " << answer << std::endl;
	SetEvent(hEvent);
	return 0;
}

void main(void) {
	char PCName[30], ClientName[30], Message[200];
	WSAData WSADat; // Свойства WinSock (результат функции WSAStartup)
	sockaddr_in sin; // Свойства(адрес) создаваемого сокета
	SOCKET Sock, Client; // Серверный и клиентский сокеты

	WSAStartup(0x0202, &WSADat); // Инициализация WinSock
	// 0x0202 - версия WinSock. Может быть 1.0, 1.1, 2.0, 2.2
	// WSADat - структура, куда будут занесены рез. инициализации
	gethostname(PCName, 30); // Получение имени текущего ПК

	sin.sin_family = AF_INET; // Тип адреса
	sin.sin_addr.s_addr = 0;  // IP-адрес сервера (пори создании сервера можно 0)
	sin.sin_port = htons(2803); // Номер порта сервера
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Создание сокета
	bind(Sock, (sockaddr*)&sin, sizeof(sin)); // Связывание созданного сокета с адресом sin

	// ***** Ожиданеи клиента
	printf("Wait of client...\n");

	listen(Sock, SOMAXCONN); // Прослушивание сокета сервером (для подключения клиента)
	Client = accept(Sock, (sockaddr*)&sin, 0); // Ожидание клиента
	recv(Client, ClientName, 30, 0); // Получение имени компьютера клиента
	send(Client, PCName, strlen(PCName) + 1, 0); // Отправка имени этого компьютера (сервера)

	printf("Client ( \"%s\" ) has connected!\n", ClientName);

	// ***** Меню

			// Приём сообщения от клиента
			if (recv(Client, Message, 200, 0) != SOCKET_ERROR) {
				number = atof(Message);
				hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				QueueUserWorkItem(ThreadFunc, (PVOID)hEvent, WT_EXECUTEDEFAULT);
				WaitForSingleObject(hEvent, 1000);
				CloseHandle(hEvent);
				s = to_string(answer);
				char res[200];
				strcpy_s(res, s.c_str());
				if (send(Client, res, strlen(res) + 1, 0) != SOCKET_ERROR) printf("Sent!\n");
				//if (send(Client, "fdvd", strlen("fdvd") + 1, 0) != SOCKET_ERROR) printf("Sent!\n");
			}
			else printf("Error of getting!\n");

	// Закрытие сокетов и окончание работы с WinSock
	closesocket(Sock);
	closesocket(Client);
	WSACleanup();
	_gettch();
}

double Teilor(double x) {
	double e = 0.001;
	if (x == 0.0)
	{
		std::cout << "Not defined !" << std::endl;
		return 0;
	}
	if (x < 0.0)
		x = -x;
	double t = x - 1;
	double u = t;
	double lnabsx = u;
	int n = 1;
	do
	{
		n++;
		u *= -((n - 1) * t) / n;
		lnabsx += u;
	} while (u > e || u < -e);
	return lnabsx;
}