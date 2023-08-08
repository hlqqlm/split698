#ifndef __DLT698_45_DATETIME_H_
#define __DLT698_45_DATETIME_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2DatetimeName				"datetime"

// offset-size
#define kP2DatetimeOffset				(0)
#define kP2DatetimeSize					(kDlt698DatetimeSize)

#define kP2DatetimeWholeSize			(kP2DatetimeOffset + kP2DatetimeSize)

// main
cp_t P2DatetimeExplain(const char *whole);


//{{{ part
// part_ix
// 各部分索引号
typedef enum
{
	kP2DatetimePartIxMain,
	kP2DatetimePartNum				
} P2DatetimePartIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2DatetimePartNum];
	bool datatype_exist;
} P2DatetimePcut;
// 缺省的是没有datatype的
#define kP2DatetimePcutDef { kPcutDef, { kPcutItemDef }, false }

cp_t P2DatetimePcutOpen(P2DatetimePcut *m);
cp_t P2DatetimePcutClose(P2DatetimePcut *m);
cp_t P2DatetimePcutValid(const P2DatetimePcut *m);

// 配置数据类型字节是否存在
void P2DatetimePcutConfigDatatypeExist(P2DatetimePcut *m, bool exist);

cp_t P2DatetimePcutOpenBase(Pcut *base);
cp_t P2DatetimePcutCloseBase(Pcut *base);

//}}}


// pfill
cp_t P2DatetimeFillInit(Pfill *m, bool datatype_exist, const Dlt698Datetime *datetime);

// test
cp_t P2DatetimeTest(QTEST_ARG);
#endif

