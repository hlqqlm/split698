#ifndef __DLT698_45_ARRAY_H_
#define __DLT698_45_ARRAY_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_data_choice.h"
#include "p2_nvar.h"

// name
#define kP2ArrayName				"octet_string"
#define kP2ArrayNameDatatype		"data_type"
#define kP2ArrayNameVarlen			"var_len"
#define kP2ArrayNameContent			"content"



// datatype
// datatype，如果是在“数据类型定义”中，则没有datatype
#define kP2ArrayDatatypeOffset					(0)
#define kP2ArrayDatatypeSize					(kDlt698DatatypeSize)

// var_len
// 长度是变长的，按照标准编码： DL/T 790.6-2010  采用配电线载波系统的配电自动化 第6部分 A-XDR编码规则	
#define kP2ArrayVarlenOffset					(kP2ArrayDatatypeOffset + kP2ArrayDatatypeSize)
#define kP2ArrayVarlenSize(_varlen_size)		(_varlen_size)

// content
#define kP2ArrayContentOffset(_varlen_size)	\
	(kP2ArrayVarlenOffset + kP2ArrayVarlenSize(_varlen_size))
#define kP2ArrayContentSize(_content_size)	(_content_size)

// all
#define kP2ArrayWholeSize(_varlen_size, _content_size)		\
		(kP2ArrayContentOffset(_varlen_size) + kP2ArrayContentSize(_content_size))


// datatype

// var_len
int P2ArrayVarlenSize(const char *whole);

// content
int P2ArrayContentOffset(const char *whole);

// all
int P2ArrayPartSize(const char *whole);
int P2ArrayNum(const char *whole);


//{{{ part
// part_ix
// 各部分索引号
typedef enum
{
	kP2ArrayPartIxDatatype,
	kP2ArrayPartIxVarlen,
	kP2ArrayPartIxContent,
	kP2ArrayPartNum				
} P2ArrayPartIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2ArrayPartNum];

	P2NvarPcut nvar_part;
	int nvar_open_cnt;				// nvar_part open计数
	P2DataChoicePcut one_data_choice;
} P2ArrayPcut;
#define kP2ArrayPcutDef { kPcutDef, { kPcutItemDef }, kP2NvarPcutDef, 0, kP2DataChoicePcutDef }

cp_t P2ArrayPcutOpen(P2ArrayPcut *m);
cp_t P2ArrayPcutClose(P2ArrayPcut *m);
cp_t P2ArrayPcutValid(const P2ArrayPcut *m);

// array_ix		
// ix		数组第ix个元素。
// 数组下标是否合法
cp_t P2ArrayPcutArrayIxValid(int ix, const char *whole);
int P2ArrayPcutIxLen(P2ArrayPcut *m, int ix, const char *whole);
int P2ArrayPcutIxOffset(P2ArrayPcut *m, int ix, const char *whole);
// 内容是否合法
cp_t P2ArrayPcutIxValid(P2ArrayPcut *m, int ix, const char *whole);

cp_t P2ArrayPcutOpenBase(Pcut *base);
cp_t P2ArrayPcutCloseBase(Pcut *base);

// 有数据类型的array
// 如果用在choice中，open/close函数接口必须是这个形式
cp_t P2ArrayDatatypePcutOpen(P2ArrayPcut *m);
cp_t P2ArrayDatatypePcutClose(P2ArrayPcut *m);
cp_t P2ArrayDatatypePcutValid(const P2ArrayPcut *m);
//}}}


// 增加一个元素，同时初始化
cp_t P2ArrayFillInitThenAdd(Pfill *m, Pfill *fill_one_content);

// test
cp_t P2ArrayTest(QTEST_ARG);
#endif

