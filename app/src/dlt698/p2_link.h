#ifndef __P2_LINK_H_
#define __P2_LINK_H_

#include "qos/qcp.h"
#include "qos/qbuf.h"
#include "qos/qtimeout.h"
#include "p2_base.h"


// 接收缓冲区，可以满足保存一帧
#define P2_LINK_RX_BUF_SIZE				(1 * (P2_FRAME_SIZE_MAX))
// 接收一个报文，字符之间间隔时间最大值
#define P2_RX_INTERVAL_TIMEOUT_SEC		(30)		



// 通用报文缓存
typedef struct P2LinkS
{
// public:
	int first_rx_error_part_ix;		// 第一个错误的PartIx，表示对应的部分出错。测试用
	int last_rx_error_part_ix;		// 最后一个错误的PartIx，表示对应的部分出错
									// 如果不在[kP2PartIxHead, kP2PartNum)之间，表示上一次没有错误
	cp_t last_rx_error_cp;			// 错误位置
	int last_discard_n;				// 上一次抛弃的非法字符数
	int discard_total;				// 计数解帧过程中，抛弃的非法字符总数
	int discard_cnt;				// 解帧过程中，抛弃次数计数

	bool valid_frame_exist;			// 避免反复判断有效帧是否存在，在Update()后取此变量

// private:
	cp_t (*print_rx_error)(const struct P2LinkS *);	// 打印接收错误

	QtimeoutT timeout;				// 超时判断
	QbufT qbuf;						// 缓冲区
	P2Pcut pcut;					// 用来检查帧合法性

	char buf[P2_LINK_RX_BUF_SIZE];
} P2Link;
#define kP2LinkDef {									\
	-1,	-1, 0, 0, 0, 0, false,					\
	P2LinkPrintRxErrorDisable,						\
	kQtimeoutNormal, kQbufDef, kP2PcutDef, {0}		\
}

// class
cp_t P2LinkOpen(P2Link *m);
cp_t P2LinkClose(P2Link *m);
cp_t P2LinkValid(const P2Link *m);
// 打印接收时的错误
typedef cp_t (*P2LinkPrintRxErrorT)(const P2Link *m);
// 配置解帧错误打印函数
void P2LinkConfigPrintRxError(P2Link *m, P2LinkPrintRxErrorT print_rx_error);
void P2LinkPrint(const P2Link *m);

// get
P2Pcut *P2LinkPart(P2Link *m);

// rx
int P2LinkRoom(const P2Link *m);
int P2LinkRxSize(const P2Link *m);
char *P2LinkBuf(P2Link *m);
char *P2LinkRxPos(P2Link *m);
cp_t P2LinkAppend(P2Link *m, int len);
// 经过时间pass，用来判断接收超时. return: 非0表示发生超时
cp_t P2LinkPassTime(P2Link *m, int pass);
// return: true 有效帧存在在buf中
bool P2LinkUpdate(P2Link *m);

// valid_frame
// 缓冲区内有完整的帧，可以调用的函数如下
// return: 0 缓冲区内有一个完整有效帧
cp_t P2LinkFrameValid(P2Link *m);
int P2LinkFrameSize(P2Link *m);
cp_t P2LinkDiscardValidFrame(P2Link *m);
void P2LinkFramePrint(P2Link *m);


// 正常打印错误
cp_t P2LinkPrintRxErrorNormal(const P2Link *m);
// 禁止打印
cp_t P2LinkPrintRxErrorDisable(const P2Link *m);

// test
cp_t P2LinkTest(QTEST_ARG);
#endif

