#ifndef __P2_TIME_H_
#define __P2_TIME_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2TimeName				"time"

// offset-size
#define kP2TimeOffset				(0)
#define kP2TimeSize					(kDlt698TimeSize)

#define kP2TimeWholeSize			(kP2TimeOffset + kP2TimeSize)

// main
cp_t P2TimeExplain(const char *whole);


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2TimeCutIxMain,
	kP2TimeCutNum				
} P2TimeCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2TimeCutNum];
} P2TimePcut;
// 缺省的是没有datatype的
#define kP2TimePcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2TimePcutOpen(P2TimePcut *m);
cp_t P2TimePcutClose(P2TimePcut *m);
cp_t P2TimePcutValid(const P2TimePcut *m);

cp_t P2TimePcutOpenBase(Pcut *base);
cp_t P2TimePcutCloseBase(Pcut *base);

//}}}


// pfill
cp_t P2TimeFillInit(Pfill *m, bool datatype_exist, const Dlt698Time *datetime);

// test
cp_t P2TimeTest(QTEST_ARG);
#endif

