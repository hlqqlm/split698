#ifndef __P2_DATETIMES_H_
#define __P2_DATETIMES_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2DatetimesName				"date_time_s"

// offset-size
#define kP2DatetimesOffset				(0)
#define kP2DatetimesSize				(kDlt698DatetimeSSize)

#define kP2DatetimesWholeSize			(kP2DatetimesOffset + kP2DatetimesSize)

// main
cp_t P2DatetimesExplain(const char *whole);


//{{{ part
// part_ix
// 各部分索引号
typedef enum
{
	kP2DatetimesCutIxMain,
	kP2DatetimesCutNum				
} P2DatetimesCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2DatetimesCutNum];
} P2DatetimesPcut;
// 缺省的是没有datatype的
#define kP2DatetimesPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2DatetimesPcutOpen(P2DatetimesPcut *m);
cp_t P2DatetimesPcutClose(P2DatetimesPcut *m);
cp_t P2DatetimesPcutValid(const P2DatetimesPcut *m);

cp_t P2DatetimesPcutOpenBase(Pcut *base);
cp_t P2DatetimesPcutCloseBase(Pcut *base);

//}}}


// pfill
cp_t P2DatetimesFillInit(Pfill *m, bool datatype_exist, const Dlt698DatetimeS *date_time_s);

// test
cp_t P2DatetimesTest(QTEST_ARG);
#endif

