#ifndef __P2_INTEGER_H_
#define __P2_INTEGER_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2IntegerName				"integer"
#define kP2IntegerNameDatatype		"datatype"
#define kP2IntegerNameContent		"content"

// offset-size
#define kP2IntegerContentOffset	(0)
#define kP2IntegerContentSize		(kDlt698IntegerSize)

#define kP2IntegerWholeSize(_datatype_size)		(kP2IntegerContentOffset(_datatype_size) + kP2IntegerContentSize)

// all
int8_t P2IntegerToValue(const char *whole);

//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2IntegerCutIxContent,
	kP2IntegerCutNum				
} P2IntegerPartIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2IntegerCutNum];
	//bool datatype_exist;
} P2IntegerPcut;
// 缺省的是没有datatype的
#define kP2IntegerPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2IntegerPcutOpen(P2IntegerPcut *m);
cp_t P2IntegerPcutClose(P2IntegerPcut *m);
cp_t P2IntegerPcutValid(const P2IntegerPcut *m);

cp_t P2IntegerPcutOpenBase(Pcut *base);
cp_t P2IntegerPcutCloseBase(Pcut *base);

//}}}


// test
cp_t P2IntegerTest(QTEST_ARG);
#endif

