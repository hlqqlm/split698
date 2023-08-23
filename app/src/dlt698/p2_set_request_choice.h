#ifndef __P2_SET_REQUEST_CHOICE_H_
#define __P2_SET_REQUEST_CHOICE_H_



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#include "p2_choice.h"


#define kP2SetRequestName				"set_request"
#define kP2SetRequestNameChoice			"set_request_choice"
#define kP2SetRequestNameVar			"set_request_var"

#define kP2SetRequestChoiceOffset			(0)
#define kP2SetRequestChoiceSize				(1)

#define kP2SetRequestVarOffset			(kP2SetRequestChoiceOffset + kP2SetRequestChoiceSize)
#define kP2SetRequestVarSize(_var_size)	(_var_size)

//{{{ choice
/*
SET
Request ∷∷=CHOICE
{
	请求设置一个对象属性 [1] SetRequestNormal
	请求设置 若干个 对象属性 [2] SetRequestNormalList
	请求设置后读取 若干个 对象属性 [3 ] SetTh enGetRequestNormalList
}
*/
typedef enum 
{
	kP2SetRequestChoiceNormal = 1,		// 请求操作一个对象方法 [1] SetRequestNormal，
	kP2SetRequestChoiceNormalList = 2,	// 请求操作若干个对象方法 [2] SetRequestNormalList，
	kP2SetRequestChoiceSetThenGetRequestNormalList = 3,	// 请求操作若干个对象方法后读取若干个对象属性 [3] SetThenGetRequestNormalList
} P2SetRequestChoiceT;

typedef enum 
{
	kP2SetRequestChoiceIxNormal,	// 
	kP2SetRequestChoiceIxNormalList,	// 
	kP2SetRequestChoiceIxNormalSetThenGetRequestNormalList,	// 
	kP2SetRequestChoiceNum
} P2SetRequestChoiceIxT;
int P2SetRequestChoice2Ix(uint8_t choice);
uint8_t P2SetRequestChoiceIx2Value(int choice_ix);
cp_t P2SetRequestChoiceIxValid(int choice_ix);
cp_t P2SetRequestChoiceValid(uint8_t choice);
const char *P2SetRequestChoiceStr(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2SetRequestChoicePcut;
#define kP2SetRequestChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2SetRequestChoicePcutOpen(P2SetRequestChoicePcut *m);
cp_t P2SetRequestChoicePcutClose(P2SetRequestChoicePcut *m);
cp_t P2SetRequestChoicePcutValid(const P2SetRequestChoicePcut *m);


// 声明用子类base来open/close父类的函数
//PCUT_DECLEAR_OPEN_BASE(P2SetRequestChoicePcut);
//PCUT_DECLEAR_CLOSE_BASE(P2SetRequestChoicePcut);
cp_t P2SetRequestChoicePcutOpenBase(Pcut *base);
cp_t P2SetRequestChoicePcutCloseBase(Pcut *base);
//}}}


cp_t P2SetRequestChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2SetRequestChoiceTest(QTEST_ARG);
#endif

