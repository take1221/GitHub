/********************************************************/
/*      socket.c                                        */
/*      Socket Module for Win32                         */
/*                           First Version 1999/01/04   */
/*                           Polling       1999/01/20   */
/*                           Divide sock#  2001/05/30   */
/*      coded by whashimo                               */
/*      ビルドには ws2_32.lib をリンクする必要あり      */
/*                           Warning off   2002/08/26   */
/********************************************************/
#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <time.h>

struct timeval tv;              /* タイムアウト設定時間 */

/********************************************************
  serverサンプル
{
  int netd, fd;
  InitSocket();
  OpenServer(&netd, 10001);
  WaitServer(netd, &fd);
  ReadSocket(fd,..);
  WriteSocket(fd,..);
  CloseServer(fd);
  QuitServer();
}
  clientサンプル
{
  int netd;
  InitSocket();
  OpenClient(&netd, 10001, "xx.xx.xx.xx");
  WriteSocket(netd,..);
  ReadSocket(netd,..);
  CloseClient(netd);
}
********************************************************/


/* 最初に一度だけ読みこむ */
void InitSocket(void)
{
  WSADATA WSAData;

  /* initiates use of the Windows Sockets DLL by a process. */
  if(WSAStartup(MAKEWORD(2,0), &WSAData)){
    perror("WSAStart()");
    exit(1);
  }
  tv.tv_sec=0;                    /* sec  */ 
  tv.tv_usec=0;                   /* msec */
}

int OpenServer(int *netd, int port)
{
  struct sockaddr_in addr;
  
  if((*netd=socket(PF_INET, SOCK_STREAM, 0))<0){
    perror("socket()");
    return(0);
  }
  
  memset(&addr, 0, sizeof(addr));
  
  addr.sin_family=PF_INET;
  addr.sin_port=htons((unsigned short)port);
  addr.sin_addr.s_addr=INADDR_ANY;

  if(bind(*netd, (struct sockaddr *)&addr, sizeof(addr))<0){
    perror("bind()");
    return(0);
  }

  if(listen(*netd, 1)<0){
    perror("listen()");
    return(0);
  }
  return(1);
}

void WaitServer(int netd, int *fd)
{
  int len;
  long host;
  struct sockaddr_in from;
  printf("connection waiting...\n");
  
  len=sizeof(from);
  if((*fd=accept(netd, (struct sockaddr *)&from, &len))<0){
    perror("accept");
    exit(1);
  }
  host=ntohl(from.sin_addr.s_addr);
  printf("connection established (%d.%d.%d.%d)\n",
         (host>>24)&0xff,
         (host>>16)&0xff,
         (host>>8)&0xff,
         host&0xff);
}

int OpenClient(int *netd, int port, char *c_addr)
{
  struct sockaddr_in addr;
  
  if((*netd=socket(PF_INET, SOCK_STREAM, 0))<0){
    perror("socket()");
    return(0);
  }
  
  memset(&addr, 0, sizeof(addr));
  
  addr.sin_family=PF_INET;
  addr.sin_port=htons((unsigned short)port);
  addr.sin_addr.s_addr=inet_addr(c_addr);

  if(connect(*netd, (struct sockaddr *)&addr, sizeof(addr))<0){
    perror("connect()");
    return(0);
  }
  tv.tv_sec=0;                    /* sec  */ 
  tv.tv_usec=0;                   /* msec */
  return(1);
}


/* return 1(=TRUE) if read buffer is ready */
int ReadySocket(int netd)
{
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(netd, &readfds); 
  return(select(0, &readfds, NULL, NULL, &tv));
}

void ReadSocket(int netd, int num, unsigned char *buffer)
{
  recv(netd, (char*)buffer, num, 0);
}

void WriteSocket(int netd, int num, unsigned char *buffer)
{
  send(netd, (const char*)buffer, num, 0);
}

void CloseServer(int netd)
{
  closesocket(netd);
}

void CloseClient(int netd)
{
  closesocket(netd);
  WSACleanup();
}

void QuitServer(void)
{
  WSACleanup();
}