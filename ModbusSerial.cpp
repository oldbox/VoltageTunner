#include "stdafx.h"
#include "ModbusSerial.h"

#define MAX_SIZE 255

CModbusSerial::CModbusSerial(CSerial* serial)
	:	m_serial(serial)
{

}


CModbusSerial::~CModbusSerial()
{
}

bool CModbusSerial::Read03Data(int nAddr, int nReg, int nCount, unsigned char *czData0304, int nFC)
{
	if (!m_serial) return false;

	unsigned char pBuf[16];
	unsigned char czData[MAX_SIZE];
	memset(czData, 0, MAX_SIZE);
	memset(pBuf, 0, 16);

	pBuf[0] = nAddr;
	pBuf[1] = nFC;
	pBuf[2] = (nReg - 40001) >> 8;       //¼Ä´æÆ÷µØÖ·¸ß
	pBuf[3] = (nReg - 40001) & 0xff;      //¼Ä´æÆ÷µØÖ·µÍ
	pBuf[4] = nCount >> 8;    //¼Ä´æÆ÷¸öÊý
	pBuf[5] = nCount & 0xff;
	pBuf[6] = LOBYTE(CRC16(pBuf, 6));
	pBuf[7] = HIBYTE(CRC16(pBuf, 6));

	m_serial->Lock();

	bool bRet = false;
	int nFaild = 0;

	int nLen = 0;

	if (8 == m_serial->SendData(pBuf, 8))
	{
		while (1)
		{
			int nRecv = m_serial->ReadData(czData + nLen, 1);
			if (nRecv > 0)
			{
				nLen += nRecv;
				if (czData[0] == nAddr)
				{
					if (nLen >= 2)
					{
						if (czData[1] == nFC)
						{
							if (nLen == 5 + nCount * 2)
							{
								if (CRC16(czData, 5 + nCount * 2) == 0x0000)
								{
									memcpy(czData0304, czData + 3, nCount * 2);
									nLen = 0;
									m_serial->UnLock();
									return true;
								}
								else
								{
									memmove(czData, czData + 1, nLen - 1);
									nLen -= 1;
								}
							}
							else
							{
								continue;
							}
						}
						else
						{
							memmove(czData, czData + 1, nLen - 1);
							nLen -= 1;
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					memset(czData, 0, MAX_SIZE);
					nLen = 0;
				}
			}
			else
			{
				//ÅÐ¶Ï³¬Ê±
				if (nFaild > 3)
				{
					m_serial->UnLock();
					return false;
				}
				nFaild++;
			}
		}
	}

	m_serial->UnLock();

}

bool CModbusSerial::Write10Data(int nAddr, int FisrtRegister, int Regs, unsigned char *czData)
{
	if (!m_serial) return false;

	int Reg = FisrtRegister - 40001;
	unsigned char pBuf[MAX_SIZE];
	unsigned char Data[MAX_SIZE];
	memset(Data, 0, MAX_SIZE);

	memset(pBuf, 0, MAX_SIZE);
	pBuf[0] = nAddr;
	pBuf[1] = 0x10;
	pBuf[2] = Reg >> 8;
	pBuf[3] = Reg & 0xff;
	pBuf[4] = Regs >> 8;
	pBuf[5] = Regs & 0xff;
	pBuf[6] = Regs * 2;
	for (int i = 0; i<Regs * 2; i++)
	{
		pBuf[7 + i] = czData[i];
	}
	unsigned short dCRC = CRC16(pBuf, 7 + Regs * 2);
	pBuf[7 + Regs * 2] = LOBYTE(dCRC);
	pBuf[8 + Regs * 2] = HIBYTE(dCRC);

	m_serial->Lock();

	bool bRet = false;
	int nFaild = 0;
	int nLen = 0;
	//13 10 00 00 00 01 02 01 40  72 90
	if (9 + Regs * 2 == m_serial->SendData(pBuf, 9 + Regs * 2))
	{
		while (1)
		{
			int nRecv = m_serial->ReadData(Data + nLen, 1);
			if (nRecv > 0)
			{
				nLen += nRecv;
				if (Data[0] == nAddr)
				{
					if (nLen >= 2)
					{
						if (Data[1] == 0x10)
						{
							if (nLen == 8)
							{
								if (CRC16(Data, 8) == 0x0000)
								{
									nLen = 0;
									m_serial->UnLock();
									return true;
								}
								else
								{
									memmove(Data, Data + 1, nLen - 1);
									nLen -= 1;
								}
							}
							else
							{
								continue;
							}
						}
						else
						{
							memmove(Data, Data + 1, nLen - 1);
							nLen -= 1;
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					memset(Data, 0, MAX_SIZE);
					nLen = 0;
				}
			}
			else
			{
				//ÅÐ¶Ï³¬Ê±
				if (nFaild > 3)
				{
					m_serial->UnLock();
					return false;
				}
				nFaild++;
			}
		}
	}

	m_serial->UnLock();
}


unsigned short CModbusSerial::CRC16(unsigned char *pBuf, int nLength)
{
	unsigned short wReturn = 0xFFFF;
	for (int nDataLen = 0; nDataLen<nLength; nDataLen++)
	{
		wReturn ^= (unsigned short)((unsigned char)(pBuf[nDataLen]));
		for (int nIndex = 0; nIndex<8; nIndex++)
		{
			if (wReturn & 0x0001)
			{
				wReturn >>= 1;
				wReturn ^= 0xA001;
			}
			else
			{
				wReturn >>= 1;
			}
		}
	}
	return wReturn;
}

unsigned short CModbusSerial::getUInt16(unsigned char *czData, int startIndex)
{
	char pValue[2] = { '\0' };

	pValue[0] = czData[1 + startIndex];
	pValue[1] = czData[0 + startIndex];

	unsigned short value = 0;
	
	memcpy(&value, pValue, 2);

	return value;

}

float CModbusSerial::getFloat(unsigned char *czData, int startIndex)
{
	char pValue[4] = { '\0' };

	pValue[0] = czData[1 + startIndex];
	pValue[1] = czData[0 + startIndex];
	pValue[2] = czData[3 + startIndex];
	pValue[3] = czData[2 + startIndex];

	float value = 0.0;

	memcpy(&value, pValue, 4);

	return value;
}