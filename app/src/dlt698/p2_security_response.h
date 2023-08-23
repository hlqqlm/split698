#ifndef __P2_SECURITY_RESPONSE_H_ 
#define __P2_SECURITY_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_security_response_data_choice.h"
#include "p2_security_response_verify.h"

#define kP2SecurityResponseName			"security_response"
#define kP2SecurityResponseNameData		"data"
#define kP2SecurityResponseNameVerify	"verify"


//{{{ base
/*
SECURITY-Response∷=SEQUENCE
{
应用数据单元 CHOICE
{
明文数据单元 [0] octet-string，
密文数据单元 [1] octet-string，
安全传输错误码 [2] DAR
}，
数据验证信息 CHOICE OPTIONAL
{
数据MAC [0] MAC
}
}
*/
#define kP2SecurityResponseDataOffset				(0)
#define kP2SecurityResponseDataSize(_data_size)		(_data_size)

#define kP2SecurityResponseVerifyOffset(_data_size)	(kP2SecurityResponseDataOffset + kP2SecurityResponseDataSize(_data_size))
#define kP2SecurityResponseVerifySize(_verify_size)	(_verify_size)

#define kP2SecurityResponseWholeSize(_data_size, _verify_size)	(kP2SecurityResponseVerifyOffset(_data_size) + kP2SecurityResponseVerifySize(_verify_size))

// value
typedef struct 
{
	uint8_t foo;
} P2SecurityResponseValue;
#define kP2SecurityResponseValueDef { 0 }
//}}}


//{{{ cut
typedef enum
{
	kP2SecurityResponseCutIxData,
	kP2SecurityResponseCutIxVerify,
	kP2SecurityResponseCutNum				
} P2SecurityResponseCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SecurityResponseCutNum];

	// 模仿P2ClientApduPcut初始化
	P2SecurityResponseDataChoicePcut data;
	P2SecurityResponseVerifyPcut verify;
} P2SecurityResponsePcut;
#define kP2SecurityResponsePcutDef {				\
	kPcutDef, { kPcutItemDef }					\
	, kP2SecurityResponseDataChoicePcutDef, kP2SecurityResponseVerifyPcutDef	\
}

cp_t P2SecurityResponsePcutOpen(P2SecurityResponsePcut *m);
cp_t P2SecurityResponsePcutClose(P2SecurityResponsePcut *m);
cp_t P2SecurityResponsePcutValid(const P2SecurityResponsePcut *m);

cp_t P2SecurityResponsePcutOpenBase(Pcut *base);
cp_t P2SecurityResponsePcutCloseBase(Pcut *base);
//}}}


// fill
// fill_data		填data部分
// fill_verify		填verify部分
cp_t P2SecurityResponseFillInit(Pfill *m, Pfill *fill_data, Pfill *fill_verify);

// test
cp_t P2SecurityResponseTest(QTEST_ARG);
#endif

