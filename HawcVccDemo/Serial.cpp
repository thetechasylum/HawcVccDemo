
#include <windows.h>

#include <iostream>
#include <stdlib.h>

#include "Serial.h"

HANDLE SerialOpenPort(std::string portName, uint32_t baudRate)
{
	HANDLE hSerial;

	std::string portPath = "\\\\.\\" + portName;
	std::wstring wPortName = std::wstring(portPath.begin(), portPath.end());
	hSerial = CreateFile(wPortName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);
	if (hSerial != INVALID_HANDLE_VALUE)
	{
		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
		if (!GetCommState(hSerial, &dcbSerialParams))
		{
			std::cout << "serial port, unable to read current port settings." << std::endl << std::endl;
		}
		dcbSerialParams.BaudRate = baudRate;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.StopBits = ONESTOPBIT;
		dcbSerialParams.Parity = NOPARITY;
		if (!SetCommState(hSerial, &dcbSerialParams))
		{
			std::cout << "serial port, unable to set new port settings." << std::endl << std::endl;
		}

		COMMTIMEOUTS timeouts = { 0 };
		timeouts.ReadIntervalTimeout = 20;
		timeouts.ReadTotalTimeoutConstant = 500;
		timeouts.ReadTotalTimeoutMultiplier = 0;
		timeouts.WriteTotalTimeoutConstant = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;
		if (!SetCommTimeouts(hSerial, &timeouts))
		{
			std::cout << "serial port, unable to set port timeouts." << std::endl << std::endl;
		}
	}
	else
	{
		uint32_t err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND)
		{
			std::cout << "serial port does not exist." << std::endl << std::endl;
		}
		else
		{
			std::cout << "Unknown error: " << err << "\n";
		}
	}
	return (hSerial);
}

void SerialClosePort(HANDLE hSerial)
{
	CloseHandle(hSerial);
}

int32_t SerialWrite(HANDLE hSerial, const uint8_t *data, uint32_t dataLen)
{
	int32_t dataWritten;
	DWORD dwBytesWritten = 0;
	bool wrtieRslt;

	wrtieRslt = WriteFile(hSerial, data, dataLen, &dwBytesWritten, NULL);
	dataWritten = (wrtieRslt) ? dwBytesWritten : SERIAL_ERROR;
	return (dataWritten);
}

int32_t SerialRead(HANDLE hSerial, const uint8_t *data, uint32_t maxDataLen)
{
	int32_t dataRead;
	DWORD dwBytesRead = 0;
	bool readRslt;

	readRslt = ReadFile(hSerial, (char *)data, maxDataLen, &dwBytesRead, NULL);
	dataRead = (readRslt) ? dwBytesRead : SERIAL_ERROR;
	return (dataRead);
}
