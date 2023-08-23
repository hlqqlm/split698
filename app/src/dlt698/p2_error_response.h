#ifndef __P2_ERROR_RESPONSE_H_ 
#define __P2_ERROR_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_enum.h"
#include "qdlt698.h"



//{{{ base
/*
ERROR-Response∷=SEQUENCE
{
 服务序号-优先级 PIID，
  异常类型 ENUMERATED
  {
   APDU 无法解析 （1），
    服务不支持 （2），
	 其他 （255）
	 }
	 }
*/
#define kP2ErrorResponseName			"error_response"
#define kP2ErrorResponseNamePiid		"piid"
#define kP2ErrorResponseNameType		"type"


// basic information
#define kP2ErrorResponsePiidOffset				(0)
#define kP2ErrorResponsePiidSize				(kP2PiidSize)

#define kP2ErrorResponseTypeOffset				(kP2ErrorResponsePiidOffset + kP2ErrorResponsePiidSize)
#define kP2ErrorResponseTypeSize				(1)
//}}}


//{{{ type
typedef enum 
{
	kP2ErrorResponseTypeEnumApdu = 1,		// APDU 无法解析 （1），
	kP2ErrorResponseTypeEnumUnsupported = 2,	//  服务不支持 （2），
	kP2ErrorResponseTypeEnumOther = 255,	// 其他 （255）
} P2ErrorResponseEnumT;

typedef enum 
{
	kP2ErrorResponseTypeEnumIxApdu,		// 
	kP2ErrorResponseTypeEnumIxUnsupported,	// 
	kP2ErrorResponseTypeEnumIxOther,		// 
	kP2ErrorResponseTypeEnumNum
} P2ErrorResponseTypeEnumIxT;
int P2ErrorResponseTypeEnum2Ix(uint8_t enumv);
uint8_t P2ErrorResponseTypeEnumIx2Value(int enumv_ix);
cp_t P2ErrorResponseTypeEnumIxValid(int enumv_ix);
cp_t P2ErrorResponseTypeEnumValid(uint8_t enumv);
const char *P2ErrorResponseTypeEnumStr(uint8_t enumv);

const char *P2ErrorResponseTypeMem(const char *whole);
uint8_t P2ErrorResponseTypeValue(const char *whole);
//}}}


//{{{ cut
typedef enum
{
	kP2ErrorResponseCutIxPiid,
	kP2ErrorResponseCutIxType,
	kP2ErrorResponseCutNum				
} P2ErrorResponseCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ErrorResponseCutNum];

	// 模仿P2ClientApduPcut初始化
	P2EnumPcut enum_type;		// 因为要执行enum do table，所以必须要用sub来解析enum
} P2ErrorResponsePcut;
#define kP2ErrorResponsePcutDef { kPcutDef, { kPcutItemDef }, kP2EnumPcutDef }

cp_t P2ErrorResponsePcutOpen(P2ErrorResponsePcut *m);
cp_t P2ErrorResponsePcutClose(P2ErrorResponsePcut *m);
cp_t P2ErrorResponsePcutValid(const P2ErrorResponsePcut *m);

cp_t P2ErrorResponsePcutOpenBase(Pcut *base);
cp_t P2ErrorResponsePcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ErrorResponseTest(QTEST_ARG);
#endif

