//提供运行时需要的各种功能

#include"RSS.h"


int get_table_count()
{
	int i = 0, count = 0;
	while (info.label[i++] == '\t')
		count++;
	return count;
}

void pop_end_table()
{
	strcat(RSS_MEM, end_label_[--label_end_count]);
	free(end_label_[label_end_count]);
}

void zero_struct()//防止数据存活
{
	memset(info.label, 0x0, LABEL_SIZE);
	memset(info.get_start, 0x0, INFO_SIZE);
	memset(info.get_end, 0x0, INFO_SIZE);
	info.need_find = FALSE;
	info.need_ana = TRUE;
	info.need_push = FALSE;
}

void push_end_label(char *label_buff)//将结束标签推入数组
{
	end_label_[label_end_count] = (char*)malloc(strlen(label_buff) + 1);
	strcpy(end_label_[label_end_count], label_buff);
	label_end_count++;
}

void end_lable(char *label_end_buff)
{
	int i = 0;

	while (info.label[i] != '<')
		i++;
	label_end_buff[i + 1] = '/';
	strcpy(&label_end_buff[i + 2], &info.label[i + 1]);
	//去除/n
	i = 0;
	if (!info.need_push)//如果是需要压栈的标签则不需要去除
	{
		memset(info.label, 0x0, strlen(info.label));
		while (label_end_buff[i++] != '<');
		strcpy(info.label, &label_end_buff[i-1]);
		memset(label_end_buff, 0x0, strlen(label_end_buff));
		strcpy(label_end_buff, info.label);
	}
}


char* Get_String(char *symbol_start, char *symbol_end)
{
	char *start, *end, *msg;

	start = strstr(target_, symbol_start);
	start += strlen(symbol_start);
	end = strstr(start, symbol_end);

	msg = (char*)malloc(end - start + 1);
	memset(msg, 0x0, end - start + 1);

	strncpy(msg, start, end - start);
	return msg;
}

void write_version(int i)
{
	if (i == 1)
		strcat(RSS_MEM, RSSVersion_1);
	if (i == 2)
		strcat(RSS_MEM, RSSVersion_2);
}

void add_bracket(char *label_buff)
{
	int i = 0;
	strcpy(label_buff, info.label);
	while (!is_letter(info.label[i]))
		i++;
	label_buff[i] = '<';
	strcpy(&label_buff[i + 1], &info.label[i]);
	strcat(label_buff, ">");
	strcpy(info.label, label_buff);
	memset(label_buff, 0x0, strlen(label_buff));
	label_buff[0] = '\n';
	strcat(label_buff, info.label);
	strcpy(info.label, label_buff);
}


void clean_up()
{
	free(info.label);
	free(info.get_start);
	free(info.get_end);
	free(rf_buff);
	free(RSS_MEM);
	free(target_);
}