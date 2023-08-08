#ifndef  __QBUF_H__
#define  __QBUF_H__

#include "qcp.h"
#include "qtimeout.h"

// 通用报文缓存
typedef struct 
{
// public:
	int capacity;		// buf最大长度
	int len;			// 当前存储的报文长度

// private:
	char *buf;			// 缓冲区

	QtimeoutT *timeout;	// 超时模块，如果某次接收，经过了限值时间后，没有收后续内容，则会被清除
} QbufT;
#define kQbufDef {	\
	0, 0,			\
	NULL, NULL,		\
}
extern const QbufT kQbufZero;

// 初始化
cp_t QbufInit(QbufT *m, char *buf, int capacity, QtimeoutT *timeout);
cp_t QbufValid(const QbufT *m);
cp_t QbufEqual(const QbufT *x, const QbufT *y);
void QbufPrint(const QbufT *m);

// info
// 空、满
bool QbufEmpty(const QbufT *m);
bool QbufFull(const QbufT *m);
// 剩余空间
int QbufRoom(const QbufT *m);
int QbufLen(const QbufT *m);
// 比较m->buf内容，和buf里是否一致，长度一致
cp_t QbufCmp(const QbufT *m, const char *buf, int len);


// 当前接收位置
char *QbufPos(QbufT *m);
// 追加一段内容
cp_t QbufAppend(QbufT *m, int len);
// 经过时间pass，用来判断是否超时；
// return: 非0表示发生超时
cp_t QbufPassTime(QbufT *m, int pass);
// 剩余超时时间
//int QbufTimeoutRemain(const QbufT *m);
// 清除所有内容
void QbufClear(QbufT *m);
// 将缓冲区左边的部分去掉
cp_t QbufDiscardLeft(QbufT *m, int discard_n);


#endif

