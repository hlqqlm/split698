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

20190418-----------------------------------------------------------------------
创建.
*/
// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <inttypes.h>
//#include <time.h>
//#include <sys/time.h>
//#include <math.h>
//#include <sstream>

//#include "rtconfig.h"
//#include "rtthread.h"

#include "qmem.h"
#include "qmem.xcp.h"
//#define this_file_id	0x8355aab7		// echo -n qmem.c | rhash --simple -



cp_t MemIsSame(char ch, const void *mem, int size)
{
	dvb(NULL != mem);
	dvb(0 < size);

	const char *p = (const char *)mem;
	for (int i = 0; i < size; i++, p++) 
		ifbr(*p == ch);

	return 0;

}


// 查找字符
// return: index if find. -1 if NOT find.
int MemFindCh(char ch, const void *mem, int size)
{
	dvb(NULL != mem);
	dvb(0 <= size);
	const char *p = (const char *)mem;
	int i;
	for (i = 0; i < size; i++, p++)
	{
		if (*p == ch)
		{
			return i;
		}
	}
	return -1;
}



// find needle in haystack
const char *MemInMem(const char *haystack, int haystack_size, const char *needle, int needle_size)
{
	dvb(NULL != haystack);
	dvb(0 < haystack_size);
	dvb(NULL != needle);
	dvb(0 < needle_size);
	if (haystack_size < needle_size)
	{
		return NULL;
	}
	// left is already find space.
	// right is NOT find space.
	int right_size = haystack_size;
	const char *p = haystack;
	while (right_size >= needle_size)
	{
		if (0 == memcmp(p, needle, needle_size))
		{
			return p;
		}
		p++;
		right_size--;
	}
	return NULL;
}



// "\x12\x34\x56\x78\x90"       ->      "\x90\x78\x56\x34\x12"
void MemReverse(void *mem, int size)
{
        int i = 0;
        const int half = size / 2;
        char *left = (char *)mem;
        char *right = (char*)mem + size - 1;
        for (i = 0; i < half; i++, left++, right--) 
        {
                const char c = *left;
                *left = *right;
                *right = c;
        }
        return;
}
void MemcpyReverse(void *dst, const void *src, int size)
{
	const char *srcp = (char*)src;
	char *dstp = (char*)dst;
	
	for (int i = 0; i < size; ++i)
	{
		const int ix = size - i - 1;
		dstp[ix] = srcp[i];
	}
}



// this function will modify the str, and return pointer is the first char which
// is NOT blank.
// return: trimed string head pointer. NEVER return NULL.
char *StrTrim(char *str)
{
        dvb(NULL != str);

        char *head = str;
        while (isspace(*head))
        {
                head++;
        }

        StrTrimRight(head);
        return head;
}
char *StrTrimRight(char *str)
{
        dvb(NULL != str);

        int len = strlen(str);
        if (0 >= len)
        {
                return str;
        }

        char *p = str + len - 1;
        while (isspace(*p) && (p > str)) 
        {
                p--;
        }
        dvb((p >= str) && (p <= str + len - 1));
        if (isspace(*p)) 
        {
                dvb(p == str);
                *p = '\0';
        }
        else 
        {
                p++;
                *p = '\0';
        }
        return str;
}


static int m_malloc_cnt = 0;
static int m_free_cnt = 0;
//void *qmalloc(size_t size)
void *QmemMalloc(size_t size)
{
	// 用malloc()，当空间用到一定时候，返回NULL。用rt_malloc()正常
	void *p = malloc(size);
	if (NULL != p)
		++m_malloc_cnt;
	return p;
}
//void qfree(void *ptr)
void QmemFree(void *ptr)
{
	--m_malloc_cnt;
	++m_free_cnt;
	free(ptr);
}
void *QmemRealloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}
int MallocCnt(void)
{
	return m_malloc_cnt;
}
int FreeCnt(void)
{
	return m_free_cnt;
}

