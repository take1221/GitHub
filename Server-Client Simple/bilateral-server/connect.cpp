/********************************************************/
/*  Program for Using RS232C from WindowsNT             */
/*  Coded by whashimo                                   */
/*  1995/11/30 Second Version                           */
/*  2004/11/09 タイムアウト処理追加                     */
/********************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE hEvent_1;
HANDLE idcom_1;

HANDLE hEvent_2;
HANDLE idcom_2;


/* Opening Com1 Port from WindowsNT */
int Open_Comm1(void)
{
  BOOL fRetVal;
  DCB dcb;
  COMMTIMEOUTS  Timeout;

  idcom_1 = CreateFile("COM7",GENERIC_READ | GENERIC_WRITE,
                       0,NULL,OPEN_EXISTING,
                       0,NULL);
  //insted of OpenComm()

  /* バッファのクリア */
  PurgeComm(idcom_1, PURGE_TXCLEAR | PURGE_RXCLEAR); 

  GetCommState(idcom_1, &dcb);

  dcb.DCBlength = sizeof(DCB);
  dcb.BaudRate = 115200;
  dcb.ByteSize = 8;
  dcb.Parity   = NOPARITY;
  dcb.StopBits = ONESTOPBIT;
  fRetVal = SetCommState(idcom_1, &dcb);

  /* タイムアウトの設定 */
  Timeout.ReadIntervalTimeout = 0xFFFFFFFF;
  Timeout.ReadTotalTimeoutMultiplier = 0;
  Timeout.ReadTotalTimeoutConstant = 1000;
  Timeout.WriteTotalTimeoutMultiplier = 0;
  Timeout.WriteTotalTimeoutConstant = 1000;
  SetCommTimeouts(idcom_1, &Timeout);

  SetupComm(idcom_1, 128, 128);

  return fRetVal? TRUE:FALSE;
}

/* Opening Com2 Port from WindowsNT */
int Open_Comm2(void)
{
  BOOL fRetVal;
  DCB dcb;
  COMMTIMEOUTS  Timeout;

  idcom_2 = CreateFile("COM2",GENERIC_READ | GENERIC_WRITE,
                       0,NULL,OPEN_EXISTING,
                       0,NULL);
  //insted of OpenComm()

  /* バッファのクリア */
  PurgeComm(idcom_2, PURGE_TXCLEAR | PURGE_RXCLEAR);

  GetCommState(idcom_2, &dcb);

  dcb.DCBlength = sizeof(DCB);
  dcb.BaudRate = 38400;
  dcb.ByteSize = 8;
  dcb.Parity   = NOPARITY;
  dcb.StopBits = ONESTOPBIT;
  fRetVal = SetCommState(idcom_2, &dcb);

  /* タイムアウトの設定 */
  Timeout.ReadIntervalTimeout = 0xFFFFFFFF;
  Timeout.ReadTotalTimeoutMultiplier = 0;
  Timeout.ReadTotalTimeoutConstant = 1000;
  Timeout.WriteTotalTimeoutMultiplier = 0;
  Timeout.WriteTotalTimeoutConstant = 1000;
  SetCommTimeouts(idcom_2, &Timeout);

  SetupComm(idcom_2, 128, 128);

  return fRetVal? TRUE:FALSE;
}


/* Close Com1 Port */
void Close_Comm1(void)
{
  FlushFileBuffers(idcom_1);
  CloseHandle(idcom_1);
}

/* Close Com2 Port */
void Close_Comm2(void)
{
  FlushFileBuffers(idcom_2);
  CloseHandle(idcom_2);
}


/* Read from Com1 Port */
int Read_Comm1(int bufnum, unsigned char *buffer)
{
  DWORD nNumber=0;
  ReadFile(idcom_1, buffer, bufnum, &nNumber, NULL);
  if(bufnum>(int)nNumber)
  	Read_Comm1(bufnum-(int)nNumber, buffer+(int)nNumber);
  return (int)nNumber;
}

/* Read from Com2 Port */
int Read_Comm2(int bufnum, unsigned char *buffer)
{
  DWORD nNumber=0;
  ReadFile(idcom_2, buffer, bufnum, &nNumber, NULL);
  if(bufnum>(int)nNumber)
    Read_Comm2(bufnum-(int)nNumber, buffer+(int)nNumber);
  return (int)nNumber;
}


/* Write to Com1 Port */
void Write_Comm1(int bufnum, unsigned char *buffer)
{
  DWORD nNumber;
  WriteFile(idcom_1, buffer, bufnum, &nNumber, NULL);
}

/* Write to Com2 Port */
void Write_Comm2(int bufnum, unsigned char *buffer)
{
  DWORD nNumber;
  WriteFile(idcom_2, buffer, bufnum, &nNumber, NULL);
}


void Clear_Comm1(void)
{
  PurgeComm(idcom_1, PURGE_TXABORT | PURGE_RXABORT |
            PURGE_TXCLEAR | PURGE_RXCLEAR);
}

void Clear_Comm2(void)
{
  PurgeComm(idcom_2, PURGE_TXABORT | PURGE_RXABORT |
            PURGE_TXCLEAR | PURGE_RXCLEAR);
}

void Write_force_Comm1(unsigned char buf)
{
  TransmitCommChar(idcom_1, buf);
}

void Write_force_Comm2(unsigned char buf)
{
  TransmitCommChar(idcom_2, buf);
}
