/*

进行分析和创建RSS文件

*/



#include"RSS.h"


void analysis_rf()
{
	char *point = rf_file, *tmp;
	int i = 0, j = 0;
	int table_count = 0, pre_table_count = 0;

	Init_();

	tmp = info.label;

	while (point[i] != 0x0)
	{
		if (point[i] == ':')
		{
			if (!info.need_ana)
				goto out;
			tmp = info.get_start;
			i++;
			if (point[i] == '\"')
				i++;
			j = 0;
			info.need_ana = FALSE;
		}
out:
		if (point[i] == '\"'&&point[i + 1] == '-'&&point[i + 2] == '\"')
		{
			tmp = info.get_end;
			i += 3;
			j = 0;
			info.need_find = TRUE;
		}
		if (point[i] == '\n' || point[i] == 0x7f)
		{
			if (tmp[--j] == '\"')
				tmp[j] = '\0';
			tmp = info.label;
			i++;
			j = 0;

			//判断是否是channel,image,item
			if (strstr(info.label, "channel"))
				info.need_push = TRUE;
			if (strstr(info.label, "image"))
				info.need_push = TRUE;
			if (strstr(info.label, "item"))
				info.need_push = TRUE;

			pre_table_count = table_count;
			table_count = get_table_count();

			if (table_count<pre_table_count)
				pop_end_table();

			write_rss();
			zero_struct();//替换成zero_struct
		}
		tmp[j] = point[i];
		i++; j++;
	}

	while (label_end_count != 0)
		pop_end_table();

	strcat(RSS_MEM, "\n</rss>");//结束rss
}

int write_rss()
{
	//准备存放标签的内存空间
	char *label_buff = (char*)malloc(strlen(info.label) + 5);
	memset(label_buff, 0x0, strlen(info.label) + 5);

	char *text = NULL;

	//如果是RSS写入版本信息
	if (!stricmp(info.label, "rss"))
	{
		if (!stricmp(info.get_start, "1.0"))
			write_version(1);
		if (!stricmp(info.get_start, "2.0"))
			write_version(2);
		else write_version();//如果版本都不匹配 默认使用2.0版本
		return 0;
	}

	//写入标签
	add_bracket(label_buff);
	strcat(RSS_MEM, label_buff);

	//判断是否需要查找标签内的内容
	if (strlen(info.get_start) != 0)
	{
		if (info.need_find)
		{
			text = Get_String(info.get_start, info.get_end);
			strcat(RSS_MEM, text);
		}
		else strcat(RSS_MEM, info.get_start);
	}

	end_lable(label_buff);

	if (info.need_push)
	{
		push_end_label(label_buff);
		goto end;
	}
	strcat(RSS_MEM, label_buff);
end:
	free(label_buff);
	free(text);
	return 0;
}