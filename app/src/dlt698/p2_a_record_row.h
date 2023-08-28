#ifndef __P2_A_RECORD_ROW_H_ 
#define __P2_A_RECORD_ROW_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"
#include "p2_sequence_of_var_len.h"


#define kP2ARecordRowName				"a_record_row"
#define kP2ARecordRowNameSequenceOfData	"sequence_of_data"

//{{{ base
/*
A-RecordRow∷=SEQUENCE
{
 第 1 列数据 Data，
  第 2 列数据 Data，
   „
    第 N 列数据 Data
	}
*/
#define kP2ARecordRowSequenceOfDataOffset				(0)
#define kP2ARecordRowSequenceOfDataSize(_sod_size)		(_sod_size)


#define kP2ARecordRowWholeSize(_sod_size)	(kP2ARecordRowSequenceOfDataOffset + kP2ARecordRowSequenceOfDataSize(_sod_size))
//}}}


//{{{ cut
typedef enum
{
	kP2ARecordRowCutIxSequenceOfData,
	kP2ARecordRowCutNum				
} P2ARecordRowCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ARecordRowCutNum];

	// sub
	P2DataChoicePcut data;
	P2SequenceOfVarLenPcut sequence_of_data;
} P2ARecordRowPcut;
#define kP2ARecordRowPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2DataChoicePcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2ARecordRowPcutOpen(P2ARecordRowPcut *m);
cp_t P2ARecordRowPcutClose(P2ARecordRowPcut *m);
cp_t P2ARecordRowPcutValid(const P2ARecordRowPcut *m);

cp_t P2ARecordRowPcutOpenBase(Pcut *base);
cp_t P2ARecordRowPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ARecordRowTest(QTEST_ARG);
#endif

