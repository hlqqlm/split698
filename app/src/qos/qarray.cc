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

20220523-----------------------------------------------------------------------
huanglin 创建.
一个变长的数组。注意和qtable相区别
1 每条数据有固定长度
2 当缓冲区不够时，会自动增长
3 增长到capacity_max时，停止增长
4 有效数据，放在array的前面。空余部分，一定在array后面
*/
#include <stdlib.h>

#include "qmem.h"
#include "qstr.h"
#include "qarray.h"
#include "qarray.xcp.h"
//#define this_file_id	0xeae7ec24  // echo -n qarray.c | rhash --simple -

#define TEST_EN		(0)

//{{{ class
cp_t QarrayOpen(Qarray *m, int capacity_step, int capacity_max, int item_size)
{
	m->capacity_step = capacity_step;
	m->capacity_max = capacity_max;
	m->item_size = item_size;
	dve(QarrayValid(m));
	return 0;
}
static cp_t Free(Qarray *m)
{
	dve(QarrayValid(m));
	if (NULL == m->mem)
		return 0;

	m->count = 0;
	m->capacity = 0;
	free(m->mem);
	m->mem = NULL;
	dve(QarrayValid(m));
	return 0;
}
cp_t QarrayClose(Qarray *m)
{
	dve(QarrayValid(m));
	ifer(Free(m));
	dvb(NULL == m->mem);
	return 0;
}
cp_t QarrayValid(const Qarray *m)
{
	ifbr(NULL != m);
#if QARRAY_PRINT_EN	> 0
	ifbr(NULL != m->print);
#endif
	ifbr(0 < m->capacity_step);
	ifbr(0 < m->capacity_max);
	ifbr(0 < m->item_size);

	ifbr(0 <= m->capacity);
	ifbr(m->capacity <= m->capacity_max);
	ifbr(0 <= m->count);
	ifbr(m->count <= m->capacity);

	if (NULL == m->mem)
		ifbr(0 == m->capacity);
	else
		ifbr(0 < m->capacity);
	return 0;
}
//}}}


//{{{ item & ix
// 没有检查的版本
static void *ItemIx(Qarray *m, int ix)
{
	return (char*)m->mem + (ix * m->item_size);
}
static const void *ItemIxConst(const Qarray *m, int ix)
{
	return (char*)m->mem + (ix * m->item_size);
}
// 对外接口，用有检查的版本
void *QarrayItemIx(Qarray *m, int ix)
{
	if (0 != QarrayIxValid(m, ix))
		return NULL;
	return ItemIx(m, ix);
}
const void *QarrayItemIxConst(const Qarray *m, int ix)
{
	if (0 != QarrayIxValid(m, ix))
		return NULL;
	return ItemIxConst(m, ix);
	// return (char *)m->mem + (ix * m->item_size);
}
static void WriteIx(Qarray *m, int ix, const void *item)
{
	dvb(NULL != item);
	void *dst = ItemIx(m, ix);
	memcpy(dst, item, m->item_size);
}
cp_t QarrayIxValid(const Qarray *m, int ix)
{
	dve(QarrayValid(m));
	ifbr(0 <= ix);
	ifbr(ix < m->count);
	return 0;
}
//}}}


//{{{ print
void QarraySetPrint(Qarray *m, QarrayPrintT print)
{
#if QARRAY_PRINT_EN	> 0
	dve(QarrayValid(m));
	dvb(NULL != print);
	m->print = print;
#endif
	return;
}
void QarrayPrintNothing(const struct QarrayS *m, const char *action, int ix)
{
	return;
}
void QarrayPrintNormal(const struct QarrayS *m, const char *action, int ix)
{
	qos_printf("%s <", action);
	QarrayPrint(m);
	qos_printf("> ix=%d", ix);
	if (ix >= 0)
	{
		const char *mem = (const char*)ItemIxConst(m, ix);
		printf_hex_ex(" ", LINEND, mem, m->item_size, "");
	}
	else
		qos_printf(LINEND);
}
void QarrayPrintFull(const struct QarrayS *m, const char *action, int ix)
{
	qos_printf("%s <", action);
	QarrayPrint(m);
	qos_printf("> ix=%d" LINEND, ix);
	QarrayPrintItems(m);
	return;
}
void QarrayPrintItems(const Qarray *m)
{
	const int num = QarrayCount(m);
	for (int i = 0; i < num; ++i)
	{
		const char *mem = (const char *)ItemIxConst(m, i);
		qos_printf("%d\t", i);
		printf_hex_ex("", LINEND, mem, m->item_size, "");
	}
	return;
}
void QarrayPrint(const Qarray *m)
{
	//qos_printf("capacity_step=%d capacity_max=%d item_size=%d capacity=%d count=%d mem=%p"
	//		, m->capacity_step, m->capacity_max, m->item_size, m->capacity, m->count, m->mem);

#if QARRAY_PRINT_EN	> 0
	qos_printf("%d,%d,%d,%d,%d,%p,%p"
			, m->capacity_step, m->capacity_max, m->item_size,  m->capacity, m->count, m->mem, m->print);
#else
	qos_printf("%d,%d,%d,%d,%d,%p"
			, m->capacity_step, m->capacity_max, m->item_size,  m->capacity, m->count, m->mem);
#endif
}
void QarrayPrintIx(const Qarray *m, int ix)
{
	const char *mem = (const char *)ItemIxConst(m, ix);
	printf_hex(mem, m->item_size, "");
}
void QarrayPrintAll(const Qarray *m)
{
	QarrayPrint(m);
	qos_printf(LINEND);
	for (int i = 0; i < m->count; ++i)
	{
		qos_printf("%d\t", i);
		QarrayPrintIx(m, i);
		qos_printf(LINEND);
	}
	return;
}
static void Print(const Qarray *m, const char *action, int ix)
{
#if QARRAY_PRINT_EN	> 0
	m->print(m, action, ix);
#endif
	return;
}
//}}}


//{{{ get
static int IncCapacity(const Qarray *m)
{
	const int nc = m->capacity + m->capacity_step;
	if (nc > m->capacity_max)
		return m->capacity_max;
	return nc;
}
// 有两种full: 一种是达到capacity，一种是达到capacity_max
static bool Full(const Qarray *m)
{
	return (m->capacity <= m->count);
}
bool QarrayCapacityMaxFull(const Qarray *m)
{
	return (m->capacity_max <= m->count);
}
bool QarrayEmpty(const Qarray *m)
{
	return (0 == m->count);
}
int QarrayCount(const Qarray *m) 
{ 
	return m->count; 
}
int QarrayCapacity(const Qarray *m) 
{
	return m->capacity;
}
int QarrayByteNum(const Qarray *m)
{
	return m->count * m->item_size;
}
cp_t QarrayMemCmp(const Qarray *m, const char *mem, int size)
{
	ifbr(size == QarrayByteNum(m));
	ifbr(0 == memcmp(m->mem, mem, size));
	return 0;
}
//}}}


//{{{ array_mem
// 下面负责操作array的内存分配、释放
// 变更array空间，可能是增大，也有可能是缩小
static cp_t QarrayResize(Qarray *m, int new_capacity)
{
	if (m->capacity == new_capacity)	// 新旧capacity不能一样
		return 0;
	ifbr(0 < new_capacity);
	ifbr(m->count <= new_capacity);		// 必须要容纳下已经存储在table里的addr

	// 不能大于上限
	ifbr(new_capacity <= m->capacity_max);

	const int new_size = new_capacity * m->item_size;
    void * const mem = qrealloc(m->mem, new_size);
	// 严重错误，分配内存失败
	if (NULL == mem)
	{
		dvb(NULL != mem);
		qos_printf("ERROR: Qarray realloc FAILED. new_size=%d <", new_size);
		QarrayPrint(m);
		qos_printf(">" LINEND);
		return cph;
	}
	m->mem = mem;
	m->capacity = new_capacity;
	return 0;
}
// 让capacity和count一致，减小内存占用
cp_t QarrayShrink(Qarray *m)
{
	dve(QarrayValid(m));

	if (0 == m->count)
	{
		ifer(Free(m));
		return 0;
	}

	if (Full(m))
		return 0;

	dvb(m->count < m->capacity);
	ifer(QarrayResize(m, m->count));
	return 0;
}
//}}}


//{{{ add & remove
cp_t QarrayAdd(Qarray *m, const void *item)
{
	dve(QarrayValid(m));

	if (Full(m))
	{
		// 增长空间
		const int new_capacity = IncCapacity(m);
		// 空间增长不了
		ifbr(new_capacity > m->capacity);
		ifer(QarrayResize(m, new_capacity));
	}

	dvb(!Full(m));
	// 将item放置在尾部
	WriteIx(m, m->count, item);
	++m->count;
	Print(m, "add", m->count-1);

	dve(QarrayValid(m));
	return 0;
}
// ix: 当ix=count，表示在尾部插入，相当于add
cp_t QarrayInsert(Qarray *m, int ix, const void *item)
{
	dve(QarrayValid(m));
	ifbr(0 <= ix);
	ifbr(ix <= m->count); 

	if (Full(m))
	{
		// 增长空间
		const int new_capacity = IncCapacity(m);
		// 空间增长不了
		ifbr(new_capacity > m->capacity);
		ifer(QarrayResize(m, new_capacity));
	}

	void *src = ItemIx(m, ix);
	void *dst = ItemIx(m, ix + 1);
	const int size = (m->count - ix) * m->item_size;
	// 必须要用memmove，src、dst可能有重叠部分
	memmove(dst, src, size);

	WriteIx(m, ix, item);
	++m->count;
	Print(m, "insert", m->count-1);

	dve(QarrayValid(m));
	return 0;
}
cp_t QarrayRemove(Qarray *m, int ix)
{
	dve(QarrayValid(m));
	
	ifer(QarrayIxValid(m, ix));
	const int move_item_n = m->count - ix - 1;
	// 如果是移除最后一个item，则不用调用memmove()
	if (0 < move_item_n)
	{
		dvb(0 < move_item_n);
		void *item = ItemIx(m, ix);
		void *item_next = ItemIx(m, ix + 1);
		const int move_size = move_item_n * m->item_size;
		Print(m, "remove", ix);
		// 往前挪动一个item的位置
		memmove(item, item_next, move_size);
	}

	--m->count;
	return 0;
}
cp_t QarrayWriteIx(Qarray *m, int ix, const void *item)
{
	dve(QarrayValid(m));
	ifer(QarrayIxValid(m, ix));
	WriteIx(m, ix, item);
	Print(m, "write_ix", ix);
	return 0;
}
cp_t QarrayClear(Qarray *m)
{
	dve(QarrayValid(m));
	// ifer(Free(m));
	m->count = 0;
	Print(m, "clear", -1);
	dve(QarrayValid(m));
	return 0;
}
//}}}


//{{{ find
int QarrayFind(const Qarray *m, const void *item)
{
	dve(QarrayValid(m));	

	for (int i = 0; i < m->count; ++i)
	{
		const void *itemix = ItemIxConst(m, i);
		if (0 == memcmp(itemix, item, m->item_size))
			return i;
	}

	return -1;
}
//}}}


//{{{ table_to_table
// 只比较基本内容是否相同
cp_t QarrayEqual(const Qarray *x, const Qarray *y)
{
	dve(QarrayValid(x));
	dve(QarrayValid(y));

	ifbr(x->item_size == y->item_size);
	ifbr(x->count == y->count);
	if (x->count > 0)
	{
		dvb(y->count > 0);
		ifbr(0 == memcmp(x->mem, y->mem, QarrayByteNum(x)));
	}
	
	return 0;
}
cp_t QarrayDup(Qarray *m, const Qarray *src)
{
    // item_size必须相同
    ifbr(src->item_size == m->item_size);
    // 要能放下src中内容
    ifbr(src->count <= m->capacity_max);
    // 一般m和src是相同类型的qarray，因此，上面两条应能满足

    // 不用处理src->count == 0的情况，如果memcpy(NULL, NULL, 0);合法

    if (m->capacity < src->count)
        ifer(QarrayResize(m, src->count));
    m->count = 0;

    dvb(src->count <= m->capacity);
    dvb(0 == m->count);
    memcpy(m->mem, src->mem, QarrayByteNum(src));
    m->count = src->count;
    dve(QarrayValid(m));
    dve(QarrayEqual(m, src));

    return 0;
}
cp_t QarrayAppend(Qarray *m, const Qarray *src)
{
    // 要逐个从src取出item，然后追加到m中。
    const int count = QarrayCount(src);
    for (int i = 0; i < count; ++i)
    {
        const void * const item = ItemIxConst(src, i);
        ifer(QarrayAdd(m, item));
    }
    
    dve(QarrayValid(m));
    return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestBase(void)
{
	Qarray array = kQarrayDef;
	Qarray *m = &array;
	ifer(QarrayOpen(m, 3, 7, 3));

	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "", 0));
	ifbr(0 == QarrayCount(m));
	ifbr(!QarrayCapacityMaxFull(m));
	ifbr(QarrayEmpty(m));
	ifbr(0 == QarrayCapacity(m));
	ifbr(0 == QarrayByteNum(m));

	ifer(QarrayShrink(m));

	ifbr(0 != QarrayIxValid(m, 0));

	// 111
	ifer(QarrayAdd(m, "111"));
	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "111", 3));
	ifbr(1 == QarrayCount(m));
	ifbr(!QarrayCapacityMaxFull(m));
	ifbr(!QarrayEmpty(m));
	ifbr(3 == QarrayCapacity(m));
	ifbr(3 == QarrayByteNum(m));

	// 111 222
	ifer(QarrayAdd(m, "222"));
	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "111222", 6));
	ifbr(2 == QarrayCount(m));
	ifbr(!QarrayCapacityMaxFull(m));
	ifbr(!QarrayEmpty(m));
	ifbr(3 == QarrayCapacity(m));
	ifbr(6 == QarrayByteNum(m));

	// 111 222 333
	ifer(QarrayAdd(m, "333"));
	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "111222333", 9));
	ifbr(3 == QarrayCount(m));
	ifbr(!QarrayCapacityMaxFull(m));
	ifbr(!QarrayEmpty(m));
	ifbr(3 == QarrayCapacity(m));
	ifbr(9 == QarrayByteNum(m));

	// 111 222 333 444
	ifer(QarrayAdd(m, "444"));
	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "111222333444", 12));
	ifbr(4 == QarrayCount(m));
	ifbr(!QarrayCapacityMaxFull(m));
	ifbr(!QarrayEmpty(m));
	ifbr(6 == QarrayCapacity(m));
	ifbr(12 == QarrayByteNum(m));

	// 111 222 333 444 555 666 
	ifer(QarrayAdd(m, "555"));
	ifer(QarrayAdd(m, "666"));
	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "111222333444555666", 18));
	ifbr(6 == QarrayCount(m));
	ifbr(!QarrayCapacityMaxFull(m));
	ifbr(!QarrayEmpty(m));
	ifbr(6 == QarrayCapacity(m));
	ifbr(18 == QarrayByteNum(m));

	// 111 222 333 444 555 666 777
	ifer(QarrayAdd(m, "777"));
	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "111222333444555666777", 21));
	ifbr(7 == QarrayCount(m));
	ifbr(QarrayCapacityMaxFull(m));
	ifbr(!QarrayEmpty(m));
	ifbr(7 == QarrayCapacity(m));
	ifbr(21 == QarrayByteNum(m));

	// 111 222 333 444 555 666 777
	ifbr(0 != QarrayAdd(m, "888"));
	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "111222333444555666777", 21));
	ifbr(7 == QarrayCount(m));
	ifbr(QarrayCapacityMaxFull(m));
	ifbr(!QarrayEmpty(m));
	ifbr(7 == QarrayCapacity(m));
	ifbr(21 == QarrayByteNum(m));

	ifer(QarrayClear(m));
	ifer(QarrayValid(m));
	ifer(QarrayMemCmp(m, "", 0));
	ifbr(0 == QarrayCount(m));
	ifbr(!QarrayCapacityMaxFull(m));
	ifbr(QarrayEmpty(m));
	ifbr(0 == QarrayByteNum(m));

	ifer(QarrayClose(m));
	return 0;
}
static cp_t TestInsert(void)
{
	Qarray array = kQarrayDef;
	Qarray *m = &array;
	ifer(QarrayOpen(m, 3, 7, 1));

	ifer(QarrayMemCmp(m, "", 0));

	// ix: 当ix=count，表示在尾部插入，相当于add
	ifer(QarrayInsert(m, 0, "1"));
	ifer(QarrayMemCmp(m, "1", 1));

	ifer(QarrayInsert(m, 0, "2"));
	ifer(QarrayMemCmp(m, "21", 2));

	ifer(QarrayInsert(m, 1, "3"));
	ifer(QarrayMemCmp(m, "231", 3));

	ifer(QarrayInsert(m, 1, "4"));
	ifer(QarrayMemCmp(m, "2431", 4));

	ifer(QarrayInsert(m, 0, "5"));
	ifer(QarrayMemCmp(m, "52431", 5));

	ifer(QarrayInsert(m, 0, "6"));
	ifer(QarrayMemCmp(m, "652431", 6));

	ifer(QarrayInsert(m, 6, "7"));
	ifer(QarrayMemCmp(m, "6524317", 7));

	// 满了
	ifbr(0 != QarrayInsert(m, 0, "8"));
	ifer(QarrayMemCmp(m, "6524317", 7));

	ifer(QarrayClose(m));
	return 0;
}
static cp_t TestRemove(void)
{
	Qarray array = kQarrayDef;
	Qarray *m = &array;
	ifer(QarrayOpen(m, 3, 7, 1));

	ifer(QarrayMemCmp(m, "", 0));

	ifbr(0 != QarrayRemove(m, 0));
	ifer(QarrayMemCmp(m, "", 0));

	ifer(QarrayAdd(m, "1"));
	ifer(QarrayAdd(m, "2"));
	ifer(QarrayAdd(m, "3"));
	ifer(QarrayMemCmp(m, "123", 3));

	// remove尾部
	ifer(QarrayRemove(m, 2));
	ifer(QarrayMemCmp(m, "12", 2));

	// remove头部
	ifer(QarrayRemove(m, 0));
	ifer(QarrayMemCmp(m, "2", 1));

	// remove一个
	ifer(QarrayRemove(m, 0));
	ifer(QarrayMemCmp(m, "", 0));

	// 满的情况
	ifer(QarrayAdd(m, "1"));
	ifer(QarrayAdd(m, "2"));
	ifer(QarrayAdd(m, "3"));
	ifer(QarrayAdd(m, "4"));
	ifer(QarrayAdd(m, "5"));
	ifer(QarrayAdd(m, "6"));
	ifer(QarrayAdd(m, "7"));
	ifer(QarrayMemCmp(m, "1234567", 7));

	// 满的情况，remove中间
	ifer(QarrayRemove(m, 3));
	ifer(QarrayMemCmp(m, "123567", 6));

	ifer(QarrayClose(m));
	return 0;
}


static const QtestItem kTestItem[] = {
	TestBase,
	TestInsert,
	TestRemove
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t QarrayTest(QTEST_ARG)
{
	return QtestTest("qarray", run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t QarrayTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

