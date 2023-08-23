#ifndef __P2_COMPACT_GET_REQUEST_H_ 
#define __P2_COMPACT_GET_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2CompactGetRequestChoiceName		"compact_get_request_choice"
#define kP2CompactGetRequestName			"compact_get_request"

// basic information
#define kP2CompactGetRequestChoiceOffset				(0)
#define kP2CompactGetRequestChoiceSize					(1)

#define kP2CompactGetRequestVarOffset					(kP2CompactGetRequestChoiceOffset + kP2CompactGetRequestChoiceSize)
#define kP2CompactGetRequestVarSize(_var_size)			(_var_size)



//{{{ choice
/*
COMPACT-GET-Request∷=CHOICE
{
请求紧凑读取一个对象属性 [1] GetRequestNormal，
请求紧凑读取多个对象属性 [2] GetRequestNormalList，
请求紧凑读取一个记录型对象属性 [3] GetRequestRecord
}
*/
typedef enum
{
	kP2CompactGetRequestChoiceNormal = 1, // 请求紧凑读取一个对象属性 [1] GetRequestNormal，
	kP2CompactGetRequestChoiceNormalList = 2, // 请求紧凑读取多个对象属性 [2] GetRequestNormalList，
	kP2CompactGetRequestChoiceRecord = 3, // 请求紧凑读取一个记录型对象属性 [3] GetRequestRecord
} P2CompactGetRequestChoiceT;
typedef enum 
{
	kP2CompactGetRequestChoiceIxNormal, // 
	kP2CompactGetRequestChoiceIxNormalList, // 
	kP2CompactGetRequestChoiceIxRecord, // 
	kP2CompactGetRequestChoiceNum
} P2CompactGetRequestChoiceIxT;
int P2CompactGetRequestChoice2Ix(uint8_t choice);
uint8_t P2CompactGetRequestChoiceIx2Value(int choice_ix);
cp_t P2CompactGetRequestChoiceIxValid(int choice_ix);
cp_t P2CompactGetRequestChoiceValid(uint8_t choice);

uint8_t P2CompactGetRequestChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2CompactGetRequestPcut;
#define kP2CompactGetRequestPcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2CompactGetRequestPcutOpen(P2CompactGetRequestPcut *m);
cp_t P2CompactGetRequestPcutClose(P2CompactGetRequestPcut *m);
cp_t P2CompactGetRequestPcutValid(const P2CompactGetRequestPcut *m);

cp_t P2CompactGetRequestPcutOpenBase(Pcut *base);
cp_t P2CompactGetRequestPcutCloseBase(Pcut *base);
//}}}


// fill
cp_t P2CompactGetRequestFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2CompactGetRequestTest(QTEST_ARG);
#endif

