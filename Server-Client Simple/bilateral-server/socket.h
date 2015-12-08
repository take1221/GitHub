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

/* OpenClient(int *netd, int port, char address[])      */
/* netd     : descriptor of socket                      */
/* port     : network port                              */
/* address[]: network IP address, likes "130.158.90.72" */
/* return value: SUCCESS 1 FAIL 0                       */
int OpenClient(int *, int, char *);

/* CloseClient(int netd)                                */
/* netd : descriptor of socket                          */
void CloseClient(int);

/* InitSocket(void)                                     */
/* 最初に一度だけ読みこむ                               */
void InitSocket(void);

/* OpenServer(int *netd, int port)                      */
/* netd     : descriptor of socket                      */
/* port     : network port                              */
/* return value: SUCCESS 1 FAIL 0                       */
int OpenServer(int *, int);

/* WaitServer(int netd, int *fd)                        */
/* netd     : descriptor of socket                      */
/* fd       : descriptor of socket                      */
void WaitServer(int, int *);

/* CloseServer(int netd)                                */
/* netd : descriptor of socket                          */
void CloseServer(int);

/* ReadSocket(int netd, int num, unsigned char buf[]    */
/* netd    : descriptor of socket                       */
/* num     : buffer size that you want to receive       */
/* buffer[]: read buffer                                */
void ReadSocket(int, int, unsigned char *);

/* WriteSocket(int netd, int num, unsigned char buf[]   */
/* netd    : descriptor of socket                       */
/* num     : buffer size that you want to send          */
/* buffer[]: write buffer                               */
void WriteSocket(int, int, unsigned char *);

/* ReadySocket(int netd)                                */
/* netd    : descriptor of socket                       */
/* return 1(=TRUE) if read buffer is ready              */
int ReadySocket(int);

/* QuitServer(void)                                     */
/* サーバを完全に終了するときに使う                     */
void QuitServer(void);
