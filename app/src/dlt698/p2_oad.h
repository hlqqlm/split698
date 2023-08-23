#ifndef __P2_OAD_H_
#define __P2_OAD_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2OadName				"double_long"

// offset-size
#define kP2OadOffset			(0)
#define kP2OadSize				(OAD_MEM_SIZE)

#define kP2OadWholeSize			(kP2OadOffset + kP2OadSize)

// all
//int32_t P2OadToValue(const char *whole);


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2OadCutIxMain,
	kP2OadCutNum				
} P2OadCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2OadCutNum];
} P2OadPcut;
#define kP2OadPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2OadPcutOpen(P2OadPcut *m);
cp_t P2OadPcutClose(P2OadPcut *m);
cp_t P2OadPcutValid(const P2OadPcut *m);

cp_t P2OadPcutOpenBase(Pcut *base);
cp_t P2OadPcutCloseBase(Pcut *base);

//}}}


// test
cp_t P2OadTest(QTEST_ARG);
#endif

