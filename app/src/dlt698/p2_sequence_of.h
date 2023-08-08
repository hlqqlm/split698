#ifndef __DLT698_45_SEQUENCE_OF_H_ 
#define __DLT698_45_SEQUENCE_OF_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_nfix.h"


#define kP2SequenceOfNumOffset	(0)
#define kP2SequenceOfNumSize(_num_size)	(_num_size)

#define kP2SequenceOfContentOffset(_num_size)	(kP2SequenceOfNumOffset + kP2SequenceOfNumSize(_num_size))
// _content_size = num_value * one_content_size
#define kP2SequenceOfContentSize(_content_size)	(_content_size)


#define kP2SequenceOfWholeSize(_num_size, _content_size)	(kP2SequenceOfContentOffset(_num_size) + kP2SequenceOfContentSize(_content_size))

// num
int P2SequenceOfNum(const char *whole);

// content
cp_t P2SequenceOfContentIxValid(const char *whole, int content_ix);
int P2SequenceOfContentOffset(const char *whole);
int P2SequenceOfContentIxOffset(int one_size, const char *whole, int content_ix);
const char *P2SequenceOfContentIx(int one_size, const char *whole, int content_ix);

//{{{ part
typedef enum
{
	kP2SequenceOfPartIxNum,
	kP2SequenceOfPartIxContent,				// num对应的变化部分
	kP2SequenceOfPartNum				
} P2SequenceOfPartIxT;

typedef struct
{
	Pcut base;		
	PcutItem items[kP2SequenceOfPartNum];

	int one_size;					// 一个元素的大小
	const char *content_name;		// 重复元素名称

	P2NfixPcut nfix_part;
	int nfix_open_cnt;
} P2SequenceOfPcut;
#define kP2SequenceOfPcutDef {		\
	kPcutDef, { kPcutItemDef },	\
	0, "no_name",					\
	kP2NfixPcutDef, 0,				\
}

cp_t P2SequenceOfPcutOpen(P2SequenceOfPcut *m, int one_size, const char *content_name);
cp_t P2SequenceOfPcutClose(P2SequenceOfPcut *m);
cp_t P2SequenceOfPcutValid(const P2SequenceOfPcut *m);
//}}}



// test
cp_t P2SequenceOfTest(QTEST_ARG);
#endif

