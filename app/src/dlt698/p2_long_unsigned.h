#ifndef __P2_LONG_UNSIGNED_H_
#define __P2_LONG_UNSIGNED_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2LongUnsignedName				"long_unsigned"
#define kP2LongUnsignedNameDatatype		"datatype"
#define kP2LongUnsignedNameContent		"content"

// offset-size
//#define kP2LongUnsignedDatatypeOffset					(0)
//#define kP2LongUnsignedDatatypeSize(_datatype_size)		(_datatype_size)

//#define kP2LongUnsignedContentOffset(_datatype_size)	(kP2LongUnsignedDatatypeOffset + kP2LongUnsignedDatatypeSize(_datatype_size))
#define kP2LongUnsignedContentOffset	(0)
#define kP2LongUnsignedContentSize		(kDlt698LongUnsignedSize)

#define kP2LongUnsignedWholeSize(_datatype_size)		(kP2LongUnsignedContentOffset(_datatype_size) + kP2LongUnsignedContentSize)

// all
//int P2LongUnsignedPartSize(bool datatype_exist);
//uint16_t P2LongUnsignedToValue(bool datatype_exist, const char *whole);
uint16_t P2LongUnsignedToValue(const char *whole);
void P2LongUnsignedExplain(const char *whole);


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	//kP2LongUnsignedPartIxDatatype,
	kP2LongUnsignedCutIxContent,
	kP2LongUnsignedCutNum				
} P2LongUnsignedPartIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2LongUnsignedCutNum];
	//bool datatype_exist;
} P2LongUnsignedPcut;
// 缺省的是没有datatype的
#define kP2LongUnsignedPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2LongUnsignedPcutOpen(P2LongUnsignedPcut *m);
cp_t P2LongUnsignedPcutClose(P2LongUnsignedPcut *m);
cp_t P2LongUnsignedPcutValid(const P2LongUnsignedPcut *m);

// 配置数据类型字节是否存在
// void P2LongUnsignedPcutConfigDatatypeExist(P2LongUnsignedPcut *m, bool exist);

cp_t P2LongUnsignedPcutOpenBase(Pcut *base);
cp_t P2LongUnsignedPcutCloseBase(Pcut *base);

//}}}


// pfill
cp_t P2LongUnsignedFillInit(Pfill *m, bool datatype_exist, uint16_t value);

// test
cp_t P2LongUnsignedTest(QTEST_ARG);
#endif

