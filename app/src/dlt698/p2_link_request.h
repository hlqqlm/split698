#ifndef __DLT698_45_LINK_REQUEST_H_ 
#define __DLT698_45_LINK_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_enum.h"
#include "qdlt698.h"




/*
PIID-ACD——见 0。 心跳周期——单位：秒。 date_time——见 0。

LINK-Request∷=SEQUENCE
{
	服务序号-优先级-ACD PIID-ACD
	请求类型 ENUMERATED
	{
		登录 （0），			
		心跳 （1），
		退出登录 （2）
	}，
	心跳周期 long-unsigned，
	请求时间 date_time，
}
*/
#define kP2LinkRequestName			"link_request"
#define kP2LinkRequestNamePiidAcd	"piid_acd"
#define kP2LinkRequestNameType		"type"
#define kP2LinkRequestNameHeartbeatInterval		"heartbeat_interval"
#define kP2LinkRequestNameDatetime	"datetime"


// basic information
#define kP2LinkRequestPiidAcdOffset				(0)
#define kP2LinkRequestPiidAcdSize				(kP2PiidAcdSize)

#define kP2LinkRequestTypeOffset				(kP2LinkRequestPiidAcdOffset + kP2LinkRequestPiidAcdSize)
#define kP2LinkRequestTypeSize					(1)

#define kP2LinkRequestHeartbeatIntervalOffset	(kP2LinkRequestTypeOffset + kP2LinkRequestTypeSize)
#define kP2LinkRequestHeartbeatIntervalSize		(kDlt698LongUnsignedSize)

#define kP2LinkRequestDatetimeOffset			(kP2LinkRequestHeartbeatIntervalOffset + kP2LinkRequestHeartbeatIntervalSize)
#define kP2LinkRequestDatetimeSize				(kDlt698DatetimeSize)


//{{{ type
typedef enum 
{
	kP2LinkRequestTypeEnumLogin = 0,		// 登录 （0），
	kP2LinkRequestTypeEnumHeartbeat = 1,	// 心跳 （1），
	kP2LinkRequestTypeEnumHeartLogout = 2,	// 退出登录 （2）
} P2LinkRqeustEnumT;

typedef enum 
{
	kP2LinkRequestTypeEnumIxLogin,		// 登录 （0），
	kP2LinkRequestTypeEnumIxHeartbeat,	// 心跳 （1），
	kP2LinkRequestTypeEnumIxLogout,		// 退出登录 （2）
	kP2LinkRequestTypeEnumNum
} P2LinkRequestTypeEnumIxT;
int P2LinkRequestTypeEnum2Ix(uint8_t enumv);
uint8_t P2LinkRequestTypeEnumIx2Value(int enumv_ix);
cp_t P2LinkRequestTypeEnumIxValid(int enumv_ix);
cp_t P2LinkRequestTypeEnumValid(uint8_t enumv);
const char *P2LinkRequestTypeEnumStr(uint8_t enumv);

const char *P2LinkRequestTypeMem(const char *whole);
uint8_t P2LinkRequestTypeValue(const char *whole);
//}}}


uint16_t P2LinkRequestHeartbeatIntervalValue(const char *whole);


//{{{ cut
typedef enum
{
	kP2LinkRequestCutIxPiidAcd,
	kP2LinkRequestCutIxType,
	kP2LinkRequestCutIxHeartbeatInterval,
	kP2LinkRequestCutIxDatetime,
	kP2LinkRequestPartNum				
} P2LinkRequestCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2LinkRequestPartNum];

	// 模仿P2ClientApduPcut初始化
	P2EnumPcut enum_type;
} P2LinkRequestPcut;
#define kP2LinkRequestPcutDef {				\
	kPcutDef, { kPcutItemDef }					\
	, kP2EnumPcutDef								\
}

cp_t P2LinkRequestPcutOpen(P2LinkRequestPcut *m);
cp_t P2LinkRequestPcutClose(P2LinkRequestPcut *m);
cp_t P2LinkRequestPcutValid(const P2LinkRequestPcut *m);

cp_t P2LinkRequestPcutOpenBase(Pcut *base);
cp_t P2LinkRequestPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2LinkRequestTest(QTEST_ARG);
#endif

