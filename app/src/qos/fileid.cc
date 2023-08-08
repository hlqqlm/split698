/*
This program is licensed under the GPLv2 License:
Copyright 2023 Huanglin <hlqqlm@qq.com>


This file is part of Split698.

Split698 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

Split698 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Split698. If not, see <https://www.gnu.org/licenses/>.


20190116-----------------------------------------------------------------------
创建.
*/
#include <stdio.h>

#include "app.h"
#include "fileid.h"
#include "fileid.xcp.h"

//#define this_file_id 0x573F9049


//{{{ item
/*
const FileidItem kFileidTable[] = {
#include "id_filename_table_uniq.h"
};
const unsigned int kFileidTableLen = sizeof(kFileidTable) / sizeof(kFileidTable[0]);
*/
char *FileidStr(char str[FILEID_STR_SIZE], uint32_t fileid)
{
	snprintf(str, FILEID_STR_SIZE, "%08x", fileid);
    return str;
}
char *FileidItemStr(char str[FILEID_ITEM_STR_SIZE], const FileidItem *item)
{
	char id_str[FILEID_STR_SIZE];
	snprintf(str, FILEID_ITEM_STR_SIZE, "%s %-30s", FileidStr(id_str, item->fileid), item->name);
	return str;
}


//}}}


//{{{ class
/*
int FileidTableOpen(FileidTable *m, const FileidItem table[], int num)
{
	m->table = table;
	m->num = num;

	const int v = FileidTableValid(m);
	if (0 != v)
	{
		printf("WARNNING: fileid_table has error, error at line %d\n", v);
		printf("file_id_table is:");
		FileidTablePrint(table, num);
	}

	return 0;
}
int FileidTableClose(FileidTable *m)
{
	return 0;
}
*/
int FileidTableUnsortIx(const FileidItem table[], int num)
{
	dvb(0 < num);

	const FileidItem *curr = table;
	for (int i = 0; i < num - 1; ++i, ++curr)
	{
		const FileidItem *next = curr + 1;
		// 不能有fileid相等的两行，即两个文件的fileid相同，如果出现，变更一个文件的名字即可
		if (curr->fileid == next->fileid)
			return i + 1;
		// 必须是排序的
		if (!(curr->fileid < next->fileid))
			return i + 1;
	}
	return 0;
}
int FileidTableValid(const FileidItem table[], int num)
{
	if (num <= 0)
		return __LINE__;
	if (0 != FileidTableUnsortIx(table, num))
		return __LINE__;
	return 0;
}
//}}}


//{{{ get / set
//int FileidTableLen(const FileidTable *m) 
//{ 
//	return m->num; 
//}
const char *FileidTableFilenameById(const FileidItem table[], int num, uint32_t fileid)
{
    const char *find = FileidTableLookup(table, num, fileid);
	if (NULL != find)
		return find;
    return "NONAME";
}
//}}}


//{{{ lookup
const char *FileidTableLookup(const FileidItem table[], int num, uint32_t fileid)
{
	const FileidItem *curr = table;
	for (int i = 0; i < num; ++i, ++curr)
	{
		if (curr->fileid == fileid)
			return curr->name;
	}

	return NULL;
}
void FileidTablePrint(const FileidItem table[], int num)
{
	const FileidItem *curr = table;
	for (int i = 0; i < num; ++i, ++curr)
	{
		char item_str[FILEID_ITEM_STR_SIZE]; 
		printf("%-5d %s\n", i + 1, FileidItemStr(item_str, curr));
	}
}
//}}}



