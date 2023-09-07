#ifndef __P2_LONG64_H_
#define __P2_LONG64_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2Long64Name				"long64"

// offset-size
#define kP2Long64Offset				(0)
#define kP2Long64Size				(8)

#define kP2Long64WholeSize	(kP2Long64Offset + kP2Long64Size)

// all
int64_t P2Long64ToValue(const char *whole);


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2Long64CutIxMain,
	kP2Long64CutNum				
} P2Long64CutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2Long64CutNum];
} P2Long64Pcut;
#define kP2Long64PcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2Long64PcutOpen(P2Long64Pcut *m);
cp_t P2Long64PcutClose(P2Long64Pcut *m);
cp_t P2Long64PcutValid(const P2Long64Pcut *m);

cp_t P2Long64PcutOpenBase(Pcut *base);
cp_t P2Long64PcutCloseBase(Pcut *base);

//}}}


// pfill
cp_t P2Long64FillInit(Pfill *m, bool datatype_exist, int64_t value);

// test
cp_t P2Long64Test(QTEST_ARG);
#endif

