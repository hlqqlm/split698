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

20230518-----------------------------------------------------------------------
huanglin 创建.
内存记录表
*/
#include "qtest.h"
#include "qmemtable.h"
#include "qmemtable.xcp.h"


//#define this_file_id	0xf505b6ca // echo -n qmemtable.c | rhash --simple -


#define TEST_EN				(0)
#define PRINT_IN_TEST_EN	(1)
#define kCapacityStep		(3)
#define kCapacityMax		(1000)

#if QMEMTABLE_EN > 0
//{{{ item
void QmemtableItemPrint(const QmemtableItem *item)
{
	char cp_buf_malloc[CPSTR_SIZE];
	char cp_buf_free[CPSTR_SIZE];
	CpStr(cp_buf_malloc, item->cp_malloc);
	CpStr(cp_buf_free, item->cp_free);

	qos_printf("%p,%u,%u,%s,%d,%s,%d", item->ptr, item->size, item->real_size, cp_buf_malloc, item->malloc_ix, cp_buf_free, item->free_ix);
}
//}}}


//{{{ class
cp_t QmemtableOpen(Qmemtable *m, int head_size, int tail_size)
{
	m->head_size = head_size;
	m->tail_size = tail_size;
	ifer(QarrayOpen(&m->array, kCapacityStep, kCapacityMax, sizeof(QmemtableItem)));
	
	return 0;
}
cp_t QmemtableClose(Qmemtable *m)
{
	ifer(QarrayClose(&m->array));
	return 0;
}
cp_t QmemtableValid(const Qmemtable *m)
{
	ifer(QarrayValid(&m->array));
	return 0;
}
//}}}


//{{{ print
void QmemtablePrintNothing(const struct QmemtableS *m, const char *action, int ix, const struct QmemtableItemS *item)
{
	return;
}
void QmemtablePrintNormal(const struct QmemtableS *m, const char *action, int ix, const struct QmemtableItemS *item)
{
	qos_printf("Memtable: %s,%d,<", action, ix);
	QmemtableItemPrint(item);
	qos_printf(">,%d" LINEND, QarrayCount(&m->array));
	return;
}
static void Print(const Qmemtable *m, const char *action, int ix, const QmemtableItem *item)
{
	m->print(m, action, ix, item);
}
QmemtablePrintT QmemtableSetPrint(Qmemtable *m, QmemtablePrintT print)
{
	QmemtablePrintT old = m->print;
	m->print = print;
	return old;
}
void QmemtablePrintAll(const Qmemtable *m)
{
	const int num = QarrayCount(&m->array);
	qos_printf("memtable total %d, malloc_cnt=%d:" LINEND, num, MallocCnt());
	for (int i = 0; i < num; ++i)
	{
		const QmemtableItem *item = QarrayItemIxConst(&m->array, i);
		qos_printf("%d\t", i);
		QmemtableItemPrint(item);
		qos_printf(LINEND);
	}
}
//}}}


//{{{ array
static int FindPtr(const Qmemtable *m, const void *ptr)
{
	const int num = QarrayCount(&m->array);
	for (int i = 0; i < num; ++i)
	{
		const QmemtableItem *item = QarrayItemIxConst(&m->array, i);
		if (item->ptr == ptr)
			return i;
	}
	return -1;
}
//}}}


//{{{ action
static size_t RealSize(const Qmemtable *m, size_t size)
{
	return size + m->head_size + m->tail_size;
}
static size_t TailOffset(const Qmemtable *m, size_t size)
{
	return size + m->head_size;
}
void *QmemtableRegister(Qmemtable *m, size_t size, cp_t cp_malloc)
{
	QmemtableItem item = kQmemtableItemDef;
	item.real_size = RealSize(m, size);
	void *malloc_ptr = QmemMalloc(item.real_size);

	// 前后barrier填写上初值
	memset(malloc_ptr, kMemtableBarrierCh, m->head_size);
	const size_t tail_offset = TailOffset(m, size);
	memset((char*)malloc_ptr + tail_offset, kMemtableBarrierCh, m->tail_size);

	item.ptr = (char*)malloc_ptr + m->head_size;
	item.size = size;
	item.cp_malloc = cp_malloc;
	item.malloc_ix = MallocCnt();
	const cp_t cp = QarrayAdd(&m->array, &item);
	if (0 != cp)
	{
		char cp_buf[CPSTR_SIZE];
		CpStr(cp_buf, cp);
		qos_printf("ERROR: QmemtableRegister failed. at %s" LINEND, cp_buf);
		return item.ptr;
	}

	const int ix = QarrayCount(&m->array) - 1;
	const QmemtableItem *item_ix = QarrayItemIxConst(&m->array, ix);
	Print(m, "malloc", ix, item_ix);
	return item.ptr;
}
cp_t QmemtableRemove(Qmemtable *m, void *ptr, cp_t cp_free)
{
	const int ix = FindPtr(m, ptr);
	ifbr(0 <= ix);

	const QmemtableItem *item_ix = QarrayItemIxConst(&m->array, ix);
	Print(m, "free", ix, item_ix);

	ifer(QarrayRemove(&m->array, ix));

	void *free_ptr = (char*)ptr - m->head_size;
	QmemFree(free_ptr);
	ifer(QarrayShrink(&m->array));
	return 0;
}
// 如果有越界写，返回索引号
int QmemtableCheckBarrier(const Qmemtable *m)
{
	const int num = QarrayCount(&m->array);
	for (int i = 0; i < num; ++i)
	{
		const QmemtableItem *item = QarrayItemIxConst(&m->array, i);
		const char *head = (char*)item->ptr - m->head_size;
		if (0 != MemIsSame(kMemtableBarrierCh, head, m->head_size))
		{
			Print(m, "barrier_head_WRONG", i, item);
			return i;
		}
		const char *tail = (char*)item->ptr + item->size;
		if (0 != MemIsSame(kMemtableBarrierCh, tail, m->tail_size))
		{
			Print(m, "barrier_tail_WRONG", i, item);
			return i;
		}
	}

	return -1;
}
//}}}
#else

cp_t QmemtableOpen(Qmemtable *m, int head_size, int tail_size) { return 0; }
cp_t QmemtableClose(Qmemtable *m) { return 0; }
void QmemtablePrintNothing(const struct QmemtableS *m, const char *action, int ix, const struct QmemtableItemS *item)
{
	return;
}
void QmemtablePrintNormal(const struct QmemtableS *m, const char *action, int ix, const struct QmemtableItemS *item)
{
	return;
}
QmemtablePrintT QmemtableSetPrint(Qmemtable *m, QmemtablePrintT print) { return NULL; }
void QmemtablePrintAll(const Qmemtable *m) { return; }
int QmemtableCheckBarrier(const Qmemtable *m) { return -1; }

#endif

//{{{ global
static Qmemtable m_memtable = kQmemtableDef;
Qmemtable *QmemtableGlobal(void)
{
	return &m_memtable;
}
#if QMEMTABLE_EN > 0
void *QmemtableRegisterGlobal(size_t size, cp_t cp_malloc)
{
	return QmemtableRegister(&m_memtable, size, cp_malloc);
}
cp_t QmemtableRemoveGlobal(void *ptr, cp_t cp_free)
{
	return QmemtableRemove(&m_memtable, ptr, cp_free);
}
#endif
cp_t QmemtableOpenGlobal(int head_size, int tail_size)
{
	return QmemtableOpen(&m_memtable, head_size, tail_size);
}
cp_t QmemtableCloseGlobal()
{
	return QmemtableClose(&m_memtable);
}
void QmemtablePrintAllGlobal(void)
{
	QmemtablePrintAll(&m_memtable);
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestNormal(void)
{
	ifbr(0 < QMEMTABLE_EN);

	Qmemtable mt = kQmemtableDef;
	Qmemtable *m = &mt;
	ifer(QmemtableOpen(m, 0, 0));
	// QmemtableSetPrint(m, QmemtablePrintNormal);

	//QmemtablePrintAll(m);

	ifbr(0 == MallocCnt());
	void *ptr1 = QmemtableRegister(m, 1, cph);
	ifbr(1 == MallocCnt());
	//QmemtablePrintAll(m);
	ifer(QmemtableRemove(m, ptr1, cph));
	ifbr(0 == MallocCnt());

	void *ptr2 = QmemtableRegister(m, 2, cph);
	ifbr(1 == MallocCnt());
	void *ptr3 = QmemtableRegister(m, 3, cph);
	ifbr(2 == MallocCnt());
	//QmemtablePrintAll(m);
	ifer(QmemtableRemove(m, ptr2, cph));
	ifbr(1 == MallocCnt());
	//QmemtablePrintAll(m);
	ifer(QmemtableRemove(m, ptr3, cph));
	ifbr(0 == MallocCnt());

	ifer(QmemtableClose(m));
	return 0;
}
static cp_t TestHeadTail(void)
{
	ifbr(0 < QMEMTABLE_EN);

	Qmemtable mt = kQmemtableDef;
	Qmemtable *m = &mt;
	ifer(QmemtableOpen(m, 8, 16));
	//QmemtableSetPrint(m, QmemtablePrintNormal);

	void *ptr1 = QmemtableRegister(m, 1, cph);
	//QmemtablePrintAll(m);
	ifer(QmemtableRemove(m, ptr1, cph));

	ifer(QmemtableClose(m));
	return 0;
}
static cp_t TestGlobal(void)
{
	Qmemtable *m = &m_memtable;
	QmemtablePrintT old_print = QmemtableSetPrint(m, QmemtablePrintNormal);

	const int mc = MallocCnt();
	void *ptr1 = qmalloc(1);
	ifbr(mc + 1 == MallocCnt());
	QmemtablePrintAll(m);
	qfree(ptr1);
	ifbr(mc == MallocCnt());

	QmemtableSetPrint(m, old_print);
	return 0;
}

static const QtestItem kTestItem[] = {
	TestNormal,
	TestHeadTail,
	TestGlobal,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t QmemtableTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t QmemtableTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

