#ifndef __P2_TSA_H_
#define __P2_TSA_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
//#include "p2_octet_string.h"
#include "p2_addr.h"


// name
#define kP2TsaName				"tsa"
#define kP2TsaNameVarlen		"var_len"
#define kP2TsaNameContent		"content"

#define kP2TsaContentSizeMin	(2)
#define kP2TsaContentSizeMax	(7)
/*
数据类型定义 说明
TSA∷=octet-string(SIZE(2...7))
*/


// var_len
// 长度是变长的，按照标准编码： DL/T 790.6-2010  采用配电线载波系统的配电自动化 第6部分 A-XDR编码规则	
#define kP2TsaVarlenOffset					(0)
#define kP2TsaVarlenSize(_varlen_size)		(_varlen_size)

// content
#define kP2TsaContentOffset(_varlen_size)	\
	(kP2TsaVarlenOffset + kP2TsaVarlenSize(_varlen_size))
#define kP2TsaContentSize(_content_size)	(_content_size)

// all
#define kP2TsaWholeSize(_varlen_size, _content_size)		\
		(kP2TsaContentOffset(_varlen_size) + kP2TsaContentSize(_content_size))


// datatype

// var_len

// content
cp_t P2TsaContentSizeValid(int content_size);

// all
int P2TsaCutSize(const char *whole);

typedef struct
{
	uint8_t foo;
} P2TsaValue;
#define kP2TsaValueDef { 0 }

//{{{ part
// part_ix
// 各部分索引号
typedef enum
{
	kP2TsaCutIxVarlen,
	kP2TsaCutIxContent,
	kP2TsaCutNum				
} P2TsaCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2TsaCutNum];

	P2AddrPcut addr;		// 对content部分解析
} P2TsaPcut;
#define kP2TsaPcutDef { kPcutDef, { kPcutItemDef }, kP2AddrPcutDef }

cp_t P2TsaPcutOpen(P2TsaPcut *m);
cp_t P2TsaPcutClose(P2TsaPcut *m);
cp_t P2TsaPcutValid(const P2TsaPcut *m);


cp_t P2TsaPcutOpenBase(Pcut *base);
cp_t P2TsaPcutCloseBase(Pcut *base);

// 有数据类型的octet-string
// 如果用在choice中，open/close函数接口必须是这个形式
//cp_t P2TsaDatatypePcutOpen(P2TsaPcut *m);
//cp_t P2TsaDatatypePcutClose(P2TsaPcut *m);
//cp_t P2TsaDatatypePcutValid(const P2TsaPcut *m);
//}}}


// pfill
cp_t P2TsaFillInitByMem(Pfill *m, bool datatype_exist, const char *string, int size);
// fill_string	用来填写string内容
cp_t P2TsaFillInitByFill(Pfill *m, bool datatype_exist, Pfill *fill_string);

// test
cp_t P2TsaTest(QTEST_ARG);
#endif

