#ifndef __P2_SECURITY_REQUEST_H_ 
#define __P2_SECURITY_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_security_request_data_choice.h"
#include "p2_security_request_verify_choice.h"


#define kP2SecurityRequestName				"security_request"
#define kP2SecurityRequestNameData			"data"
#define kP2SecurityRequestNameVerify		"verify"

//{{{ base
/*
SECURITY-Request∷=SEQUENCE
{
应用数据单元 CHOICE
{
明文数据单元 [0] octet-string，
密文数据单元 [1] octet-string
}，
数据验证信息 CHOICE
{
数据验证码 [0] SID_MAC，
随机数 [1] RN，
随机数+MAC [2] RN_MAC，
安全标识 [3] SID
}
}
*/
// SR = Security Request
// Data = 应用数据单元application data unit
#define kP2SecurityRequestDataOffset				(0)
#define kP2SecurityRequestDataSize(_data_size)		(_data_size)

#define kP2SecurityRequestVerifyOffset(_data_size)	(kP2SecurityRequestDataOffset + kP2SecurityRequestDataSize(_data_size))
#define kP2SecurityRequestVerifySize(_verify_size)	(_verify_size)

#define kP2SecurityRequestWholeSize(_data_size, _verify_size)	(kP2SecurityRequestVerifyOffset(_data_size) + kP2SecurityRequestVerifySize(_verify_size))

// value
typedef struct 
{
	uint8_t foo;
} P2SecurityRequestValue;
#define kP2SecurityRequestValueDef { 0 }
//}}}


//{{{ cut
typedef enum
{
	kP2SecurityRequestCutIxData,
	kP2SecurityRequestCutIxVerify,
	kP2SecurityRequestCutNum				
} P2SecurityRequestCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SecurityRequestCutNum];

	// 模仿P2ClientApduPcut初始化
	P2SecurityRequestDataChoicePcut data;
	P2SecurityRequestVerifyChoicePcut verify;
} P2SecurityRequestPcut;
#define kP2SecurityRequestPcutDef {				\
	kPcutDef, { kPcutItemDef }					\
	, kP2SecurityRequestDataChoicePcutDef, kP2SecurityRequestVerifyChoicePcutDef	\
}

cp_t P2SecurityRequestPcutOpen(P2SecurityRequestPcut *m);
cp_t P2SecurityRequestPcutClose(P2SecurityRequestPcut *m);
cp_t P2SecurityRequestPcutValid(const P2SecurityRequestPcut *m);

cp_t P2SecurityRequestPcutOpenBase(Pcut *base);
cp_t P2SecurityRequestPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2SecurityRequestTest(QTEST_ARG);
#endif

