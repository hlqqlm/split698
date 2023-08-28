#ifndef __P2_SEQUENCE_OF_A_RECORD_ROW_H_ 
#define __P2_SEQUENCE_OF_A_RECORD_ROW_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_a_record_row.h"
#include "p2_sequence_of_var_len.h"


#define kP2SequenceOfARecordRowName				"sequence_of_a_record_row"
#define kP2SequenceOfARecordRowNameSequenceOf	"sequence_of"

//{{{ base
/*
SEQUENCE OF A-RecordRow
*/
#define kP2SequenceOfARecordRowSequenceOfOffset			(0)
#define kP2SequenceOfARecordRowSequenceOfSize(_so_size)	(_so_size)


#define kP2SequenceOfARecordRowWholeSize(_so_size)	(kP2SequenceOfARecordRowSequenceOfOmdDataOffset + kP2SequenceOfARecordRowSequenceOfOmdDataSize(_so_size))
//}}}


//{{{ cut
typedef enum
{
	kP2SequenceOfARecordRowCutIxSequenceOf,
	kP2SequenceOfARecordRowCutNum				
} P2SequenceOfARecordRowCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SequenceOfARecordRowCutNum];

	// sub
	P2ARecordRowPcut a_record_row;
	P2SequenceOfVarLenPcut sequence_of;
} P2SequenceOfARecordRowPcut;
#define kP2SequenceOfARecordRowPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2ARecordRowPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2SequenceOfARecordRowPcutOpen(P2SequenceOfARecordRowPcut *m);
cp_t P2SequenceOfARecordRowPcutClose(P2SequenceOfARecordRowPcut *m);
cp_t P2SequenceOfARecordRowPcutValid(const P2SequenceOfARecordRowPcut *m);

cp_t P2SequenceOfARecordRowPcutOpenBase(Pcut *base);
cp_t P2SequenceOfARecordRowPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2SequenceOfARecordRowTest(QTEST_ARG);
#endif

