#ifndef __P2_DAR_H_
#define __P2_DAR_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"

// name
#define kP2DarName						"dar"

// offset-size
#define kP2DarOffset					(0)
//#define kP2DarSize						(kP2DarSize)


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2DarCutIxDar,
	kP2DarCutNum				
} P2DarCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2DarCutNum];
} P2DarPcut;
#define kP2DarPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2DarPcutOpen(P2DarPcut *m);
cp_t P2DarPcutClose(P2DarPcut *m);
cp_t P2DarPcutValid(const P2DarPcut *m);

cp_t P2DarPcutOpenBase(Pcut *base);
cp_t P2DarPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2DarTest(QTEST_ARG);
#endif

