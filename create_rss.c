/*
by:guilain
RSS这个小项目结束
代码没有怎么整理，以后有空再说
调用 只需要添加其他程序中create_rss.h文件就可以调用了//这相当于一个对外的接口
由于没有整理，某些地方显得很乱
rf文件的格式
目标rss的名字:目标站的地址www.xxx.com
""-""需要查找“”-“”之间的内容
具体查看rss.rf格式
由于v.0.1版本 很多功能没有实现，所以rss.rf必须严格按照格式进行编写
*/

#include"RSS.h"


char* target_=NULL;

int write_to_file()
{
	FILE *fp = NULL;
	char *rss_name_buff = (char*)calloc(1, strlen(rss_name) + 5);
	strcpy(rss_name_buff, rss_name);
	strcat(rss_name_buff, ".rss");
	fp = fopen(rss_name_buff, "w");
	if (fp == NULL)
		return FALSE;
	if (!fwrite(RSS_MEM, strlen(RSS_MEM), 1, fp))
		return FALSE;
	fflush(fp);
	fclose(fp);

}

int create_rss(char *rf_path)
{
	if (!read_rf(rf_path))
		return READ_ERR;
	if (!get_web_page())//用于初始化target_变量，获得RSS的消息来源
		return WEB_ERR;
	if (strlen(target_) <= 0)
		return -1;
	analysis_rf();
	write_to_file();
}