#ifndef __P2_SET_RESPONSE_H_ 
#define __P2_SET_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2SetResponseChoiceName	"set_response_choice"
#define kP2SetResponseName			"set_response"

// basic information
#define kP2SetResponseChoiceOffset					(0)
#define kP2SetResponseChoiceSize					(1)

#define kP2SetResponseVarOffset						(kP2SetResponseChoiceOffset + kP2SetResponseChoiceSize)
#define kP2SetResponseVarSize(_var_size)			(_var_size)



//{{{ choice
/*
GET-Response∷=CHOICE
{
设置一个对象属性的响应 [1] SetResponseNormal，
设置若干个对象属性的响应 [2] SetResponseNormalList，
设置的确认信息以及读取的响应 [3] SetThenGetResponseNormalList
}
*/
typedef enum
{
	kP2SetResponseChoiceNormal = 1, // 设置一个对象属性的响应 [1] SetResponseNormal，
	kP2SetResponseChoiceNormalList = 2, // 设置若干个对象属性的响应 [2] SetResponseNormalList，
	kP2SetResponseChoiceSetThenGetResponseNormalList = 3, // 设置的确认信息以及读取的响应 [3] SetThenGetResponseNormalList
} P2SetResponseChoiceT;
typedef enum 
{
	kP2SetResponseChoiceIxNormal, // 
	kP2SetResponseChoiceIxNormalList, // 
	kP2SetResponseChoiceIxSetThenGetResponseNormalList, // 
	kP2SetResponseChoiceNum
} P2SetResponseChoiceIxT;
int P2SetResponseChoice2Ix(uint8_t choice);
uint8_t P2SetResponseChoiceIx2Value(int choice_ix);
cp_t P2SetResponseChoiceIxValid(int choice_ix);
cp_t P2SetResponseChoiceValid(uint8_t choice);

uint8_t P2SetResponseChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2SetResponsePcut;
#define kP2SetResponsePcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2SetResponsePcutOpen(P2SetResponsePcut *m);
cp_t P2SetResponsePcutClose(P2SetResponsePcut *m);
cp_t P2SetResponsePcutValid(const P2SetResponsePcut *m);

cp_t P2SetResponsePcutOpenBase(Pcut *base);
cp_t P2SetResponsePcutCloseBase(Pcut *base);
//}}}


// fill
cp_t P2SetResponseFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2SetResponseTest(QTEST_ARG);
#endif

