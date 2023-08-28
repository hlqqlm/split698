#ifndef __P2_COMPACT_SET_RESPONSE_H_ 
#define __P2_COMPACT_SET_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2CompactSetResponseChoiceName	"compact_set_response_choice"
#define kP2CompactSetResponseName		"compact_set_response"

// basic information
#define kP2CompactSetResponseChoiceOffset			(0)
#define kP2CompactSetResponseChoiceSize				(1)

#define kP2CompactSetResponseVarOffset				\
	(kP2CompactSetResponseChoiceOffset + kP2CompactSetResponseChoiceSize)
#define kP2CompactSetResponseVarSize(_var_size)		(_var_size)




//{{{ choice
/*
COMPACT-SET-Response∷=CHOICE
{
紧凑设置一个对象属性的响应 [1] CompactSetResponseNormal，
紧凑设置多个对象属性的响应 [2] CompactSetResponseNormalList
}
*/
typedef enum
{
	kP2CompactSetResponseChoiceNormal = 1, // 紧凑设置一个对象属性的响应 [1] CompactSetResponseNormal，
	kP2CompactSetResponseChoiceNormalList = 2, // 紧凑设置多个对象属性的响应 [2] CompactSetResponseNormalList
} P2CompactSetResponseChoiceT;
typedef enum 
{
	kP2CompactSetResponseChoiceIxNormal, // 
	kP2CompactSetResponseChoiceIxNormalList, // 
	kP2CompactSetResponseChoiceNum
} P2CompactSetResponseChoiceIxT;
int P2CompactSetResponseChoice2Ix(uint8_t choice);
uint8_t P2CompactSetResponseChoiceIx2Value(int choice_ix);
cp_t P2CompactSetResponseChoiceIxValid(int choice_ix);
cp_t P2CompactSetResponseChoiceValid(uint8_t choice);

uint8_t P2CompactSetResponseChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2CompactSetResponsePcut;
#define kP2CompactSetResponsePcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2CompactSetResponsePcutOpen(P2CompactSetResponsePcut *m);
cp_t P2CompactSetResponsePcutClose(P2CompactSetResponsePcut *m);
cp_t P2CompactSetResponsePcutValid(const P2CompactSetResponsePcut *m);

cp_t P2CompactSetResponsePcutOpenBase(Pcut *base);
cp_t P2CompactSetResponsePcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactSetResponseTest(QTEST_ARG);
#endif

