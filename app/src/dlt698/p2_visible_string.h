#ifndef __P2_VISIBLE_STRING_H_
#define __P2_VISIBLE_STRING_H_

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
#define kP2VisibleStringName			"visible_string"
#define kP2VisibleStringNameVarlen		"var_len"
#define kP2VisibleStringNameContent		"content"



// datatype
// datatype，如果是在“数据类型定义”中，则没有datatype
//#define kP2VisibleStringDatatypeOffset					(0)
//#define kP2VisibleStringDatatypeSize(_datatype_size)	(_datatype_size)

// var_len
// 长度是变长的，按照标准编码： DL/T 790.6-2010  采用配电线载波系统的配电自动化 第6部分 A-XDR编码规则	
#define kP2VisibleStringVarlenOffset					(0)
#define kP2VisibleStringVarlenSize(_varlen_size)		(_varlen_size)

// content
#define kP2VisibleStringContentOffset(_varlen_size)	\
	(kP2VisibleStringVarlenOffset + kP2VisibleStringVarlenSize(_varlen_size))
#define kP2VisibleStringContentSize(_content_size)	(_content_size)

// all
#define kP2VisibleStringWholeSize(_varlen_size, _content_size)		\
		(kP2VisibleStringContentOffset(_varlen_size) + kP2VisibleStringContentSize(_content_size))


// datatype

// var_len

// content

// all
int P2VisibleStringCutSize(const char *whole);

typedef struct
{
	uint8_t foo;
} P2VisibleStringValue;
#define kP2VisibleStringValueDef { 0 }

//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2VisibleStringCutIxVarlen,
	kP2VisibleStringCutIxContent,
	kP2VisibleStringCutNum				
} P2VisibleStringCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2VisibleStringCutNum];
	//bool datatype_exist;
} P2VisibleStringPcut;
// 缺省的是没有datatype的，不能改，dlt698_45_data_choice.c中用了缺省定义
//#define kP2VisibleStringPcutDef { kPcutDef, { kPcutItemDef }, false }
#define kP2VisibleStringPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2VisibleStringPcutOpen(P2VisibleStringPcut *m);
cp_t P2VisibleStringPcutClose(P2VisibleStringPcut *m);
cp_t P2VisibleStringPcutValid(const P2VisibleStringPcut *m);

// 配置数据类型字节是否存在
//void P2VisibleStringPcutConfigDatatypeExist(P2VisibleStringPcut *m, bool exist);

cp_t P2VisibleStringPcutOpenBase(Pcut *base);
cp_t P2VisibleStringPcutCloseBase(Pcut *base);

// 有数据类型的octet-string
// 如果用在choice中，open/close函数接口必须是这个形式
//cp_t P2VisibleStringDatatypePcutOpen(P2VisibleStringPcut *m);
//cp_t P2VisibleStringDatatypePcutClose(P2VisibleStringPcut *m);
//cp_t P2VisibleStringDatatypePcutValid(const P2VisibleStringPcut *m);
//}}}


// pfill
cp_t P2VisibleStringFillInitByMem(Pfill *m, bool datatype_exist, const char *string, int size);
// fill_string	用来填写string内容
cp_t P2VisibleStringFillInitByFill(Pfill *m, bool datatype_exist, Pfill *fill_string);

// test
cp_t P2VisibleStringTest(QTEST_ARG);
#endif

