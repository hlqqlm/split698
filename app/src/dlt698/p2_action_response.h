#ifndef __P2_ACTION_RESPONSE_H_ 
#define __P2_ACTION_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2ActionResponseChoiceName	"action_response_choice"
#define kP2ActionResponseName			"action_response"

// basic information
#define kP2ActionResponseChoiceOffset					(0)
#define kP2ActionResponseChoiceSize					(1)

#define kP2ActionResponseVarOffset						(kP2ActionResponseChoiceOffset + kP2ActionResponseChoiceSize)
#define kP2ActionResponseVarSize(_var_size)			(_var_size)



//{{{ choice
/*
ACTION-Response∷=CHOICE
{
操作一个对象方法的响应 [1] ActionResponseNormal，
操作若干个对象方法的响应 [2] ActionResponseNormalList，
操作若干个对象方法后读取若干个属性的响应 [3] ActionThenGetResponseNormalList
}
*/
typedef enum
{
	kP2ActionResponseChoiceNormal = 1, // 操作一个对象方法的响应 [1] ActionResponseNormal，
	kP2ActionResponseChoiceNormalList = 2, // 操作若干个对象方法的响应 [2] ActionResponseNormalList，
	kP2ActionResponseChoiceActionThenGetResponseNormalList = 3, // 操作若干个对象方法后读取若干个属性的响应 [3] ActionThenGetResponseNormalList
} P2ActionResponseChoiceT;
typedef enum 
{
	kP2ActionResponseChoiceIxNormal, // 
	kP2ActionResponseChoiceIxNormalList, // 
	kP2ActionResponseChoiceIxActionThenGetResponseNormalList, // 
	kP2ActionResponseChoiceNum
} P2ActionResponseChoiceIxT;
int P2ActionResponseChoice2Ix(uint8_t choice);
uint8_t P2ActionResponseChoiceIx2Value(int choice_ix);
cp_t P2ActionResponseChoiceIxValid(int choice_ix);
cp_t P2ActionResponseChoiceValid(uint8_t choice);

uint8_t P2ActionResponseChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2ActionResponsePcut;
#define kP2ActionResponsePcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2ActionResponsePcutOpen(P2ActionResponsePcut *m);
cp_t P2ActionResponsePcutClose(P2ActionResponsePcut *m);
cp_t P2ActionResponsePcutValid(const P2ActionResponsePcut *m);

cp_t P2ActionResponsePcutOpenBase(Pcut *base);
cp_t P2ActionResponsePcutCloseBase(Pcut *base);
//}}}


// fill
cp_t P2ActionResponseFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2ActionResponseTest(QTEST_ARG);
#endif

