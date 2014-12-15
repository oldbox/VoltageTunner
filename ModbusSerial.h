#pragma once

#include "Serial.h"
class CModbusSerial
{
public:
	CModbusSerial(CSerial* serial);
	~CModbusSerial();

	bool Read03Data(int nFC, int nAddr, int nReg, int nCount, unsigned char *czData0304);
	bool Write10Data(int nAddr, int FisrtRegister, int Regs, unsigned char *czData);
private:
	CSerial* m_serial;

	unsigned short CModbusSerial::CRC16(unsigned char *pBuf, int nLength);
};

