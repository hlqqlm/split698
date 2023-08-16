#ifndef __P2_LINK_APDU_CHOICE_H_ 
#define __P2_LINK_APDU_CHOICE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_choice.h"


#define kP2LinkApduChoiceName			"link_apdu_choice"

//{{{ choice
/*
LINK-APDU∷=CHOICE
{
预连接请求 [1] LINK-Request，
预连接响应 [129] LINK-Response
}
*/
#define kP2LinkApduChoiceInvalid		(0)
typedef enum 
{
	kP2LinkApduChoiceRequest = 1,		// 预连接请求 [1] LINK-Request，
	kP2LinkApduChoiceResponse = 129,	// 预连接响应 [129] LINK-Response
} P2LinkApduChoiceT;

typedef enum 
{
	kP2LinkApduChoiceIxRequest,			// 预连接请求 [1] LINK-Request，
	kP2LinkApduChoiceIxResponse,		// 预连接响应 [129] LINK-Response
	kP2LinkApduChoiceNum
} P2LinkApduChoiceIxT;
int P2LinkApduChoice2Ix(uint8_t choice);
uint8_t P2LinkApduChoiceIx2Value(int choice_ix);
cp_t P2LinkApduChoiceIxValid(int choice_ix);
cp_t P2LinkApduChoiceValid(uint8_t choice);
const char *P2LinkApduChoiceStr(uint8_t choice);
//}}}


//{{{ part
typedef struct
{
	P2ChoicePcut choice;
} P2LinkApduChoicePcut;
#define kP2LinkApduChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2LinkApduChoicePcutOpen(P2LinkApduChoicePcut *m);
cp_t P2LinkApduChoicePcutClose(P2LinkApduChoicePcut *m);
cp_t P2LinkApduChoicePcutValid(const P2LinkApduChoicePcut *m);
//}}}


// test
cp_t P2LinkApduChoiceTest(QTEST_ARG);
#endif

