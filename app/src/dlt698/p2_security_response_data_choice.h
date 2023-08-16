#ifndef __P2_SECURITY_RESPONSE_DATA_CHOICE_H_ 
#define __P2_SECURITY_RESPONSE_DATA_CHOICE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "qdlt698.h"
#include "p2_choice.h"


#define kP2SecurityResponseDataChoiceName			"security_response_data_choice"
#define kP2SecurityResponseDataNamePlain			"plain"
#define kP2SecurityResponseDataNameCipher			"cipher"
#define kP2SecurityResponseDataNameDar				"dar"


#define kP2SecurityResponseDataChoiceOffset					(0)
#define kP2SecurityResponseDataChoiceSize					(1)

#define kP2SecurityResponseDataVarOffset					(kP2SecurityResponseDataChoiceOffset + kP2SecurityResponseDataChoiceSize)
#define kP2SecurityResponseDataVarSize(_var_size)			(_var_size)


//{{{ choice
/*
应用数据单元 CHOICE
{
明文数据单元 [0] octet-string，
密文数据单元 [1] octet-string，
安全传输错误码 [2] DAR
}
*/
#define kP2SecurityResponseDataChoiceInvalid		(0xff)
typedef enum 
{
	kP2SecurityResponseDataChoicePlain = 0,		// 明文
	kP2SecurityResponseDataChoiceCipher = 1,	// 密文
	kP2SecurityResponseDataChoiceDar = 2,		// DAR
} P2SecurityResponseDataChoiceT;

typedef enum 
{
	kP2SecurityResponseDataChoiceIxPlain,	// 明文
	kP2SecurityResponseDataChoiceIxCipher,	// 密文
	kP2SecurityResponseDataChoiceIxDar,		// DAR
	kP2SecurityResponseDataChoiceNum
} P2SecurityResponseDataChoiceIxT;
int P2SecurityResponseDataChoice2Ix(uint8_t choice);
uint8_t P2SecurityResponseDataChoiceIx2Value(int choice_ix);
cp_t P2SecurityResponseDataChoiceIxValid(int choice_ix);
cp_t P2SecurityResponseDataChoiceValid(uint8_t choice);
//}}}


//{{{ part
typedef struct
{
	P2ChoicePcut choice;
} P2SecurityResponseDataChoicePcut;
#define kP2SecurityResponseDataChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2SecurityResponseDataChoicePcutOpen(P2SecurityResponseDataChoicePcut *m);
cp_t P2SecurityResponseDataChoicePcutClose(P2SecurityResponseDataChoicePcut *m);
cp_t P2SecurityResponseDataChoicePcutValid(const P2SecurityResponseDataChoicePcut *m);
//}}}


// pfill
// fill_octet_string		当choice=0/1时，octet_string不能为NULL
cp_t P2SecurityResponseDataChoiceFillInit(Pfill *m, uint8_t choice, P2DarT dar, Pfill *fill_octet_string);

// test
cp_t P2SecurityResponseDataChoiceTest(QTEST_ARG);
#endif

