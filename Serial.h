// Serial.h

#ifndef __SERIAL_H__
#define __SERIAL_H__

//#define FC_DTRDSR       0x01
//#define FC_RTSCTS       0x02
//#define FC_XONXOFF      0x04
//#define ASCII_BEL       0x07
//#define ASCII_BS        0x08
//#define ASCII_LF        0x0A
//#define ASCII_CR        0x0D
//#define ASCII_XON       0x11
//#define ASCII_XOFF      0x13

class CSerial
{

public:
	CSerial();
	~CSerial();

	BOOL Open(int nPort, int nBaud, DWORD dwByteSize = 8, DWORD dwParity = NOPARITY, DWORD dwStopBits = ONESTOPBIT);
	BOOL Close( void );

	int ReadData( unsigned char *, int );
	int SendData( const unsigned char *, int );
	int ReadDataWaiting( void );

	void ClearInputBuffer(void);
	void ClearOutputBuffer(void);

	BOOL IsOpened( void ){ return( m_bOpened ); }

	void Lock();
	void UnLock();

	int GetPort(){return m_Port;};
	int GetBaud(){return m_Baud;}
	DWORD GetByteSize(){return m_ByteSize;}
	DWORD GetParity(){return m_Parity;}
	DWORD GetStopBits(){return m_StopBits;}
protected:
	BOOL WriteCommByte( unsigned char );

	HANDLE m_hIDComDev;
	OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
	BOOL m_bOpened;

private:
	CRITICAL_SECTION _Mutex;
	int m_Port;//´®¿ÚºÅ
	int m_Baud;
	DWORD m_ByteSize;
	DWORD m_Parity;
	DWORD m_StopBits;

};

#endif
