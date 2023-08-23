#ifndef __P2_COMPACT_SET_REQUEST_H_ 
#define __P2_COMPACT_SET_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2CompactSetRequestChoiceName		"compact_set_request_choice"
#define kP2CompactSetRequestName			"compact_set_request"

// basic information
#define kP2CompactSetRequestChoiceOffset				(0)
#define kP2CompactSetRequestChoiceSize					(1)

#define kP2CompactSetRequestVarOffset					(kP2CompactSetRequestChoiceOffset + kP2CompactSetRequestChoiceSize)
#define kP2CompactSetRequestVarSize(_var_size)			(_var_size)



//{{{ choice
/*
COMPACT-SET-Request∷=CHOICE
{
请求紧凑设置一个对象属性 [1] SetRequestNormal，
请求紧凑设置多个对象属性 [2] SetRequestNormalList
}
*/
typedef enum
{
	kP2CompactSetRequestChoiceNormal = 1, // 请求紧凑设置一个对象属性 [1] SetRequestNormal，
	kP2CompactSetRequestChoiceNormalList = 2, // 请求紧凑设置多个对象属性 [2] SetRequestNormalList
} P2CompactSetRequestChoiceT;
typedef enum 
{
	kP2CompactSetRequestChoiceIxNormal, // 
	kP2CompactSetRequestChoiceIxNormalList, // 
	kP2CompactSetRequestChoiceNum
} P2CompactSetRequestChoiceIxT;
int P2CompactSetRequestChoice2Ix(uint8_t choice);
uint8_t P2CompactSetRequestChoiceIx2Value(int choice_ix);
cp_t P2CompactSetRequestChoiceIxValid(int choice_ix);
cp_t P2CompactSetRequestChoiceValid(uint8_t choice);

uint8_t P2CompactSetRequestChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2CompactSetRequestPcut;
#define kP2CompactSetRequestPcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2CompactSetRequestPcutOpen(P2CompactSetRequestPcut *m);
cp_t P2CompactSetRequestPcutClose(P2CompactSetRequestPcut *m);
cp_t P2CompactSetRequestPcutValid(const P2CompactSetRequestPcut *m);

cp_t P2CompactSetRequestPcutOpenBase(Pcut *base);
cp_t P2CompactSetRequestPcutCloseBase(Pcut *base);
//}}}


// fill
cp_t P2CompactSetRequestFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2CompactSetRequestTest(QTEST_ARG);
#endif

