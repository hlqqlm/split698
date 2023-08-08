#ifndef  __QMEMTABLE_H__
#define  __QMEMTABLE_H__

#include "qcp.h"
#include "qtest.h"
#include "qarray.h"
#include "qmem.h"

#define QMEMTABLE_EN			(0)
#define QMEMTABLE_BARRIER_EN	(0)		// 检查越界写
										
#if QMEMTABLE_BARRIER_EN > 0
#define kMemtableHeadSize	(8)
#define kMemtableTailSize	(8)
#else
#define kMemtableHeadSize	(0)
#define kMemtableTailSize	(0)
#endif
#define kMemtableBarrierCh	(0xe5)

#if QMEMTABLE_EN > 0
#define qmalloc(_size)			QmemtableRegisterGlobal(_size, cph)
#define qfree(_ptr)				QmemtableRemoveGlobal(_ptr, cph)
#define qrealloc(_ptr, _size)	QmemRealloc(_ptr, _size)			// 程序不记录realloc分配的内存

#else
#define qmalloc(_size)			QmemMalloc(_size)
#define qfree(_ptr)				QmemFree(_ptr)
#define qrealloc(_ptr, _size)	QmemRealloc(_ptr, _size)
#endif

 
struct QmemtableItemS;
struct QmemtableS;
typedef void (*QmemtablePrintT)(const struct QmemtableS *m, const char *action, int ix, const struct QmemtableItemS *item);
void QmemtablePrintNothing(const struct QmemtableS *m, const char *action, int ix, const struct QmemtableItemS *item);
void QmemtablePrintNormal(const struct QmemtableS *m, const char *action, int ix, const struct QmemtableItemS *item);

typedef struct QmemtableItemS
{
	// malloc
	void *ptr;
	size_t size;
	size_t real_size;
	cp_t cp_malloc;
	int malloc_ix;

	// free
	cp_t cp_free;		// todo: 没有用到，可删除
	int free_ix;		// todo: 没有用到，可删除
} QmemtableItem;
#define kQmemtableItemDef { NULL, 0, 0, 0, 0, 0, 0}
void QmemtableItemPrint(const QmemtableItem *item);


typedef struct QmemtableS
{
	Qarray array;
	int head_size;			// 在分配的内存前面、后面分别留一些空间。为了防止对齐问题，最好是8的倍数
	int tail_size;

	QmemtablePrintT print;
} Qmemtable;
#define kQmemtableDef { kQarrayDef, 0, 0, QmemtablePrintNothing }

cp_t QmemtableOpen(Qmemtable *m, int head_size, int tail_size);
cp_t QmemtableClose(Qmemtable *m);
cp_t QmemtableValid(const Qmemtable *m);

// print
QmemtablePrintT QmemtableSetPrint(Qmemtable *m, QmemtablePrintT print);
void QmemtablePrintAll(const Qmemtable *m);

// action
void *QmemtableRegister(Qmemtable *m, size_t size, cp_t cp_malloc);
cp_t QmemtableRemove(Qmemtable *m, void *ptr, cp_t cp_free);
// 如果有越界写，返回索引号；否则，返回-1
int QmemtableCheckBarrier(const Qmemtable *m);

// global
Qmemtable *QmemtableGlobal(void);
cp_t QmemtableOpenGlobal(int head_size, int tail_size);
cp_t QmemtableCloseGlobal(void);
void QmemtablePrintAllGlobal(void);
#if QMEMTABLE_EN > 0
void *QmemtableRegisterGlobal(size_t size, cp_t cp_malloc);
cp_t QmemtableRemoveGlobal(void *ptr, cp_t cp_free);
#endif

// test
cp_t QmemtableTest(QTEST_ARG);
#endif

