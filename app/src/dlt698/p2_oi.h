#ifndef __P2_OI_H_
#define __P2_OI_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2OiName				"oi"

// offset-size
#define kP2OiOffset			(0)
#define kP2OiSize				(OI_MEM_SIZE)

#define kP2OiWholeSize			(kP2OiOffset + kP2OiSize)

// all
//int32_t P2OiToValue(const char *whole);


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2OiCutIxMain,
	kP2OiCutNum				
} P2OiCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2OiCutNum];
} P2OiPcut;
#define kP2OiPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2OiPcutOpen(P2OiPcut *m);
cp_t P2OiPcutClose(P2OiPcut *m);
cp_t P2OiPcutValid(const P2OiPcut *m);

cp_t P2OiPcutOpenBase(Pcut *base);
cp_t P2OiPcutCloseBase(Pcut *base);

//}}}


// test
cp_t P2OiTest(QTEST_ARG);
#endif

