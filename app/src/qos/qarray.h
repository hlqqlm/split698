#ifndef __QARRAY_H_
#define __QARRAY_H_

#include <string.h>
#include "qcp.h"
#include "qtest.h"

// 禁止掉打印功能，可以节约内存
#define QARRAY_PRINT_EN		(1)


struct QarrayS;
typedef void (*QarrayPrintT)(const struct QarrayS *m, const char *action, int ix);

typedef struct QarrayS
{
// public:
	int capacity_step;
	int capacity_max;
	int item_size;

// private:
	int capacity;
	int count;
	void *mem;			// 表格
						//
#if QARRAY_PRINT_EN	> 0
	QarrayPrintT print;
#endif
} Qarray;
#if QARRAY_PRINT_EN	> 0
#define kQarrayDef {							\
	0, 0, 0,									\
	0, 0, NULL,									\
	QarrayPrintNothing,							\
}
#else
#define kQarrayDef {							\
	0, 0, 0,									\
	0, 0, NULL,									\
}
#endif


// class
cp_t QarrayOpen(Qarray *m, int capacity_step, int capacity_max, int item_size);
cp_t QarrayClose(Qarray *m);
cp_t QarrayValid(const Qarray *m);

// print
void QarrayPrintNothing(const struct QarrayS *m, const char *action, int ix);
// 只打印当前ix，如add一条，只打印add的一条记录
void QarrayPrintNormal(const struct QarrayS *m, const char *action, int ix);
// 始终打印所有记录
void QarrayPrintFull(const struct QarrayS *m, const char *action, int ix);
void QarraySetPrint(Qarray *m, QarrayPrintT print);
void QarrayPrint(const Qarray *m);
void QarrayPrintIx(const Qarray *m, int ix);
void QarrayPrintItems(const Qarray *m);

// get
int QarrayCount(const Qarray *m);
//bool QarrayFull(const Qarray *m);
bool QarrayCapacityMaxFull(const Qarray *m);
bool QarrayEmpty(const Qarray *m);
int QarrayCapacity(const Qarray *m);
int QarrayByteNum(const Qarray *m);
cp_t QarrayMemCmp(const Qarray *m, const char *mem, int size);

// table_mem
// 让capacity和count一致，减小内存占用
cp_t QarrayShrink(Qarray *m);

// read
cp_t QarrayIxValid(const Qarray *m, int ix);
void *QarrayItemIx(Qarray *m, int ix);
const void *QarrayItemIxConst(const Qarray *m, int ix);

// write
cp_t QarrayAdd(Qarray *m, const void *item);
cp_t QarrayInsert(Qarray *m, int ix, const void *item);
cp_t QarrayRemove(Qarray *m, int ix);
cp_t QarrayWriteIx(Qarray *m, int ix, const void *item);
cp_t QarrayClear(Qarray *m);

// find
int QarrayFind(const Qarray *m, const void *item);

// table_to_table
cp_t QarrayEqual(const Qarray *x, const Qarray *y);
// 将src内容拷贝到m中，m中原有内容丢失
cp_t QarrayDup(Qarray *m, const Qarray *src);
// 将src内容追加到m中
cp_t QarrayAppend(Qarray *m, const Qarray *src);


cp_t QarrayTest(QTEST_ARG);
#endif


