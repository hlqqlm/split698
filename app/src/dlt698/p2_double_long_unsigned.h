#ifndef __P2_DOUBLE_LONG_UNSIGNED_H_
#define __P2_DOUBLE_LONG_UNSIGNED_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2DoubleLongUnsignedName				"double_long_unsigned"
#define kP2DoubleLongUnsignedNameDatatype		"datatype"
#define kP2DoubleLongUnsignedNameContent		"content"

// offset-size
#define kP2DoubleLongUnsignedDatatypeOffset						(0)
#define kP2DoubleLongUnsignedDatatypeSize(_datatype_size)		(_datatype_size)

#define kP2DoubleLongUnsignedContentOffset(_datatype_size)		(kP2DoubleLongUnsignedDatatypeOffset + kP2DoubleLongUnsignedDatatypeSize(_datatype_size))
#define kP2DoubleLongUnsignedContentSize						(kDlt698DoubleLongUnsignedSize)

#define kP2DoubleLongUnsignedWholeSize(_datatype_size)			(kP2DoubleLongUnsignedContentOffset(_datatype_size) + kP2DoubleLongUnsignedContentSize)

// all
int P2DoubleLongUnsignedCutSize(bool datatype_exist);
uint32_t P2DoubleLongUnsignedToValue(bool datatype_exist, const char *whole);

//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2DoubleLongUnsignedCutIxDatatype,
	kP2DoubleLongUnsignedCutIxContent,
	kP2DoubleLongUnsignedCutNum				
} P2DoubleLongUnsignedCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2DoubleLongUnsignedCutNum];
	bool datatype_exist;
} P2DoubleLongUnsignedPcut;
// 缺省的是没有datatype的
#define kP2DoubleLongUnsignedPcutDef { kPcutDef, { kPcutItemDef }, false }

cp_t P2DoubleLongUnsignedPcutOpen(P2DoubleLongUnsignedPcut *m);
cp_t P2DoubleLongUnsignedPcutClose(P2DoubleLongUnsignedPcut *m);
cp_t P2DoubleLongUnsignedPcutValid(const P2DoubleLongUnsignedPcut *m);

// 配置数据类型字节是否存在
void P2DoubleLongUnsignedPcutConfigDatatypeExist(P2DoubleLongUnsignedPcut *m, bool exist);

cp_t P2DoubleLongUnsignedPcutOpenBase(Pcut *base);
cp_t P2DoubleLongUnsignedPcutCloseBase(Pcut *base);

//}}}


// pfill
cp_t P2DoubleLongUnsignedFillInit(Pfill *m, bool datatype_exist, uint32_t value);

// test
cp_t P2DoubleLongUnsignedTest(QTEST_ARG);
#endif

