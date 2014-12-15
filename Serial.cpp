// Serial.cpp

#include "stdafx.h"
#include "Serial.h"

CSerial::CSerial()
{ 
	//m_Port = 0; 
	//m_Baud = 9600; 
	//m_ByteSize = 8;
	//m_Parity = NOPARITY;
	//m_StopBits = ONE5STOPBITS;
	::InitializeCriticalSection(&_Mutex);

	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
	m_hIDComDev = NULL;
	m_bOpened = FALSE;

}

CSerial::~CSerial()
{

	Close();
	::DeleteCriticalSection(&_Mutex);

}


BOOL CSerial::Open( int nPort, int nBaud ,DWORD dwByteSize, DWORD dwParity, DWORD dwStopBits)
{

	m_Port = nPort;
	m_Baud = nBaud;
	m_ByteSize = dwByteSize;
	m_Parity = dwParity;
	m_StopBits = dwStopBits;

	if( m_bOpened ) return( TRUE );

	char szPort[16];
	memset(szPort, 0, 16);
	DCB dcb;
	sprintf_s(szPort, "COM%d", nPort);

	m_hIDComDev = CreateFile((LPCWSTR)szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	if( m_hIDComDev == NULL ) return( FALSE );

	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( m_hIDComDev, &CommTimeOuts );


	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	dcb.DCBlength = sizeof( DCB );
	GetCommState( m_hIDComDev, &dcb );
	dcb.BaudRate = nBaud;
	dcb.Parity = dwParity;
	dcb.ByteSize = dwByteSize;
	dcb.StopBits = dwStopBits;
	if( !SetCommState( m_hIDComDev, &dcb ) ||
		!SetupComm( m_hIDComDev, 10000, 10000 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL ){
			DWORD dwError = GetLastError();
			if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
			if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
			CloseHandle( m_hIDComDev );
			return( FALSE );
		}

		m_bOpened = TRUE;

		return( m_bOpened );

}

BOOL CSerial::Close( void )
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( TRUE );

	if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
	CloseHandle( m_hIDComDev );
	m_bOpened = FALSE;
	m_hIDComDev = NULL;

	return( TRUE );

}

BOOL CSerial::WriteCommByte( unsigned char ucByte )
{
	BOOL bWriteStat;
	DWORD dwBytesWritten;

	bWriteStat = WriteFile( m_hIDComDev, (LPSTR) &ucByte, 1, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) ){
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) dwBytesWritten = 0;
		else{
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
		}
	}

	return( TRUE );

}

int CSerial::SendData(const unsigned char *buffer, int size)
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	EscapeCommFunction(m_hIDComDev, CLRRTS);

	DWORD dwBytesWritten = 0;
	int i;
	for( i=0; i<size; i++ ){
		WriteCommByte( buffer[i] );
		dwBytesWritten++;
	}

	EscapeCommFunction(m_hIDComDev, SETRTS);

	return( (int) dwBytesWritten );

}

int CSerial::ReadDataWaiting( void )
{

	//ÿ���ͷ�CPU�Ŀ���Ȩ����������
	::Sleep(1);

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );

	return( (int) ComStat.cbInQue );

}

int CSerial::ReadData( unsigned char *buffer, int limit )
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	dwBytesRead = (DWORD) ComStat.cbInQue;
	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;

	bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
	if( !bReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return( (int) dwBytesRead );
		}
		return( 0 );
	}

	return( (int) dwBytesRead );

}

void CSerial::Lock()
{
	::EnterCriticalSection(&_Mutex);
}
void CSerial::UnLock()
{
	::LeaveCriticalSection(&_Mutex);
}


void CSerial::ClearInputBuffer()
{
	if(IsOpened())
		::PurgeComm(m_hIDComDev, PURGE_RXABORT | PURGE_RXCLEAR );
}
///������ͻ�����
void CSerial::ClearOutputBuffer()
{
	if(IsOpened())
		::PurgeComm(m_hIDComDev, PURGE_TXABORT | PURGE_TXCLEAR );
}
