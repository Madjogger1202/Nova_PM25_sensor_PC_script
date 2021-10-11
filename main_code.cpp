
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
using namespace std;
HANDLE hSerial;

int main()
{
	LPCTSTR sPortName = L"COM3";
	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			cout << "serial port does not exist.\n";
		}
		cout << "some other error occurred.\n";
	}
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		cout << "getting state error\n";
	}
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		cout << "error setting serial port state\n";
	}
	DWORD iSize;
	byte sReceivedChar[10];
	while (true)
	{

		ReadFile(hSerial, &sReceivedChar, 10, NULL, NULL);  // 
		int Low25 = (int)sReceivedChar[2];					//
		int High25 = (int)sReceivedChar[3];
		int Low10 = (int)sReceivedChar[4];
		int High10 = (int)sReceivedChar[5];

		float pm25 = float((High25*256 + Low25)) / 10;
		float pm10 = float((High10*256 + Low10)) / 10;
		cout << "pm2.5 = " << pm25 << "ug/m3" << endl;
		cout << "pm10 = " << pm10 << "ug/m3" << endl;
		
		for(uint16_t i = 0; i<10; i++)
		{
			printf("%.2X", sReceivedChar[i]);
			cout << " ";
		}
		
		int delta = 200000;
		while (delta)
		{
			delta--;
		}
	}
	return 0;
}
