#ifndef __P2_PROXY_RESPONSE_CHOICE_H_
#define __P2_PROXY_RESPONSE_CHOICE_H_



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#include "p2_choice.h"


#define kP2ProxyResponseName					"proxy_request"
#define kP2ProxyResponseNameChoice			"proxy_request_choice"
#define kP2ProxyResponseNameVar				"proxy_request_var"

#define kP2ProxyResponseChoiceOffset			(0)
#define kP2ProxyResponseChoiceSize			(1)

#define kP2ProxyResponseVarOffset			(kP2ProxyResponseChoiceOffset + kP2ProxyResponseChoiceSize)
#define kP2ProxyResponseVarSize(_var_size)	(_var_size)

//{{{ choice
/*
PROXY-Response∷=CHOICE
{
代理读取若干个服务器的若干个对象属性的响应 [1] ProxyGetResponseList，
代理读取一个服务器的一个记录型对象属性的响应 [2] ProxyGetResponseRecord，
代理设置若干个服务器的若干个对象属性的响应 [3] ProxySetResponseList，
代理设置后读取若干个服务器的若干个对象属性的响应 [4] ProxySetThenGetResponseList，
代理操作若干个服务器的若干个对象方法的响应 [5] ProxyActionResponseList，
代理操作后读取若干个服务器的若干个对象方法和属性的响应 [6] ProxyActionThenGetResponseList，
代理透明转发命令的响应 [7] ProxyTransCommandResponse
代理终端内部转发命令的响应 [8] ProxyInnerTransCommandResponse
}
*/
typedef enum 
{
	kP2ProxyResponseChoiceGetResponseList = 1,		// 代理读取若干个服务器的若干个对象属性的响应 [1] ProxyGetResponseList，
	kP2ProxyResponseChoiceGetResponseRecord = 2,		// 代理读取一个服务器的一个记录型对象属性的响应 [2] ProxyGetResponseRecord，
	kP2ProxyResponseChoiceSetResponseList = 3,		// 代理设置若干个服务器的若干个对象属性的响应 [3] ProxySetResponseList，
	kP2ProxyResponseChoiceSetThenGetResponseList = 4,		// 代理设置后读取若干个服务器的若干个对象属性的响应 [4] ProxySetThenGetResponseList，
	kP2ProxyResponseChoiceActionResponseList = 5,		// 代理操作若干个服务器的若干个对象方法的响应 [5] ProxyActionResponseList，
	kP2ProxyResponseChoiceActionThenGetResponseList = 6,		// 代理操作后读取若干个服务器的若干个对象方法和属性的响应 [6] ProxyActionThenGetResponseList，
	kP2ProxyResponseChoiceTransCommandResponse = 7,		// 代理透明转发命令的响应 [7] ProxyTransCommandResponse
	kP2ProxyResponseChoiceInnerTransCommandResponse = 8,		// 代理终端内部转发命令的响应 [8] ProxyInnerTransCommandResponse
} P2ProxyResponseChoiceT;

typedef enum 
{
	kP2ProxyResponseChoiceIxGetResponseList,	// 
	kP2ProxyResponseChoiceIxGetResponseRecord,	// 
	kP2ProxyResponseChoiceIxSetResponseList,	// 
	kP2ProxyResponseChoiceIxSetThenGetResponseList,	// 
	kP2ProxyResponseChoiceIxActionResponseList,	// 
	kP2ProxyResponseChoiceIxActionThenGetResponseList,	// 
	kP2ProxyResponseChoiceIxTransCommandResponse,	// 
	kP2ProxyResponseChoiceIxInnerTransCommandResponse,	// 
	kP2ProxyResponseChoiceNum
} P2ProxyResponseChoiceIxT;
int P2ProxyResponseChoice2Ix(uint8_t choice);
uint8_t P2ProxyResponseChoiceIx2Value(int choice_ix);
cp_t P2ProxyResponseChoiceIxValid(int choice_ix);
cp_t P2ProxyResponseChoiceValid(uint8_t choice);
const char *P2ProxyResponseChoiceStr(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2ProxyResponseChoicePcut;
#define kP2ProxyResponseChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2ProxyResponseChoicePcutOpen(P2ProxyResponseChoicePcut *m);
cp_t P2ProxyResponseChoicePcutClose(P2ProxyResponseChoicePcut *m);
cp_t P2ProxyResponseChoicePcutValid(const P2ProxyResponseChoicePcut *m);


// 声明用子类base来open/close父类的函数
cp_t P2ProxyResponseChoicePcutOpenBase(Pcut *base);
cp_t P2ProxyResponseChoicePcutCloseBase(Pcut *base);
//}}}


cp_t P2ProxyResponseChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2ProxyResponseChoiceTest(QTEST_ARG);
#endif

