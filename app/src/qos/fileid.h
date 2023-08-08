#ifndef __FILEID_H_
#define __FILEID_H_

#include <stdbool.h>
#include <stdint.h>

//#include "fileid_register.h"
#include "qcp.h"


typedef struct {
	uint32_t fileid;
	const char *name;
} FileidItem;
#define FILEID_ITEM_V { 0, "unknow" }

#define FILEID_STR_SIZE	(9)		// 包括结尾"\0"
#define FILE_NAME_STR_SIZE	(30)	// 包括结尾"\0"
#define FILEID_ITEM_STR_SIZE (FILEID_STR_SIZE + 1 + FILE_NAME_STR_SIZE)	// 包括结尾"\0"	
char *FileidStr(char str[FILEID_STR_SIZE], uint32_t fileid);
char *FileidItemStr(char str[FILEID_ITEM_STR_SIZE], const FileidItem *item);


// class
typedef struct {
	const FileidItem *table;
	int num;
} FileidTable;

// return: 0 ok
//int FileidTableOpen(FileidTable *m, const FileidItem table[], int num);
//int FileidTableClose(FileidTable *m);

// 表格必须是排序的，且不能有相同fileid出现，本函数返回不合格的index
// return: 0，表格是排序的 
// 非0，未排序的下标，例如：第0条和第1条，两个fileid相同，返回1
int FileidTableUnsortIx(const FileidItem table[], int num);
// return: 0 valid 
int FileidTableValid(const FileidItem table[], int num);


// get / set
//int FileidTableLen(const FileidItem table, int num);
// 如果找到fileid，则返回文件名；如果没有找到,返回 NONAME；即一定会返回可打印名字
const char *FileidTableFilenameById(const FileidItem table[], int num, uint32_t fileid);
// 查找table，返回文件名；如果没有找到，返回NULL
const char *FileidTableLookup(const FileidItem table[], int num, uint32_t fileid);
void FileidTablePrint(const FileidItem table[], int num);


#endif
