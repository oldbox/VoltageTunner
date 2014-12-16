#pragma once

#include "Serial.h"
class CModbusSerial
{
public:
	CModbusSerial(CSerial* serial);
	~CModbusSerial();

	bool Read03Data(int nAddr, int nReg, int nCount, unsigned char *czData0304, int nFC = 0x03);
	bool Write10Data(int nAddr, int FisrtRegister, int Regs, unsigned char *czData);

	unsigned short getUInt16(unsigned char *czData, int startIndex);
	float getFloat(unsigned char *czData, int startIndex);
private:
	CSerial* m_serial;

	unsigned short CModbusSerial::CRC16(unsigned char *pBuf, int nLength);
};

