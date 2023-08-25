#ifndef __P2_NULL_H_
#define __P2_NULL_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"

// name
#define kP2NullName						"null"

// offset-size
#define kP2NullOffset					(0)
#define kP2NullSize						(0)


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2NullCutIxNull,
	kP2NullCutNum				
} P2NullCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2NullCutNum];
} P2NullPcut;
#define kP2NullPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2NullPcutOpen(P2NullPcut *m);
cp_t P2NullPcutClose(P2NullPcut *m);
cp_t P2NullPcutValid(const P2NullPcut *m);

cp_t P2NullPcutOpenBase(Pcut *base);
cp_t P2NullPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2NullTest(QTEST_ARG);
#endif

