//全局变量的定义
//各种宏定义
#pragma once

#include<Windows.h>
#include<stdio.h>
#include<io.h>

#define LABEL_SIZE 20
#define INFO_SIZE 512
#define M_ 1024*1024
#define RSS_SIZE 1*M_

#define READ_ERR 102
#define CREATE_ERR 103
#define WEB_ERR 201

#define is_letter(x) ((x>64&&x<91)||(x>96&&x<123))


typedef struct LABEL_INFO
{
	char *label; //
	char *get_start, *get_end;//label内容的开始和结尾
	BOOL need_find;//是否需要查找内容
	BOOL need_ana;//: 是否需要被解析
	BOOL need_push;//是否需要被推入
	//int table_count;
}label_info, *plabel_info;


extern label_info info;

extern char *rf_buff;
extern char *rf_file;
extern char *RSS_MEM;
extern char *target_;//需进行初始化，例如网页文件

extern char *end_label_[100];
extern int label_end_count;
extern char* host_url;
extern char* rss_name;

static const char *XMLHeader = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
static const char *RSSVersion_1 = "<rss version=\"1.0\">", *RSSVersion_2 = "<rss version=\"2.0\">";


//Init
void Init_();

//File
int read_rf(char *);

//get_web_page
BOOL get_web_page();

//RSS
void analysis_rf();
int write_rss();

//Func
int get_table_count();
void pop_end_table();
void zero_struct();
void push_end_label(char *label_buff);
void end_lable(char *label_end_buff);
char* Get_String(char *symbol_start, char *symbol_end);
void write_version(int i = 2);
void add_bracket(char *label_buff);
void clean_up();