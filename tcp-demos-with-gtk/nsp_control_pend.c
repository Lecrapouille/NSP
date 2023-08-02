#include <scicos/scicos_block4.h>
                 
#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>

#include <scicos/scicos_block4.h>

#ifdef _MSC_VER
#include <WinSock.h>
#pragma comment(lib, "Ws2_32.lib")
#define bzero(s, n) memset((s), 0, (n))
#define show_error(n,str) return
#else
#include <sys/socket.h>
#include <unistd.h> /* close */
#include <netdb.h>
#define show_error(n,str) perror(str);return
#endif

#define SA struct sockaddr
                 
void CBlockFunction(scicos_block *block,int flag)
{
  SCSREAL_COP *u1=GetRealInPortPtrs(block,1);
  int mu1=GetInPortSize(block,1,1);
  int nu1=GetInPortSize(block,1,2);
  SCSREAL_COP *y1=GetRealOutPortPtrs(block,1);
  int my1=GetOutPortSize(block,1,1);
  int ny1=GetOutPortSize(block,1,2);
  SCSINT32_COP *iPar=Getint32OparPtrs(block,1);
  char *pstHostname="localhost";
  SCSINT32_COP *sock=Getint32OzPtrs(block,1);
  struct sockaddr_in server;
  struct hostent *hp;
                  
  switch (flag){
  case 4:
                 
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    //if (*sock < 0) set_block_error(-2);return;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    hp = gethostbyname(pstHostname);
    memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length);
    server.sin_port = htons(*iPar);
    while ( 1 )
      {
	if (connect(*sock,(SA*)&server, sizeof(server)) == -1 )
	  { perror("connect");return;}
	else
	  return;
      }
    break;
  case 1:
    if ( send(*sock, u1, mu1*nu1*sizeof(double),0) == -1)
      { show_error(-2,"send");}
    break;
  case 2:
    if(recv(*sock, y1, ny1*my1*sizeof(double),0)<=0)
      { show_error(-2,"revc");}
    break;
  case 5:
    //if(*sock>0) close(*sock);
    break;
  }
}
