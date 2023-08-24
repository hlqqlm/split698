#ifndef __P2_RN_H_
#define __P2_RN_H_

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
#define kP2RnName				"rn"
#define kP2RnNameVarlen			"var_len"
#define kP2RnNameContent		"content"



// datatype
// datatype，如果是在“数据类型定义”中，则没有datatype

// var_len
// 长度是变长的，按照标准编码： DL/T 790.6-2010  采用配电线载波系统的配电自动化 第6部分 A-XDR编码规则	
#define kP2RnVarlenOffset					(0)
#define kP2RnVarlenSize(_varlen_size)		(_varlen_size)

// content
#define kP2RnContentOffset(_varlen_size)	\
	(kP2RnVarlenOffset + kP2RnVarlenSize(_varlen_size))
#define kP2RnContentSize(_content_size)	(_content_size)

// all
#define kP2RnWholeSize(_varlen_size, _content_size)		\
		(kP2RnContentOffset(_varlen_size) + kP2RnContentSize(_content_size))


// datatype

// var_len

// content

// all
int P2RnCutSize(const char *whole);

typedef struct
{
	uint8_t foo;
} P2RnValue;
#define kP2RnValueDef { 0 }

//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2RnCutIxVarlen,
	kP2RnCutIxContent,
	kP2RnCutNum				
} P2RnCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2RnCutNum];
} P2RnPcut;
// 缺省的是没有datatype的，不能改，dlt698_45_data_choice.c中用了缺省定义
#define kP2RnPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2RnPcutOpen(P2RnPcut *m);
cp_t P2RnPcutClose(P2RnPcut *m);
cp_t P2RnPcutValid(const P2RnPcut *m);

cp_t P2RnPcutOpenBase(Pcut *base);
cp_t P2RnPcutCloseBase(Pcut *base);
//}}}


// pfill
cp_t P2RnFillInitByMem(Pfill *m, bool datatype_exist, const char *string, int size);
// fill_string	用来填写string内容
cp_t P2RnFillInitByFill(Pfill *m, bool datatype_exist, Pfill *fill_string);

// test
cp_t P2RnTest(QTEST_ARG);
#endif

