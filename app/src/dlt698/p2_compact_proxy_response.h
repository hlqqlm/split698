#ifndef __P2_COMPACT_PROXY_RESPONSE_H_ 
#define __P2_COMPACT_PROXY_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2CompactProxyResponseChoiceName	"compact_proxy_response_choice"
#define kP2CompactProxyResponseName			"compact_proxy_response"


// basic information
#define kP2CompactProxyResponseChoiceOffset			(0)
#define kP2CompactProxyResponseChoiceSize				(1)

#define kP2CompactProxyResponseVarOffset				\
	(kP2CompactProxyResponseChoiceOffset + kP2CompactProxyResponseChoiceSize)
#define kP2CompactProxyResponseVarSize(_var_size)		(_var_size)




//{{{ choice
/*
COMPACT-PROXY-Response∷=CHOICE
{
代理精简读取服务器的若干个对象属性的响应 [1] CompactProxyGetResponseNormal，
代理精简读取服务器的一个记录型对象属性的响应 [2] CompactProxyGetResponseRecord，
代理精简设置服务器的若干个对象属性的响应 [3] CompactProxySetResponse，
代理精简设置后读取服务器的若干个对象属性的响应 [4] CompactProxySetThenGetResponse，
代理精简操作服务器的若干个对象方法的响应 [5] CompactProxyActionResponse，
代理精简操作后读取服务器的若干个对象方法和属性的响应 [6] CompactProxyActionThenGetResponse
}
*/
typedef enum
{
	kP2CompactProxyResponseChoiceNormal = 1, // 代理精简读取服务器的若干个对象属性的响应 [1] CompactProxyGetResponseNormal，
	kP2CompactProxyResponseChoiceRecord = 2, // 代理精简读取服务器的一个记录型对象属性的响应 [2] CompactProxyGetResponseRecord，
	kP2CompactProxyResponseChoiceSet = 3, // 代理精简设置服务器的若干个对象属性的响应 [3] CompactProxySetResponse，
	kP2CompactProxyResponseChoiceSetThenGet = 4, // 代理精简设置后读取服务器的若干个对象属性的响应 [4] CompactProxySetThenGetResponse，
	kP2CompactProxyResponseChoiceAction = 5, // 代理精简操作服务器的若干个对象方法的响应 [5] CompactProxyActionResponse，
	kP2CompactProxyResponseChoiceActionThenGet = 6, // 代理精简操作后读取服务器的若干个对象方法和属性的响应 [6] CompactProxyActionThenGetResponse
} P2CompactProxyResponseChoiceT;
typedef enum 
{
	kP2CompactProxyResponseChoiceIxNormal, // 
	kP2CompactProxyResponseChoiceIxRecord, // 
	kP2CompactProxyResponseChoiceIxSet, // 
	kP2CompactProxyResponseChoiceIxSetThenGet, // 
	kP2CompactProxyResponseChoiceIxAction, // 
	kP2CompactProxyResponseChoiceIxActionThenGet, // 
	kP2CompactProxyResponseChoiceNum
} P2CompactProxyResponseChoiceIxT;
int P2CompactProxyResponseChoice2Ix(uint8_t choice);
uint8_t P2CompactProxyResponseChoiceIx2Value(int choice_ix);
cp_t P2CompactProxyResponseChoiceIxValid(int choice_ix);
cp_t P2CompactProxyResponseChoiceValid(uint8_t choice);

uint8_t P2CompactProxyResponseChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2CompactProxyResponsePcut;
#define kP2CompactProxyResponsePcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2CompactProxyResponsePcutOpen(P2CompactProxyResponsePcut *m);
cp_t P2CompactProxyResponsePcutClose(P2CompactProxyResponsePcut *m);
cp_t P2CompactProxyResponsePcutValid(const P2CompactProxyResponsePcut *m);

cp_t P2CompactProxyResponsePcutOpenBase(Pcut *base);
cp_t P2CompactProxyResponsePcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxyResponseTest(QTEST_ARG);
#endif

