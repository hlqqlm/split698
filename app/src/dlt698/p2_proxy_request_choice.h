#ifndef __P2_PROXY_REQUEST_CHOICE_H_
#define __P2_PROXY_REQUEST_CHOICE_H_



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#include "p2_choice.h"


#define kP2ProxyRequestName					"proxy_request"
#define kP2ProxyRequestNameChoice			"proxy_request_choice"
#define kP2ProxyRequestNameVar				"proxy_request_var"

#define kP2ProxyRequestChoiceOffset			(0)
#define kP2ProxyRequestChoiceSize			(1)

#define kP2ProxyRequestVarOffset			(kP2ProxyRequestChoiceOffset + kP2ProxyRequestChoiceSize)
#define kP2ProxyRequestVarSize(_var_size)	(_var_size)

//{{{ choice
/*
PROXY
Request ∷∷=CHOICE
{
请求代理读取若干个服务器的若干个对象属性 [1] ProxyGetRequestList
请求代理读取一个服务器的一个记录型对象属性 [2] ProxyGetRequestRecord
请求代理设置若干个服务器的若干个对象属性 [3] ProxySetRequestList
请求代理设置后读取若干个服务器的若干个对象属性 [4] ProxySetThenGetRequestList
请求代理操作若干个服务器的若干个对象方法 [5] ProxyActionRequestList
请求代理操作后读取若干个服务器的若干个对象方法和属性 [6] ProxyActionThenGetRequestList
请求代理透明转发 命令 [7] ProxyTransCommandRequest
请求代理终端内部转发命令 [8] ProxyInnerTransCommandRequest
}
*/
typedef enum 
{
	kP2ProxyRequestChoiceGetRequestList = 1,		// 请求代理读取若干个服务器的若干个对象属性 [1] ProxyGetRequestList
	kP2ProxyRequestChoiceGetRequestRecord = 2,		// 请求代理读取一个服务器的一个记录型对象属性 [2] ProxyGetRequestRecord
	kP2ProxyRequestChoiceSetRequestList = 3,		// 请求代理设置若干个服务器的若干个对象属性 [3] ProxySetRequestList
	kP2ProxyRequestChoiceSetThenGetRequestList = 4,		// 请求代理设置后读取若干个服务器的若干个对象属性 [4] ProxySetThenGetRequestList
	kP2ProxyRequestChoiceActionRequestList = 5,		// 请求代理操作若干个服务器的若干个对象方法 [5] ProxyActionRequestList
	kP2ProxyRequestChoiceActionThenGetRequestList = 6,		// 请求代理操作后读取若干个服务器的若干个对象方法和属性 [6] ProxyActionThenGetRequestList
	kP2ProxyRequestChoiceTransCommandRequest = 7,		// 请求代理透明转发 命令 [7] ProxyTransCommandRequest
	kP2ProxyRequestChoiceInnerTransCommandRequest = 8,		// 请求代理终端内部转发命令 [8] ProxyInnerTransCommandRequest
} P2ProxyRequestChoiceT;

typedef enum 
{
	kP2ProxyRequestChoiceIxGetRequestList,	// 
	kP2ProxyRequestChoiceIxGetRequestRecord,	// 
	kP2ProxyRequestChoiceIxSetRequestList,	// 
	kP2ProxyRequestChoiceIxSetThenGetRequestList,	// 
	kP2ProxyRequestChoiceIxActionRequestList,	// 
	kP2ProxyRequestChoiceIxActionThenGetRequestList,	// 
	kP2ProxyRequestChoiceIxTransCommandRequest,	// 
	kP2ProxyRequestChoiceIxInnerTransCommandRequest,	// 
	kP2ProxyRequestChoiceNum
} P2ProxyRequestChoiceIxT;
int P2ProxyRequestChoice2Ix(uint8_t choice);
uint8_t P2ProxyRequestChoiceIx2Value(int choice_ix);
cp_t P2ProxyRequestChoiceIxValid(int choice_ix);
cp_t P2ProxyRequestChoiceValid(uint8_t choice);
const char *P2ProxyRequestChoiceStr(uint8_t choice);
//}}}


//{{{ part
typedef struct
{
	P2ChoicePcut choice;
} P2ProxyRequestChoicePcut;
#define kP2ProxyRequestChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2ProxyRequestChoicePcutOpen(P2ProxyRequestChoicePcut *m);
cp_t P2ProxyRequestChoicePcutClose(P2ProxyRequestChoicePcut *m);
cp_t P2ProxyRequestChoicePcutValid(const P2ProxyRequestChoicePcut *m);


// 声明用子类base来open/close父类的函数
cp_t P2ProxyRequestChoicePcutOpenBase(Pcut *base);
cp_t P2ProxyRequestChoicePcutCloseBase(Pcut *base);
//}}}


cp_t P2ProxyRequestChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2ProxyRequestChoiceTest(QTEST_ARG);
#endif

