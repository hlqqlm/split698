#ifndef __P2_A_COMPACT_RESULT_RECORD_H_
#define __P2_A_COMPACT_RESULT_RECORD_H_



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#include "p2_choice.h"


#define kP2ACompactResultRecordName				"a_compact_result_record"
#define kP2ACompactResultRecordNameChoice		"a_compact_result_record_choice"
#define kP2ACompactResultRecordNameVar			"a_compact_result_record_var"

#define kP2ACompactResultRecordChoiceOffset		(0)
#define kP2ACompactResultRecordChoiceSize		(1)

#define kP2ACompactResultRecordVarOffset			(kP2ACompactResultRecordChoiceOffset + kP2ACompactResultRecordChoiceSize)
#define kP2ACompactResultRecordVarSize(_var_size)	(_var_size)

//{{{ choice
/*
A-CompactResultRecord∷=CHOICE
{
 错误信息 [0] DAR，
  M 条记录 [1] SEQUENCE OF A-RecordRow
  }
*/
typedef enum 
{
	kP2ACompactResultRecordChoiceDar = 0,		// 错误信息 [0] DAR，
	kP2ACompactResultRecordChoiceSequenceOfARecordRow = 1,	// M 条记录 [1] SEQUENCE OF A-RecordRow
} P2ACompactResultRecordChoiceT;

typedef enum 
{
	kP2ACompactResultRecordChoiceIxDar,	// 
	kP2ACompactResultRecordChoiceIxSequenceOfARecordRow,	// 
	kP2ACompactResultRecordChoiceNum
} P2ACompactResultRecordChoiceIxT;
int P2ACompactResultRecordChoice2Ix(uint8_t choice);
uint8_t P2ACompactResultRecordChoiceIx2Value(int choice_ix);
cp_t P2ACompactResultRecordChoiceIxValid(int choice_ix);
cp_t P2ACompactResultRecordChoiceValid(uint8_t choice);
const char *P2ACompactResultRecordChoiceStr(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2ACompactResultRecordPcut;
#define kP2ACompactResultRecordPcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2ACompactResultRecordPcutOpen(P2ACompactResultRecordPcut *m);
cp_t P2ACompactResultRecordPcutClose(P2ACompactResultRecordPcut *m);
cp_t P2ACompactResultRecordPcutValid(const P2ACompactResultRecordPcut *m);


// 声明用子类base来open/close父类的函数
//PCUT_DECLEAR_OPEN_BASE(P2ACompactResultRecordPcut);
//PCUT_DECLEAR_CLOSE_BASE(P2ACompactResultRecordPcut);
cp_t P2ACompactResultRecordPcutOpenBase(Pcut *base);
cp_t P2ACompactResultRecordPcutCloseBase(Pcut *base);
//}}}


cp_t P2ACompactResultRecordChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2ACompactResultRecordChoiceTest(QTEST_ARG);
#endif

