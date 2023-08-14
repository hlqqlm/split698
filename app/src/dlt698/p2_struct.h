#ifndef __P2_STRUCT_H_
#define __P2_STRUCT_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_data_choice.h"
#include "p2_nvar.h"

// name
#define kP2StructName				"struct"
#define kP2StructNameVarlen			"var_len"
#define kP2StructNameContent		"content"



// var_len
// 长度是变长的，按照标准编码： DL/T 790.6-2010  采用配电线载波系统的配电自动化 第6部分 A-XDR编码规则	
#define kP2StructVarlenOffset					0
#define kP2StructVarlenSize(_varlen_size)		(_varlen_size)

// content
#define kP2StructContentOffset(_varlen_size)	\
	(kP2StructVarlenOffset + kP2StructVarlenSize(_varlen_size))
#define kP2StructContentSize(_content_size)	(_content_size)

// all
#define kP2StructWholeSize(_varlen_size, _content_size)		\
		(kP2StructContentOffset(_varlen_size) + kP2StructContentSize(_content_size))


// var_len
int P2StructVarlenSize(const char *whole);

// content
int P2StructContentOffset(const char *whole);

// all
int P2StructCutSize(const char *whole);
int P2StructNum(const char *whole);


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2StructCutIxVarlen,
	kP2StructCutIxContent,
	kP2StructCutNum				
} P2StructCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2StructCutNum];

	P2NvarPcut nvar_part;
	int nvar_open_cnt;				// nvar_part open计数
	// P2DataChoicePcut one_data_choice;

	P2DataChoicePcut *subs;		// 指向num个P2DataChoicePcut，动态分配
} P2StructPcut;
#define kP2StructPcutDef { kPcutDef, { kPcutItemDef }, kP2NvarPcutDef, 0, NULL }

cp_t P2StructPcutOpen(P2StructPcut *m);
cp_t P2StructPcutClose(P2StructPcut *m);
cp_t P2StructPcutValid(const P2StructPcut *m);

// item_ix		
// ix		struct第ix个元素。
// 下标是否合法
cp_t P2StructPcutStructIxValid(int ix, const char *whole);
int P2StructPcutIxLen(P2StructPcut *m, int ix, const char *whole);
int P2StructPcutIxOffset(P2StructPcut *m, int ix, const char *whole);
// 内容是否合法
cp_t P2StructPcutIxValid(P2StructPcut *m, int ix, const char *whole);

cp_t P2StructPcutOpenBase(Pcut *base);
cp_t P2StructPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2StructTest(QTEST_ARG);
#endif

