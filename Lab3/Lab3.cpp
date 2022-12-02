#include <windows.h>
#include <stdio.h>
#include <iostream>

HANDLE hEvent;

int Teilor(double x);

DWORD WINAPI ThreadFunc(LPVOID context)
{
	hEvent = (HANDLE)context;
	printf("Hello world from worker thread\n");
	SetEvent(hEvent);
	return 0;
}

#pragma argsused
int main(int argc, char* argv[])
{
	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	QueueUserWorkItem(ThreadFunc, (PVOID)hEvent, WT_EXECUTEDEFAULT);
	WaitForSingleObject(hEvent, 1000);
	CloseHandle(hEvent);
	return 0;
}

