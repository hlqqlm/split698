#ifndef __P2_COMPACT_GET_RESPONSE_H_ 
#define __P2_COMPACT_GET_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2CompactGetResponseChoiceName	"compact_get_response_choice"
#define kP2CompactGetResponseName		"compact_get_response"

// basic information
#define kP2CompactGetResponseChoiceOffset			(0)
#define kP2CompactGetResponseChoiceSize				(1)

#define kP2CompactGetResponseVarOffset				\
	(kP2CompactGetResponseChoiceOffset + kP2CompactGetResponseChoiceSize)
#define kP2CompactGetResponseVarSize(_var_size)		(_var_size)




//{{{ choice
/*
   COMPACT-GET-Response∷=CHOICE
   {
   紧凑读取一个对象属性的响应 [1] CompactGetResponseNormal，
   紧凑读取多个对象属性的响应 [2] CompactGetResponseNormalList，
   紧凑读取一个记录型对象属性的响应 [3] CompactGetResponseRecord
   }
*/
typedef enum
{
	kP2CompactGetResponseChoiceNormal = 1, // 紧凑读取一个对象属性的响应 [1] CompactGetResponseNormal，
	kP2CompactGetResponseChoiceNormalList = 2, // 紧凑读取多个对象属性的响应 [2] CompactGetResponseNormalList，
	kP2CompactGetResponseChoiceRecord = 3, // 紧凑读取一个记录型对象属性的响应 [3] CompactGetResponseRecord
} P2CompactGetResponseChoiceT;
typedef enum 
{
	kP2CompactGetResponseChoiceIxNormal, // 
	kP2CompactGetResponseChoiceIxNormalList, // 
	kP2CompactGetResponseChoiceIxRecord, // 
	kP2CompactGetResponseChoiceNum
} P2CompactGetResponseChoiceIxT;
int P2CompactGetResponseChoice2Ix(uint8_t choice);
uint8_t P2CompactGetResponseChoiceIx2Value(int choice_ix);
cp_t P2CompactGetResponseChoiceIxValid(int choice_ix);
cp_t P2CompactGetResponseChoiceValid(uint8_t choice);

uint8_t P2CompactGetResponseChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2CompactGetResponsePcut;
#define kP2CompactGetResponsePcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2CompactGetResponsePcutOpen(P2CompactGetResponsePcut *m);
cp_t P2CompactGetResponsePcutClose(P2CompactGetResponsePcut *m);
cp_t P2CompactGetResponsePcutValid(const P2CompactGetResponsePcut *m);

cp_t P2CompactGetResponsePcutOpenBase(Pcut *base);
cp_t P2CompactGetResponsePcutCloseBase(Pcut *base);
//}}}


// fill
cp_t P2CompactGetResponseFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2CompactGetResponseTest(QTEST_ARG);
#endif

