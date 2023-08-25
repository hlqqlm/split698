#ifndef __P2_SECURITY_APDU_CHOICE_H_ 
#define __P2_SECURITY_APDU_CHOICE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#include "p2_choice.h"


#define kP2SecurityApduName					"security_apdu"
#define kP2SecurityApduNameChoice			"security_apdu_choice"
#define kP2SecurityApduNameVar				"security_apdu_var"

#define kP2SecurityApduChoiceOffset			(0)
#define kP2SecurityApduChoiceSize			(1)

#define kP2SecurityApduVarOffset			(kP2SecurityApduChoiceOffset + kP2SecurityApduChoiceSize)
#define kP2SecurityApduVarSize(_var_size)	(_var_size)

//{{{ choice
/*
SECURITY-APDU∷=CHOICE
{
 安全请求 [16] SECURITY-Request，
安全响应 [144] SECURITY-Response
}
*/
#define kP2SecurityApduChoiceInvalid		(0)
typedef enum 
{
	kP2SecurityApduChoiceRequest = 16,		// 安全请求 [16] SECURITY-Request，
	kP2SecurityApduChoiceResponse = 144,	// 安全响应 [144] SECURITY-Response
} P2SecurityApduChoiceT;

typedef enum 
{
	kP2SecurityApduChoiceIxRequest,	// 
	kP2SecurityApduChoiceIxResponse,	// 
	kP2SecurityApduChoiceNum
} P2SecurityApduChoiceIxT;
int P2SecurityApduChoice2Ix(uint8_t choice);
uint8_t P2SecurityApduChoiceIx2Value(int choice_ix);
cp_t P2SecurityApduChoiceIxValid(int choice_ix);
cp_t P2SecurityApduChoiceValid(uint8_t choice);
const char *P2SecurityApduChoiceStr(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2SecurityApduChoicePcut;
#define kP2SecurityApduChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2SecurityApduChoicePcutOpen(P2SecurityApduChoicePcut *m);
cp_t P2SecurityApduChoicePcutClose(P2SecurityApduChoicePcut *m);
cp_t P2SecurityApduChoicePcutValid(const P2SecurityApduChoicePcut *m);
//}}}

#if 0
//{{{ qpack
typedef struct
{
	P2ChoiceQpack base;
} P2SecurityApduChoiceQpack;
#define kP2SecurityApduChoiceQpackDef {	\
	kP2ChoiceQpackDef,					\
}
cp_t P2SecurityApduChoiceQpackOpen(P2SecurityApduChoiceQpack *m);
cp_t P2SecurityApduChoiceQpackClose(P2SecurityApduChoiceQpack *m);
cp_t P2SecurityApduChoiceQpackValid(const P2SecurityApduChoiceQpack *m);
//}}}
#endif


cp_t P2SecurityApduChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2SecurityApduChoiceTest(QTEST_ARG);
#endif

