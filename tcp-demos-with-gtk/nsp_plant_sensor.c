#include <stdio.h>
#include <strings.h>
#include <sys/types.h>

#include <scicos/scicos_block4.h>

#ifdef _MSC_VER
#include <WinSock.h>
#pragma comment(lib, "Ws2_32.lib")
#define bzero(s, n) memset((s), 0, (n))
#define show_error(n,str) return n
#else
#include <sys/socket.h>
#include <unistd.h> /* close */
#define show_error(n,str) perror(str);return n
#endif

static int start_tcp_server(int _iPort)
{
  struct sockaddr_in server, client_addr;
  struct sockaddr_in from;
  static int sock = 0;
  int bytes_recieved , true = 1 ;
  int iFromLength = 0;
  int iServerLength = 0;
  int connected;
  struct timeval tv;
  int iTimeout = 2000;
  
  sock = socket(AF_INET, SOCK_STREAM , 0);
  if (sock < 0)  return -1;
  
  tv.tv_sec = 10;
  tv.tv_usec = 0;
#ifndef _MSC_VER
  if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,(char *)&tv,sizeof(tv)) < 0) {show_error(-3,"setsockopt");};
#else
  if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,(const char *)&iTimeout,sizeof(iTimeout))<0)
    {show_error(-3,"setsockopt");}
#endif
  bzero(&server, sizeof(server)); 
  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(_iPort);
  iServerLength = sizeof(server);
  if (bind(sock, (struct sockaddr *)&server, iServerLength) < 0) {show_error(-2,"bind");}
  if (listen(sock, 5) != 0) {show_error(-2,"listen");}
  iServerLength = sizeof(client_addr);
  connected = accept(sock, (struct sockaddr *)&client_addr,&iServerLength);
  if (connected<0) {show_error(-5,"accept");}
#ifndef _MSC_VER
  // close(sock); deja ? 
#else
  closesocket(sock);
#endif
  return connected;
}

void toto(scicos_block *block,int flag)
{
  SCSREAL_COP *u1=GetRealInPortPtrs(block,1);
  int mu1=GetInPortSize(block,1,1);
  int nu1=GetInPortSize(block,1,2);
  SCSREAL_COP *y1=GetRealOutPortPtrs(block,1);
  int my1=GetOutPortSize(block,1,1);
  int ny1=GetOutPortSize(block,1,2);
  SCSINT32_COP *Port=Getint32OparPtrs(block,1);
  SCSINT32_COP *connected=Getint32OzPtrs(block,1);
  SCSREAL_COP *evout=GetNevOutPtrs(block);
  int ierr;
  switch (flag){
  case 4:
    // Initialize Winsock 
    *connected = start_tcp_server(*Port);
    if (*connected<0) {set_block_error(-2);return;}
    break;
  case 5:
#ifndef _MSC_VER
    close(*connected);
#else
    closesocket(*connected);
#endif
    break;
  case 1:
  if(recv(*connected, y1, ny1*my1*sizeof(double),0)<=0) {set_block_error(-2);return;}
    break;
  case 2:
    if(send(*connected, u1, nu1*mu1*sizeof(double),0)<=0) {set_block_error(-2);return;}
    break;
  case 3:
    *evout=*y1-GetScicosTime(block);
    break;
  }
}

