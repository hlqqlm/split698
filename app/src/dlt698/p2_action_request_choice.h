#ifndef __P2_ACTION_REQUEST_CHOICE_H_
#define __P2_ACTION_REQUEST_CHOICE_H_



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#include "p2_choice.h"


#define kP2ActionRequestName				"action_request"
#define kP2ActionRequestNameChoice			"action_request_choice"
#define kP2ActionRequestNameVar				"action_request_var"

#define kP2ActionRequestChoiceOffset			(0)
#define kP2ActionRequestChoiceSize				(1)

#define kP2ActionRequestVarOffset			(kP2ActionRequestChoiceOffset + kP2ActionRequestChoiceSize)
#define kP2ActionRequestVarSize(_var_size)	(_var_size)

//{{{ choice
/*
ACTION-Request∷=CHOICE
{
	请求操作一个对象方法 [1] ActionRequestNormal，
	请求操作若干个对象方法 [2] ActionRequestNormalList，
	请求操作若干个对象方法后读取若干个对象属性 [3] ActionThenGetRequestNormalList
}
*/
typedef enum 
{
	kP2ActionRequestChoiceNormal = 1,		// 请求操作一个对象方法 [1] ActionRequestNormal，
	kP2ActionRequestChoiceNormalList = 2,	// 请求操作若干个对象方法 [2] ActionRequestNormalList，
	kP2ActionRequestChoiceActionThenGetRequestNormalList = 3,	// 请求操作若干个对象方法后读取若干个对象属性 [3] ActionThenGetRequestNormalList
} P2ActionRequestChoiceT;

typedef enum 
{
	kP2ActionRequestChoiceIxNormal,	// 
	kP2ActionRequestChoiceIxNormalList,	// 
	kP2ActionRequestChoiceIxNormalActionThenGetRequestNormalList,	// 
	kP2ActionRequestChoiceNum
} P2ActionRequestChoiceIxT;
int P2ActionRequestChoice2Ix(uint8_t choice);
uint8_t P2ActionRequestChoiceIx2Value(int choice_ix);
cp_t P2ActionRequestChoiceIxValid(int choice_ix);
cp_t P2ActionRequestChoiceValid(uint8_t choice);
const char *P2ActionRequestChoiceStr(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2ActionRequestChoicePcut;
#define kP2ActionRequestChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2ActionRequestChoicePcutOpen(P2ActionRequestChoicePcut *m);
cp_t P2ActionRequestChoicePcutClose(P2ActionRequestChoicePcut *m);
cp_t P2ActionRequestChoicePcutValid(const P2ActionRequestChoicePcut *m);


// 声明用子类base来open/close父类的函数
//PCUT_DECLEAR_OPEN_BASE(P2ActionRequestChoicePcut);
//PCUT_DECLEAR_CLOSE_BASE(P2ActionRequestChoicePcut);
cp_t P2ActionRequestChoicePcutOpenBase(Pcut *base);
cp_t P2ActionRequestChoicePcutCloseBase(Pcut *base);
//}}}


cp_t P2ActionRequestChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2ActionRequestChoiceTest(QTEST_ARG);
#endif

