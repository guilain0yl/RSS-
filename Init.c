//此文件用来对全局变量进行初始化
//
//
//
//
#include"RSS.h"
#include"Init_.h"

void Init_()
{
	info.label = (char*)malloc(LABEL_SIZE);
	memset(info.label, 0x0, LABEL_SIZE);
	info.get_start = (char*)malloc(INFO_SIZE);
	memset(info.get_start, 0x0, INFO_SIZE);
	info.get_end = (char*)malloc(INFO_SIZE);
	memset(info.get_end, 0x0, INFO_SIZE);
	info.need_find = FALSE;
	info.need_ana = TRUE;
	info.need_push = FALSE;
	//info.table_count=0;
	//为生成的RSS文件分配内存，并写入XML信息
	RSS_MEM = (char*)malloc(RSS_SIZE);
	memset(RSS_MEM, 0x0, RSS_SIZE);
	strcat(RSS_MEM, XMLHeader);
}