#ifndef __P2_RELEASE_RESPONSE_H_ 
#define __P2_RELEASE_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_enum.h"
#include "qdlt698.h"



//{{{ base
/*
RELEASE-Response∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  结果 ENUMERATED {成功（0）}
  }
*/
#define kP2ReleaseResponseName			"relase_response"
#define kP2ReleaseResponseNamePiidAcd	"piid-acd"
#define kP2ReleaseResponseNameResult	"result"

// basic information
#define kP2ReleaseResponsePiidAcdOffset				(0)
#define kP2ReleaseResponsePiidAcdSize				(kP2PiidAcdSize)

#define kP2ReleaseResponseResultOffset				(kP2ReleaseResponsePiidAcdOffset + kP2ReleaseResponsePiidAcdSize)
#define kP2ReleaseResponseResultSize				(1)
//}}}


//{{{ type
typedef enum 
{
	kP2ReleaseResponseResultEnumOk = 0,		// 成功（0）
} P2ReleaseResponseEnumT;

typedef enum 
{
	kP2ReleaseResponseResultEnumIxApdu,		// 
	kP2ReleaseResponseResultEnumNum
} P2ReleaseResponseResultEnumIxT;
int P2ReleaseResponseResultEnum2Ix(uint8_t enumv);
uint8_t P2ReleaseResponseResultEnumIx2Value(int enumv_ix);
cp_t P2ReleaseResponseResultEnumIxValid(int enumv_ix);
cp_t P2ReleaseResponseResultEnumValid(uint8_t enumv);
const char *P2ReleaseResponseResultEnumStr(uint8_t enumv);

const char *P2ReleaseResponseResultMem(const char *whole);
uint8_t P2ReleaseResponseResultValue(const char *whole);
//}}}


//{{{ cut
typedef enum
{
	kP2ReleaseResponseCutIxPiidAcd,
	kP2ReleaseResponseCutIxResult,
	kP2ReleaseResponseCutNum				
} P2ReleaseResponseCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ReleaseResponseCutNum];

	// 模仿P2ClientApduPcut初始化
	P2EnumPcut enum_result;		// 因为要执行enum do table，所以必须要用sub来解析enum
} P2ReleaseResponsePcut;
#define kP2ReleaseResponsePcutDef { kPcutDef, { kPcutItemDef }, kP2EnumPcutDef }

cp_t P2ReleaseResponsePcutOpen(P2ReleaseResponsePcut *m);
cp_t P2ReleaseResponsePcutClose(P2ReleaseResponsePcut *m);
cp_t P2ReleaseResponsePcutValid(const P2ReleaseResponsePcut *m);

cp_t P2ReleaseResponsePcutOpenBase(Pcut *base);
cp_t P2ReleaseResponsePcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ReleaseResponseTest(QTEST_ARG);
#endif

