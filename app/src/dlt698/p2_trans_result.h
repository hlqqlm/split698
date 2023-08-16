#ifndef __P2_TRANS_RESULT_H_
#define __P2_TRANS_RESULT_H_



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#include "p2_choice.h"


#define kP2TransResultName					"trans_result"
#define kP2TransResultNameChoice			"trans_result_choice"
#define kP2TransResultNameVar				"trans_result_var"

#define kP2TransResultOffset			(0)
#define kP2TransResultSize			(1)

#define kP2TransResultVarOffset			(kP2TransResultOffset + kP2TransResultSize)
#define kP2TransResultVarSize(_var_size)	(_var_size)

//{{{ choice
/*
TransResult∷=CHOICE
{
 转发结果 [0] DAR，
  返回数据 [1] octet-string
  }
*/
#define kP2TransResultChoiceNameDar				"dar"
#define kP2TransResultChoiceNameReturnData		"return_data"

typedef enum 
{
	kP2TransResultChoiceDar = 0,		// 转发结果 [0] DAR，
	kP2TransResultChoiceReturnData = 1,		// 返回数据 [1] octet-string
} P2TransResultChoiceT;

typedef enum 
{
	kP2TransResultChoiceIxDar,			// 
	kP2TransResultChoiceIxReturnData,	// 
	kP2TransResultChoiceNum
} P2TransResultChoiceIxT;
int P2TransResultChoice2Ix(uint8_t choice);
uint8_t P2TransResultChoiceIx2Value(int choice_ix);
cp_t P2TransResultChoiceIxValid(int choice_ix);
cp_t P2TransResultChoiceValid(uint8_t choice);
const char *P2TransResultChoiceStr(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2TransResultPcut;
#define kP2TransResultPcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2TransResultPcutOpen(P2TransResultPcut *m);
cp_t P2TransResultPcutClose(P2TransResultPcut *m);
cp_t P2TransResultPcutValid(const P2TransResultPcut *m);


// 声明用子类base来open/close父类的函数
cp_t P2TransResultPcutOpenBase(Pcut *base);
cp_t P2TransResultPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2TransResultTest(QTEST_ARG);
#endif

