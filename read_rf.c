//此文件读取rf格式的文件
//
#include"RSS.h"

char* rf_buff = NULL;
char*rf_file = NULL;
char *host_url = NULL;
char *rss_name = NULL;

void del_note()
{
	//此函数为消除注释行
}

int read_rf(char *filepath)//读取rss.rf文件//默认rss.rf在程序目录下，可以修改源代码
{
	char *tmp;
	if (access(filepath, 0))
		return FALSE;
	FILE *fp = NULL;
	int file_size = 0;

	//打开文件
	fp = fopen(filepath, "r");
	if (fp == NULL)
	{
		if (access(filepath, 4))
			return FALSE;
		return FALSE;
	}

	//获取文件大小
	rewind(fp);
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	//分配文件内存
	rf_buff = (char*)malloc(file_size + 1);
	memset(rf_buff, 0x0, file_size + 1);

	//读取文件
	while(fread(rf_buff, file_size, 1, fp))
		return FALSE;

	//添加断位符//检测到次符号代表rf文件解析结束
	rf_buff[strlen(rf_buff)] = 0x7f;

	//解析RSS文件名
	tmp = strchr(rf_buff, ':');
	rss_name = (char*)calloc(1, tmp - rf_buff + 1);
	strncpy(rss_name, rf_buff, tmp - rf_buff);
	tmp++;

	//解析出获取信息源地址
	tmp = strchr(tmp, '\n');
	host_url = (char*)calloc(1, tmp - rf_buff + 1);
	strncpy(host_url, rf_buff, tmp - rf_buff);
	rf_file = ++tmp;

	fclose(fp);

	return TRUE;
}