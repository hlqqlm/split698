#ifndef __P2_GET_REQUEST_H_ 
#define __P2_GET_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"


#define kP2GetRequestChoiceOffset					(0)
#define kP2GetRequestChoiceSize(_choice_size)		(_choice_size)

#define kP2GetRequestChoiceName		"get_request_choice"
#define kP2GetRequestName			"get_request"


//{{{ choice
/*
GET-Request∷=CHOICE
{
请求读取一个对象属性 [1] GetRequestNormal，
请求读取若干个对象属性 [2] GetRequestNormalList，
请求读取一个记录型对象属性 [3] GetRequestRecord，
请求读取若干个记录型对象属性 [4] GetRequestRecordList，
请求读取分帧传输的下一帧 [5] GetRequestNext，
请求读取一个对象属性的 MD5 值 [6] GetRequestMD5，
读取一个精简的记录型对象属性请求 [23] GetRequestSimplifyRecord
}
*/
typedef enum
{
	kP2GetRequestChoiceGetRequestNormal = 1,		// 请求读取一个对象属性 [1] GetRequestNormal，
	kP2GetRequestChoiceGetRequestNormalList = 2,	// 请求读取若干个对象属性 [2] GetRequestNormalList，
	kP2GetRequestChoiceGetRequestRecord = 3,		// 请求读取一个记录型对象属性 [3] GetRequestRecord，
	kP2GetRequestChoiceGetRequestRecordList = 4,	// 请求读取若干个记录型对象属性 [4] GetRequestRecordList，
	kP2GetRequestChoiceGetRequestNext = 5,			// 请求读取分帧传输的下一帧 [5] GetRequestNext，
	kP2GetRequestChoiceGetRequestMD5 = 6,			// 请求读取一个对象属性的 MD5 值 [6] GetRequestMD5，
	kP2GetRequestChoiceGetRequestSimplifyRecord = 23,	// 读取一个精简的记录型对象属性请求 [23] GetRequestSimplifyRecord
} P2GetRequestChoiceT;
typedef enum 
{
	kP2GetRequestChoiceIxGetRequestNormal,	// 请求读取一个对象属性 [1] GetRequestNormal，
	kP2GetRequestChoiceIxGetRequestNormalList,	// 请求读取若干个对象属性 [2] GetRequestNormalList，
	kP2GetRequestChoiceIxGetRequestRecord,		// 请求读取一个记录型对象属性 [3] GetRequestRecord，
	kP2GetRequestChoiceIxGetRequestRecordList,	// 请求读取若干个记录型对象属性 [4] GetRequestRecordList，
	kP2GetRequestChoiceIxGetRequestNext,			// 请求读取分帧传输的下一帧 [5] GetRequestNext，
	kP2GetRequestChoiceIxGetRequestMD5,			// 请求读取一个对象属性的 MD5 值 [6] GetRequestMD5，
	kP2GetRequestChoiceIxGetRequestSimplifyRecord,	// 读取一个精简的记录型对象属性请求 [23] GetRequestSimplifyRecord
	kP2GetRequestChoiceNum
} P2GetRequestChoiceIxT;
int P2GetRequestChoice2Ix(uint8_t choice);
uint8_t P2GetRequestChoiceIx2Value(int choice_ix);
cp_t P2GetRequestChoiceIxValid(int choice_ix);
cp_t P2GetRequestChoiceValid(uint8_t choice);

uint8_t P2GetRequestChoice(const char *whole);
//}}}


//{{{ part
typedef struct 
{
	P2ChoicePcut choice;
} P2GetRequestPcut;
#define kP2GetRequestPcutDef {								\
	kP2ChoicePcutDef,										\
}

//cp_t P2GetRequestPcutOpen(P2GetRequestPcut *m);
//cp_t P2GetRequestPcutOpen(PcutBase *base);
cp_t P2GetRequestPcutOpen(P2GetRequestPcut *m);
cp_t P2GetRequestPcutClose(P2GetRequestPcut *m);
cp_t P2GetRequestPcutValid(const P2GetRequestPcut *m);

cp_t P2GetRequestPcutOpenBase(Pcut *base);
cp_t P2GetRequestPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2GetRequestTest(QTEST_ARG);
#endif

