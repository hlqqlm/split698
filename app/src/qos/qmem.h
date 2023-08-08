#ifndef _QMEM_H_
#define _QMEM_H_


#include <stddef.h>     /* offsetof */

//#include "qdebug.h"
#include "qcp.h"
#include "qmemtable.h"

// 下面内容，来自include/linux/container_of.h
#define typeof_member(T, m)	typeof(((T*)0)->m)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 * WARNING: any const qualifier of @ptr is lost.
 */
#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	static_assert(__same_type(*(ptr), ((type *)0)->member) ||	\
		      __same_type(*(ptr), void),			\
		      "pointer type mismatch in container_of()");	\
	((type *)(__mptr - offsetof(type, member))); })



// ref: https://stackoverflow.com/questions/2129521/why-doesnt-sizeof-parse-struct-members
#define MEMBER_SIZEOF(T,F) sizeof(((T *)0)->F)




//ref:  ~/cm/app/exe/qos_ut/utest/qstr_ut.c
//      ~/cm/app/lib/qos/qstr.c

cp_t MemIsSame(char ch, const void *mem, int size);
// 查找字符
// return: index if find. -1 if NOT find.
int MemFindCh(char ch, const void *mem, int size);
// find needle in haystack
const char *MemInMem(const char *haystack, int haystack_size, const char *needle, int needle_size);
// 翻转
void MemReverse(void *mem, int size);
void MemcpyReverse(void *dst, const void *src, int size);
// this function will modify the str, and return pointer is the first char which
// is NOT blank.
// return: trimed string head pointer. NEVER return NULL.
char *StrTrim(char *str);
char *StrTrimRight(char *str);

/*
// 结构体信息
typedef struct StructItemS
{
	size_t offset;
	size_t size;
	const char *name;
	struct StructItemS *sub;
	int sub_len;
} StructItemT;
cp_t StructItemEqual(const StructItemT &x, const StructItemT &y);
bool operator==(const StructItemT &x, const StructItemT &y);
#define STRUCT_ITEM_SUB(_s, _item, _sub, _sub_len)	{ offsetof(_s, _item), sizeof(_s::_item), #_item, _sub, _sub_len }
#define STRUCT_ITEM(_s, _item)						STRUCT_ITEM_SUB(_s, _item, NULL, 0)	
*/


//void *qmalloc(size_t size);
//void qfree(void *ptr);
void *QmemMalloc(size_t size);
void QmemFree(void *ptr);
void *QmemRealloc(void *ptr, size_t size);
int MallocCnt(void);
#endif

