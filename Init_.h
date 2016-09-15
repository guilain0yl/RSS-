//定义全局变量

#include"RSS.h"

label_info info;
char* RSS_MEM = NULL;//RSS内存块
char *end_label_[100] = { 0 };//闭合标签堆栈
int label_end_count = 0;//栈内存活的数量