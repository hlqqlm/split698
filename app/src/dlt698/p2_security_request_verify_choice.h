#ifndef __P2_SECURITY_REQUEST_VERIFY_CHOICE_H_ 
#define __P2_SECURITY_REQUEST_VERIFY_CHOICE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_choice.h"


#define P2_SECURITY_REQUEST_VERIFY_CHOICE_NAME			"security_request_verify_choice"

//{{{ choice
/*
数据验证信息 CHOICE
{
数据验证码 [0] SID_MAC，
随机数 [1] RN，
随机数+MAC [2] RN_MAC，
安全标识 [3] SID
}
*/
#define kP2SecurityRequestVerifyChoiceInvalid		(0xff)
typedef enum 
{
	kP2SecurityRequestVerifyChoiceSidMac = 0,	// 数据验证码 [0] SID_MAC，
	kP2SecurityRequestVerifyChoiceRn = 1,		// 随机数 [1] RN，
	kP2SecurityRequestVerifyChoiceRnMac = 2,	// 随机数+MAC [2] RN_MAC，
	kP2SecurityRequestVerifyChoiceSid = 3,		// 安全标识 [3] SID
} P2SecurityRequestVerifyChoiceT;

typedef enum 
{
	kP2SecurityRequestVerifyChoiceIxSidMac,	// 数据验证码 [0] SID_MAC，
	kP2SecurityRequestVerifyChoiceIxRn,		// 随机数 [1] RN，
	kP2SecurityRequestVerifyChoiceIxRnMac,	// 随机数+MAC [2] RN_MAC，
	kP2SecurityRequestVerifyChoiceIxSid,	// 安全标识 [3] SID
	kP2SecurityRequestVerifyChoiceNum
} P2SecurityRequestVerifyChoiceIxT;
int P2SecurityRequestVerifyChoice2Ix(uint8_t choice);
uint8_t P2SecurityRequestVerifyChoiceIx2Value(int choice_ix);
cp_t P2SecurityRequestVerifyChoiceIxValid(int choice_ix);
cp_t P2SecurityRequestVerifyChoiceValid(uint8_t choice);
//}}}


//{{{ part
typedef struct
{
	P2ChoicePcut choice;
} P2SecurityRequestVerifyChoicePcut;
#define kP2SecurityRequestVerifyChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2SecurityRequestVerifyChoicePcutOpen(P2SecurityRequestVerifyChoicePcut *m);
cp_t P2SecurityRequestVerifyChoicePcutClose(P2SecurityRequestVerifyChoicePcut *m);
cp_t P2SecurityRequestVerifyChoicePcutValid(const P2SecurityRequestVerifyChoicePcut *m);
//}}}

#if 0
//{{{ qpack
typedef struct
{
	P2ChoiceQpack base;
} P2SecurityRequestVerifyChoiceQpack;
#define kP2SecurityRequestVerifyChoiceQpackDef {	\
	kP2ChoiceQpackDef,					\
}
cp_t P2SecurityRequestVerifyChoiceQpackOpen(P2SecurityRequestVerifyChoiceQpack *m);
cp_t P2SecurityRequestVerifyChoiceQpackClose(P2SecurityRequestVerifyChoiceQpack *m);
cp_t P2SecurityRequestVerifyChoiceQpackValid(const P2SecurityRequestVerifyChoiceQpack *m);
//}}}
#endif

// test
cp_t P2SecurityRequestVerifyChoiceTest(QTEST_ARG);
#endif

