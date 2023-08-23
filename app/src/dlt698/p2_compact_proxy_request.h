#ifndef __P2_COMPACT_PROXY_REQUEST_H_ 
#define __P2_COMPACT_PROXY_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2CompactProxyRequestChoiceName	"compact_proxy_request_choice"
#define kP2CompactProxyRequestName			"compact_proxy_request"

// basic information
#define kP2CompactProxyRequestChoiceOffset			(0)
#define kP2CompactProxyRequestChoiceSize			(1)

#define kP2CompactProxyRequestVarOffset				(kP2CompactProxyRequestChoiceOffset + kP2CompactProxyRequestChoiceSize)
#define kP2CompactProxyRequestVarSize(_var_size)	(_var_size)



//{{{ choice
/*
COMPACT-PROXY-Request∷=CHOICE
{
请求紧凑代理读取服务器的若干个对象属性 [1] CompactProxyGetRequestNormal，
请求紧凑代理读取服务器的一个记录型对象属性 [2] CompactProxyGetRequestRecord，
请求紧凑代理设置服务器的若干个对象属性 [3] CompactProxySetRequestNormal，
请求紧凑代理设置后读取服务器的若干个对象属性 [4] CompactProxySetThenGetRequest，
请求紧凑代理操作服务器的若干个对象方法 [5] CompactProxyActionRequest，
请求紧凑代理操作后读取服务器的若干个对象方法和属性 [6] CompactProxyActionThenGetRequest，
}
*/
typedef enum
{
	kP2CompactProxyRequestChoiceGetRequestNormal = 1, // 请求紧凑代理读取服务器的若干个对象属性 [1] CompactProxyGetRequestNormal，
	kP2CompactProxyRequestChoiceGetRequestRecord = 2, // 请求紧凑代理读取服务器的一个记录型对象属性 [2] CompactProxyGetRequestRecord，
	kP2CompactProxyRequestChoiceSetRequestNormal = 3, // 请求紧凑代理设置服务器的若干个对象属性 [3] CompactProxySetRequestNormal，
	kP2CompactProxyRequestChoiceSetThenGetRequest = 4, // 请求紧凑代理设置后读取服务器的若干个对象属性 [4] CompactProxySetThenGetRequest，
	kP2CompactProxyRequestChoiceActionRequest = 5, // 请求紧凑代理操作服务器的若干个对象方法 [5] CompactProxyActionRequest，
	kP2CompactProxyRequestChoiceActionThenGetRequest = 6, // 请求紧凑代理操作后读取服务器的若干个对象方法和属性 [6] CompactProxyActionThenGetRequest，
} P2CompactProxyRequestChoiceT;
typedef enum 
{
	kP2CompactProxyRequestChoiceIxGetRequestNormal, // 
	kP2CompactProxyRequestChoiceIxGetRequestRecord, // 
	kP2CompactProxyRequestChoiceIxSetRequestNormal, // 
	kP2CompactProxyRequestChoiceIxSetThenGetRequest, // 
	kP2CompactProxyRequestChoiceIxActionRequest, // 
	kP2CompactProxyRequestChoiceIxActionThenGetRequest, // 
	kP2CompactProxyRequestChoiceNum
} P2CompactProxyRequestChoiceIxT;
int P2CompactProxyRequestChoice2Ix(uint8_t choice);
uint8_t P2CompactProxyRequestChoiceIx2Value(int choice_ix);
cp_t P2CompactProxyRequestChoiceIxValid(int choice_ix);
cp_t P2CompactProxyRequestChoiceValid(uint8_t choice);

uint8_t P2CompactProxyRequestChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2CompactProxyRequestPcut;
#define kP2CompactProxyRequestPcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2CompactProxyRequestPcutOpen(P2CompactProxyRequestPcut *m);
cp_t P2CompactProxyRequestPcutClose(P2CompactProxyRequestPcut *m);
cp_t P2CompactProxyRequestPcutValid(const P2CompactProxyRequestPcut *m);

cp_t P2CompactProxyRequestPcutOpenBase(Pcut *base);
cp_t P2CompactProxyRequestPcutCloseBase(Pcut *base);
//}}}


// fill
cp_t P2CompactProxyRequestFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2CompactProxyRequestTest(QTEST_ARG);
#endif

