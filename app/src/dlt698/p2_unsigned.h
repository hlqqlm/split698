#ifndef __P2_UNSIGNED_H_
#define __P2_UNSIGNED_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2UnsignedName				"unsigned"
#define kP2UnsignedNameDatatype		"datatype"
#define kP2UnsignedNameContent		"content"

// offset-size
#define kP2UnsignedContentOffset	(0)
#define kP2UnsignedContentSize		(kDlt698UnsignedSize)

#define kP2UnsignedWholeSize(_datatype_size)		(kP2UnsignedContentOffset(_datatype_size) + kP2UnsignedContentSize)

// all
uint8_t P2UnsignedToValue(const char *whole);

//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2UnsignedCutIxContent,
	kP2UnsignedCutNum				
} P2UnsignedPartIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2UnsignedCutNum];
	//bool datatype_exist;
} P2UnsignedPcut;
// 缺省的是没有datatype的
#define kP2UnsignedPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2UnsignedPcutOpen(P2UnsignedPcut *m);
cp_t P2UnsignedPcutClose(P2UnsignedPcut *m);
cp_t P2UnsignedPcutValid(const P2UnsignedPcut *m);

cp_t P2UnsignedPcutOpenBase(Pcut *base);
cp_t P2UnsignedPcutCloseBase(Pcut *base);

//}}}


// test
cp_t P2UnsignedTest(QTEST_ARG);
#endif

