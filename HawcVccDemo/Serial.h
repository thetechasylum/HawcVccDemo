#pragma once

#define SERIAL_ERROR		(-1)

HANDLE SerialOpenPort(std::string portName, uint32_t baudRate);

void SerialClosePort(HANDLE hSerial);

int32_t SerialWrite(HANDLE hSerial, const uint8_t *data, uint32_t dataLen);

int32_t SerialRead(HANDLE hSerial, const uint8_t *data, uint32_t maxDataLen);
