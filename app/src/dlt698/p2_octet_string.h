#ifndef __P2_OCTET_STRING_H_
#define __P2_OCTET_STRING_H_

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
#define kP2OctetStringName				"octet_string"
#define kP2OctetStringNameVarlen		"var_len"
#define kP2OctetStringNameContent		"content"



// datatype
// datatype，如果是在“数据类型定义”中，则没有datatype
//#define kP2OctetStringDatatypeOffset					(0)
//#define kP2OctetStringDatatypeSize(_datatype_size)	(_datatype_size)

// var_len
// 长度是变长的，按照标准编码： DL/T 790.6-2010  采用配电线载波系统的配电自动化 第6部分 A-XDR编码规则	
#define kP2OctetStringVarlenOffset					(0)
#define kP2OctetStringVarlenSize(_varlen_size)		(_varlen_size)

// content
#define kP2OctetStringContentOffset(_varlen_size)	\
	(kP2OctetStringVarlenOffset + kP2OctetStringVarlenSize(_varlen_size))
#define kP2OctetStringContentSize(_content_size)	(_content_size)

// all
#define kP2OctetStringWholeSize(_varlen_size, _content_size)		\
		(kP2OctetStringContentOffset(_varlen_size) + kP2OctetStringContentSize(_content_size))


// datatype

// var_len

// content

// all
int P2OctetStringCutSize(const char *whole);

typedef struct
{
	uint8_t foo;
} P2OctetStringValue;
#define kP2OctetStringValueDef { 0 }

//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	//kP2OctetStringCutIxDatatype,
	kP2OctetStringCutIxVarlen,
	kP2OctetStringCutIxContent,
	kP2OctetStringCutNum				
} P2OctetStringCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2OctetStringCutNum];
} P2OctetStringPcut;
// 缺省的是没有datatype的，不能改，dlt698_45_data_choice.c中用了缺省定义
#define kP2OctetStringPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2OctetStringPcutOpen(P2OctetStringPcut *m);
cp_t P2OctetStringPcutClose(P2OctetStringPcut *m);
cp_t P2OctetStringPcutValid(const P2OctetStringPcut *m);

cp_t P2OctetStringPcutOpenBase(Pcut *base);
cp_t P2OctetStringPcutCloseBase(Pcut *base);
//}}}


// pfill
cp_t P2OctetStringFillInitByMem(Pfill *m, bool datatype_exist, const char *string, int size);
// fill_string	用来填写string内容
cp_t P2OctetStringFillInitByFill(Pfill *m, bool datatype_exist, Pfill *fill_string);

// test
cp_t P2OctetStringTest(QTEST_ARG);
#endif

