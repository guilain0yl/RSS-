#include"RSS.h"
#include<tchar.h>

#pragma comment(lib,"ws2_32.lib")

#define SERVERPORT 80

#define SENDBUFFSIZE 2*1024

#define u64 unsigned long long

#define RECVBUFF 5*M_

typedef struct URLNODEINFO
{
	char Host[50];//host http://host
	char Dir[500];//dir http://host dir
}UrlNodeInfo;

static UrlNodeInfo Web_Info;

BOOL GetHostName(char *Target, char **IpList);

//辅助函数
BOOL html_end(char *html);
char* DislodgeHttp(char *Buff);

//web函数
SOCKET ConnectTarget();
BOOL GetHostName(char *Target, char **IpList);
BOOL SendResquest(SOCKET ClientSock);
int ReceiveResponse(SOCKET ClientSock);