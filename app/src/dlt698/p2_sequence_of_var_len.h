#ifndef __P2_SEQUENCE_OF_VAR_LEN_H_ 
#define __P2_SEQUENCE_OF_VAR_LEN_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill_repository.h"
#include "qdlt698.h"
#include "p2_nvar.h"

// name
#define kP2SequenceOfVarLenName			"sequence_of"
#define kP2SequenceOfVarLenNameNum		"num"
#define kP2SequenceOfVarLenNameContent	"content"


// offset-size
#define kP2SequenceOfVarLenNumOffset	(0)
#define kP2SequenceOfVarLenNumSize(_num_size)	(_num_size)

#define kP2SequenceOfVarLenContentOffset(_num_size)	(kP2SequenceOfVarLenNumOffset + kP2SequenceOfVarLenNumSize(_num_size))
// _content_size = num_value * one_content_size
#define kP2SequenceOfVarLenContentSize(_content_size)	(_content_size)


#define kP2SequenceOfVarLenWholeSize(_num_size, _content_size)	(kP2SequenceOfContentOffset(_num_size) + kP2SequenceOfContentSize(_content_size))

// num
int P2SequenceOfVarLenNum(const char *whole);

// content
cp_t P2SequenceOfVarLenContentIxValid(const char *whole, int content_ix);
int P2SequenceOfVarLenContentOffset(const char *whole);


//{{{ cut
typedef enum
{
	kP2SequenceOfVarLenCutIxNum,
	kP2SequenceOfVarLenCutIxContent,				// num对应的变化部分
	kP2SequenceOfVarLenCutNum				
} P2SequenceOfVarLenCutIxT;

typedef struct
{
	Pcut base;		
	PcutItem items[kP2SequenceOfVarLenCutNum];

	Pcut *cut_one;				// 用来解析一个content单元的对象
	const char *content_name;		// 重复元素名称
									//
	P2NvarPcut nvar_cut;
	int nvar_open_cnt;
} P2SequenceOfVarLenPcut;
#define kP2SequenceOfVarLenPcutDef {		\
	kPcutDef, { kPcutItemDef },		\
	NULL, "no_name",					\
	kP2NvarPcutDef, 0,					\
}

cp_t P2SequenceOfVarLenPcutOpen(P2SequenceOfVarLenPcut *m, Pcut *cut_one, const char *content_name);
cp_t P2SequenceOfVarLenPcutClose(P2SequenceOfVarLenPcut *m);
cp_t P2SequenceOfVarLenPcutValid(const P2SequenceOfVarLenPcut *m);
//}}}


// fill
cp_t P2SequenceOfVarLenFillInit(Pfill *m, PfillRepository *fill_repository);

// test
cp_t P2SequenceOfVarLenTest(QTEST_ARG);
#endif

