#ifndef __P2_CONNECT_RESPONSE_INFO_H_ 
#define __P2_CONNECT_RESPONSE_INFO_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_enum.h"
#include "p2_optional.h"
#include "p2_security_data.h"


#define kP2ConnectResponseInfoName				"action_response_normal"
#define kP2ConnectResponseInfoNameConnectResult	"connect_result"
#define kP2ConnectResponseInfoNameSecurityDataOptional	"security_data_optional"


//{{{ base
/*
ConnectResponseInfo∷=SEQUENCE
{
 认证结果 ConnectResult，
  认证附加信息 SecurityData OPTIONAL
  }

  SecurityData∷=SEQUENCE
  {
   服务器随机数 RN，
    服务器签名信息 octet-string
	}
*/

// basic information
#define kP2ConnectResponseInfoConnectResultOffset		(0)
#define kP2ConnectResponseInfoConnectResultSize			(1)

#define kP2ConnectResponseInfoSecurityDataOptionalOffset		\
	(kP2ConnectResponseInfoConnectResultOffset + kP2ConnectResponseInfoConnectResultSize)
#define kP2ConnectResponseInfoSecurityDataOptionalSize(_sdo_size)		(_sdo_size)
//}}}


//{{{ connect_result
/*
ConnectResult∷=ENUMERATED
{
	 允许建立应用连接 （0），
		 密码错误 （1），
		 对称解密错误 （2），
		 非对称解密错误 （3），
		 签名错误 （4），
		 协议版本不匹配 （5），
		 ESAM 通信故障 （6），
		 其他错误 （255）
}
*/
typedef enum 
{
	kP2ConnectResultAllowConnection = 0,		// 允许建立应用连接 （0），
	kP2ConnectResultPasswordWrong = 1,		// 密码错误 （1），
	kP2ConnectResultSymmetricDecryptionError = 2,		// 对称解密错误 （2），
	kP2ConnectResultAsymmetricDecryptionError = 1,		// 非对称解密错误 （3），
	kP2ConnectResultSignWrong = 4,		// 签名错误 （4），
	kP2ConnectResultProtocolVersionWrong = 5,		// 协议版本不匹配 （5），
	kP2ConnectResultEsamError = 6,		// ESAM 通信故障 （6），
	kP2ConnectResultOtherError = 255,		// 其他错误 （255）
} P2ConnectResultEnumT;

typedef enum 
{
	kP2ConnectResultIxAllowConnection,		// 允许建立应用连接 （0），
	kP2ConnectResultIxPasswordWrong,		// 密码错误 （1），
	kP2ConnectResultIxSymmetricDecryptionError,		// 对称解密错误 （2），
	kP2ConnectResultIxAsymmetricDecryptionError,		// 非对称解密错误 （3），
	kP2ConnectResultIxSignWrong,		// 签名错误 （4），
	kP2ConnectResultIxProtocolVersionWrong,		// 协议版本不匹配 （5），
	kP2ConnectResultIxEsamError,		// ESAM 通信故障 （6），
	kP2ConnectResultIxOtherError,		// 其他错误 （255）
	kP2ConnectResultNum
} P2ConnectResultIxT;
int P2ConnectResultEnum2Ix(uint8_t enumv);
uint8_t P2ConnectResultEnumIx2Value(int enumv_ix);
cp_t P2ConnectResultEnumIxValid(int enumv_ix);
cp_t P2ConnectResultEnumValid(uint8_t enumv);
const char *P2ConnectResultEnumStr(uint8_t enumv);

const char *P2ConnectResultMem(const char *whole);
uint8_t P2ConnectResultValue(const char *whole);
//}}}


//{{{ cut
typedef enum
{
	kP2ConnectResponseInfoCutIxConnectResult,
	kP2ConnectResponseInfoCutIxSecurityDataOptional,
	kP2ConnectResponseInfoCutNum				
} P2ConnectResponseInfoCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ConnectResponseInfoCutNum];

	// sub
	// P2ConnectResultPcut connect_result;
	P2EnumPcut enum_connect_result;

	P2OptionalPcut security_data_optional;
	P2SecurityDataPcut security_data;
} P2ConnectResponseInfoPcut;
#define kP2ConnectResponseInfoPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2EnumPcutDef, kP2OptionalPcutDef, kP2SecurityDataPcutDef			\
}

cp_t P2ConnectResponseInfoPcutOpen(P2ConnectResponseInfoPcut *m);
cp_t P2ConnectResponseInfoPcutClose(P2ConnectResponseInfoPcut *m);
cp_t P2ConnectResponseInfoPcutValid(const P2ConnectResponseInfoPcut *m);

cp_t P2ConnectResponseInfoPcutOpenBase(Pcut *base);
cp_t P2ConnectResponseInfoPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2ConnectResponseInfoTest(QTEST_ARG);
#endif

