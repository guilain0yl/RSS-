//获取web页面

//获取web页面

#include"get_web_page.h"

char* get_link(char *key_start,char* key_end)
{
	char *start, *end, *link;
	start = strstr(rf_buff, key_start);
	start += strlen(key_start);
	end = strstr(start, key_end);
	link = (char*)malloc(end - start + 2);
	memset(link, 0x0, end - start + 2);
	strncpy(link, start, end - start);
	return link;
}

void Init_Node(char *link)
{
	const char *domain_name[] = { ".com", ".cn", ".tv", ".cc", ".net", ".org", ".top", ".info" };
	char *host_,*dir;
	int i = 0;

	memset(Web_Info.Host, 0x0, 50);
	host_ = strchr(host_url, ':');
	host_++;
	strcpy(Web_Info.Host, host_);

	dir = strstr(link, "http://");
	if (dir == NULL)
		strcat(Web_Info.Dir, link);
	else{
		dir += strlen("http://");
		strcat(Web_Info.Dir, dir);
	}
}

BOOL get_web_page()
{
	SOCKET socket;
	char *link = get_link("link:\"","\"");
	Init_Node(link);
	free(link);

	socket = ConnectTarget();
	if (socket < 0)
		return FALSE;
	if (!SendResquest(socket))
		return FALSE;
	if (ReceiveResponse(socket))
		return FALSE;
	return TRUE;
}

SOCKET ConnectTarget()
{
	struct sockaddr_in ServerAddr;
	char *IpList[10] = { NULL };
	SOCKET ClientSock;

	if (GetHostName(Web_Info.Host, IpList))
	{
		ClientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (ClientSock == INVALID_SOCKET)
		{
			//MessageBox(NULL, _T("ClientSocket Error"), _T("Web Spider"), 0);
			return -1;
		}
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(SERVERPORT);
		ServerAddr.sin_addr.S_un.S_addr = inet_addr(IpList[0]);
		memset(ServerAddr.sin_zero, 0x00, 8);
		if ((connect(ClientSock, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr))) == SOCKET_ERROR)
		{
			MessageBox(NULL, _T("Connect Error"), _T("RSS"), 0);
			return -1;
		}
		return ClientSock;
	}
	else return -1;
}

BOOL GetHostName(char *Target, char **IpList)//
{
	hostent *pHost;
	WSADATA ws;
	int i = 0;

	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
		MessageBox(NULL, _T("WSAStartup Error"), _T("Web Spider"), 0);
	pHost = gethostbyname(Target);
	if (pHost != NULL)
	{
		while (*pHost->h_addr_list != NULL)
		{
			IpList[i] = inet_ntoa(*(struct in_addr *)*pHost->h_addr_list);
			*pHost->h_addr_list++;
			i++;
		}
		return TRUE;
	}
	else return FALSE;
}

int ReceiveResponse(SOCKET ClientSock)
{
	u64 Ret = 0, RecvPos = 0;
	int try_count = 0;
	char *RecvBuff = (char*)malloc(RECVBUFF);
	char *TextPos = NULL;
	memset(RecvBuff, 0x0, RECVBUFF);
	Sleep(1000);
	do{
		Ret = recv(ClientSock, &RecvBuff[RecvPos], RECVBUFF, 0);
		if (Ret == 0 || Ret == SOCKET_ERROR || Ret < 0)
		{
			Sleep(1000);
			if (try_count == 3)
			{
				free(RecvBuff);
				//printf("Client Quit!\n");
				return -1;
			}
		}
		RecvPos += Ret;
	} while (!html_end(RecvBuff));

	TextPos = DislodgeHttp(RecvBuff);


	target_ = (char*)malloc(strlen(TextPos));
	memset(target_, 0x0, strlen(TextPos));
	strcpy(target_, TextPos);

	free(RecvBuff);
	return 0;
}

BOOL SendResquest(SOCKET ClientSock)
{
	char *RequestBuff = (char*)malloc(SENDBUFFSIZE);
	memset(RequestBuff, 0x0, SENDBUFFSIZE);
	sprintf(RequestBuff, "GET http://%s HTTP/1.1\r\nHost:%s\r\nConnection: keep-alive\r\nAccept: */*\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.155 Safari/537.36\r\nAccept-Language: zh-CN,zh;q=0.8\r\n\r\n", Web_Info.Dir);
	if (send(ClientSock, RequestBuff, (int)strlen(RequestBuff), 0) == SOCKET_ERROR)
	{
		free(RequestBuff);
		MessageBox(NULL, _T("SendRequest Error"), _T("Web Spider"), 0);
		return FALSE;
	}
	else{
		free(RequestBuff);
		return TRUE;
	}
}


//判断是否结束
BOOL html_end(char *html)
{
	char *end_symbol = NULL;
	if (strstr(html, "</html>"))
		return TRUE;
	return FALSE;
}

//获取消息正文起始位置
char* DislodgeHttp(char *Buff)
{
	char *TextPos = NULL;
	if (TextPos = strstr(Buff, "<html>"))
		return TextPos;
	return NULL;
}
