#ifndef __DLT698_45_SECURITY_REQUEST_DATA_CHOICE_H_ 
#define __DLT698_45_SECURITY_REQUEST_DATA_CHOICE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_choice.h"


#define kP2SecurityRequestDataChoiceName			"security_request_data_choice"
#define kP2SecurityRequestDataNamePlain				"plain"
#define kP2SecurityRequestDataNameCipher			"cipher"

//{{{ choice
/*
应用数据单元 CHOICE
{
明文数据单元 [0] octet-string，
密文数据单元 [1] octet-string
}，
*/
#define kP2SecurityRequestDataChoiceInvalid		(0xff)
typedef enum 
{
	kP2SecurityRequestDataChoicePlain = 0,	// 明文
	kP2SecurityRequestDataChoiceCipher = 1,	// 密文
} P2SecurityRequestDataChoiceT;

typedef enum 
{
	kP2SecurityRequestDataChoiceIxPlain,	// 明文
	kP2SecurityRequestDataChoiceIxCipher,	// 密文
	kP2SecurityRequestDataChoiceNum
} P2SecurityRequestDataChoiceIxT;
int P2SecurityRequestDataChoice2Ix(uint8_t choice);
uint8_t P2SecurityRequestDataChoiceIx2Value(int choice_ix);
cp_t P2SecurityRequestDataChoiceIxValid(int choice_ix);
cp_t P2SecurityRequestDataChoiceValid(uint8_t choice);
//}}}


//{{{ part
typedef struct
{
	P2ChoicePcut choice;
} P2SecurityRequestDataChoicePcut;
#define kP2SecurityRequestDataChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2SecurityRequestDataChoicePcutOpen(P2SecurityRequestDataChoicePcut *m);
cp_t P2SecurityRequestDataChoicePcutClose(P2SecurityRequestDataChoicePcut *m);
cp_t P2SecurityRequestDataChoicePcutValid(const P2SecurityRequestDataChoicePcut *m);
//}}}


// test
cp_t P2SecurityRequestDataChoiceTest(QTEST_ARG);
#endif

