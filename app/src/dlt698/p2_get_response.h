#ifndef __DLT698_45_GET_RESPONSE_H_ 
#define __DLT698_45_GET_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2GetResponseChoiceName	"get_response_choice"
#define kP2GetResponseName			"get_response"

// basic information
#define kP2GetResponseChoiceOffset					(0)
#define kP2GetResponseChoiceSize					(1)

#define kP2GetResponseVarOffset						(kP2GetResponseChoiceOffset + kP2GetResponseChoiceSize)
#define kP2GetResponseVarSize(_var_size)			(_var_size)




//{{{ choice
/*
GET-Response∷=CHOICE
{
读取一个对象属性的响应 [1] GetResponseNormal，
读取若干个对象属性的响应 [2] GetResponseNormalList，
读取一个记录型对象属性的响应 [3] GetResponseRecord，
读取若干个记录型对象属性的响应 [4] GetResponseRecordList，
读取分帧传输的下一帧的响应 [5] GetResponseNext，
读取一个对象属性的 MD5 值的响应 [6] GetResponseMD5，
读取一个精简记录型对象属性请求 [23] GetResponseSimplifyRecord
}
*/
typedef enum
{
	kP2GetResponseChoiceGetResponseNormal = 1, // 读取一个对象属性的响应 [1] GetResponseNormal，
	kP2GetResponseChoiceGetResponseNormalList = 2, // 读取若干个对象属性的响应 [2] GetResponseNormalList，
	kP2GetResponseChoiceGetResponseRecord = 3, // 读取一个记录型对象属性的响应 [3] GetResponseRecord，
	kP2GetResponseChoiceGetResponseRecordList = 4, // 读取若干个记录型对象属性的响应 [4] GetResponseRecordList，
	kP2GetResponseChoiceGetResponseNext = 5, // 读取分帧传输的下一帧的响应 [5] GetResponseNext，
	kP2GetResponseChoiceGetResponseMD5 = 6, // 读取一个对象属性的 MD5 值的响应 [6] GetResponseMD5，
	kP2GetResponseChoiceGetResponseSimplifyRecord = 23, // 读取一个精简记录型对象属性请求 [23] GetResponseSimplifyRecord
} P2GetResponseChoiceT;
typedef enum 
{
	kP2GetResponseChoiceIxGetResponseNormal, // 读取一个对象属性的响应 [1] GetResponseNormal，
	kP2GetResponseChoiceIxGetResponseNormalList, // 读取若干个对象属性的响应 [2] GetResponseNormalList，
	kP2GetResponseChoiceIxGetResponseRecord, // 读取一个记录型对象属性的响应 [3] GetResponseRecord，
	kP2GetResponseChoiceIxGetResponseRecordList, // 读取若干个记录型对象属性的响应 [4] GetResponseRecordList，
	kP2GetResponseChoiceIxGetResponseNext, // 读取分帧传输的下一帧的响应 [5] GetResponseNext，
	kP2GetResponseChoiceIxGetResponseMD5, // 读取一个对象属性的 MD5 值的响应 [6] GetResponseMD5，
	kP2GetResponseChoiceIxGetResponseSimplifyRecord, // 读取一个精简记录型对象属性请求 [23] GetResponseSimplifyRecord
	kP2GetResponseChoiceNum
} P2GetResponseChoiceIxT;
int P2GetResponseChoice2Ix(uint8_t choice);
uint8_t P2GetResponseChoiceIx2Value(int choice_ix);
cp_t P2GetResponseChoiceIxValid(int choice_ix);
cp_t P2GetResponseChoiceValid(uint8_t choice);

uint8_t P2GetResponseChoice(const char *whole);
//}}}


//{{{ part
typedef struct 
{
	P2ChoicePcut choice;
} P2GetResponsePcut;
#define kP2GetResponsePcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2GetResponsePcutOpen(P2GetResponsePcut *m);
cp_t P2GetResponsePcutClose(P2GetResponsePcut *m);
cp_t P2GetResponsePcutValid(const P2GetResponsePcut *m);

cp_t P2GetResponsePcutOpenBase(Pcut *base);
cp_t P2GetResponsePcutCloseBase(Pcut *base);
//}}}


// fill
cp_t P2GetResponseFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2GetResponseTest(QTEST_ARG);
#endif

