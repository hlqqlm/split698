#ifndef __P2_CONNECT_MECHANISM_INFO_CHOICE_H_
#define __P2_CONNECT_MECHANISM_INFO_CHOICE_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "qdlt698.h"
#include "p2_choice.h"


#define kP2ConnectMechanismInfoChoiceName		"connect_mechanism_info_choice"
#define kP2ConnectMechanismInfoNameNull			"null_security"
#define kP2ConnectMechanismInfoNamePassword		"password_security"
#define kP2ConnectMechanismInfoNameSymmetry		"symmetry_security"
#define kP2ConnectMechanismInfoNameSignature	"signature_security"


#define kP2ConnectMechanismInfoChoiceOffset					(0)
#define kP2ConnectMechanismInfoChoiceSize					(1)

#define kP2ConnectMechanismInfoVarOffset					(kP2ConnectMechanismInfoChoiceOffset + kP2ConnectMechanismInfoChoiceSize)
#define kP2ConnectMechanismInfoVarSize(_var_size)			(_var_size)


//{{{ choice
/*
ConnectMechanismInfo∷=CHOICE
{
	公共连接 [0] NullSecurity，
	一般密码 [1] PasswordSecurity，
	对称加密 [2] SymmetrySecurity，
	数字签名 [3] SignatureSecurity
}
*/
typedef enum 
{
	kP2ConnectMechanismInfoChoiceNull = 0,		// 公共连接 [0] NullSecurity，
	kP2ConnectMechanismInfoChoicePassword = 1,	// 一般密码 [1] PasswordSecurity，
	kP2ConnectMechanismInfoChoiceSymmetry = 2,	// 对称加密 [2] SymmetrySecurity，
	kP2ConnectMechanismInfoChoiceSignature = 3,	// 数字签名 [3] SignatureSecurity
} P2ConnectMechanismInfoChoiceT;

typedef enum 
{
	kP2ConnectMechanismInfoChoiceIxNull,		// 公共连接 [0] NullSecurity，
	kP2ConnectMechanismInfoChoiceIxPasswrod,	// 一般密码 [1] PasswordSecurity，
	kP2ConnectMechanismInfoChoiceIxSymmetry,	// 对称加密 [2] SymmetrySecurity，
	kP2ConnectMechanismInfoChoiceIxSignature,	// 数字签名 [3] SignatureSecurity
	kP2ConnectMechanismInfoChoiceNum
} P2ConnectMechanismInfoChoiceIxT;
int P2ConnectMechanismInfoChoice2Ix(uint8_t choice);
uint8_t P2ConnectMechanismInfoChoiceIx2Value(int choice_ix);
cp_t P2ConnectMechanismInfoChoiceIxValid(int choice_ix);
cp_t P2ConnectMechanismInfoChoiceValid(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2ConnectMechanismInfoChoicePcut;
#define kP2ConnectMechanismInfoChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2ConnectMechanismInfoChoicePcutOpen(P2ConnectMechanismInfoChoicePcut *m);
cp_t P2ConnectMechanismInfoChoicePcutClose(P2ConnectMechanismInfoChoicePcut *m);
cp_t P2ConnectMechanismInfoChoicePcutValid(const P2ConnectMechanismInfoChoicePcut *m);
//}}}


// test
cp_t P2ConnectMechanismInfoChoiceTest(QTEST_ARG);
#endif

