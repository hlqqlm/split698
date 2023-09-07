#ifndef __P2_BIT_STRING_H_
#define __P2_BIT_STRING_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"

// 因为DLT698_45前缀太长，因此，将程序内的协议编号
// 编号	协议名称		缩写
// 1	DLT645_07		P1	(protocol 1#)
// 2	DLT698_45		P2	(protocol 2#)

// name
#define kP2BitStringName				"bit_string"
#define kP2BitStringNameVarlen		"var_len"
#define kP2BitStringNameContent		"content"



// datatype
// datatype，如果是在“数据类型定义”中，则没有datatype
//#define kP2BitStringDatatypeOffset					(0)
//#define kP2BitStringDatatypeSize(_datatype_size)	(_datatype_size)

// var_len
// 长度是变长的，按照标准编码： DL/T 790.6-2010  采用配电线载波系统的配电自动化 第6部分 A-XDR编码规则	
#define kP2BitStringVarlenOffset					(0)
#define kP2BitStringVarlenSize(_varlen_size)		(_varlen_size)

// content
#define kP2BitStringContentOffset(_varlen_size)	\
	(kP2BitStringVarlenOffset + kP2BitStringVarlenSize(_varlen_size))
#define kP2BitStringContentSize(_content_size)	(_content_size)

// all
#define kP2BitStringWholeSize(_varlen_size, _content_size)		\
		(kP2BitStringContentOffset(_varlen_size) + kP2BitStringContentSize(_content_size))


// datatype

// var_len

// content

// all
int P2BitStringCutSize(const char *whole);

typedef struct
{
	uint8_t foo;
} P2BitStringValue;
#define kP2BitStringValueDef { 0 }

//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	//kP2BitStringCutIxDatatype,
	kP2BitStringCutIxVarlen,
	kP2BitStringCutIxContent,
	kP2BitStringCutNum				
} P2BitStringCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2BitStringCutNum];
} P2BitStringPcut;
// 缺省的是没有datatype的，不能改，dlt698_45_data_choice.c中用了缺省定义
#define kP2BitStringPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2BitStringPcutOpen(P2BitStringPcut *m);
cp_t P2BitStringPcutClose(P2BitStringPcut *m);
cp_t P2BitStringPcutValid(const P2BitStringPcut *m);

cp_t P2BitStringPcutOpenBase(Pcut *base);
cp_t P2BitStringPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2BitStringTest(QTEST_ARG);
#endif

