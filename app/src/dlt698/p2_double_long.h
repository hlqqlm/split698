#ifndef __P2_DOUBLE_LONG_H_
#define __P2_DOUBLE_LONG_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2DoubleLongName				"double_long"

// offset-size
#define kP2DoubleLongOffset				(0)
#define kP2DoubleLongSize				(4)

#define kP2DoubleLongWholeSize	(kP2DoubleLongOffset + kP2DoubleLongSize)

// all
int32_t P2DoubleLongToValue(const char *whole);


//{{{ part
// part_ix
// 各部分索引号
typedef enum
{
	kP2DoubleLongCutIxMain,
	kP2DoubleLongCutNum				
} P2DoubleLongCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2DoubleLongCutNum];
} P2DoubleLongPcut;
#define kP2DoubleLongPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2DoubleLongPcutOpen(P2DoubleLongPcut *m);
cp_t P2DoubleLongPcutClose(P2DoubleLongPcut *m);
cp_t P2DoubleLongPcutValid(const P2DoubleLongPcut *m);

cp_t P2DoubleLongPcutOpenBase(Pcut *base);
cp_t P2DoubleLongPcutCloseBase(Pcut *base);

//}}}


// pfill
cp_t P2DoubleLongFillInit(Pfill *m, bool datatype_exist, int32_t value);

// test
cp_t P2DoubleLongTest(QTEST_ARG);
#endif

