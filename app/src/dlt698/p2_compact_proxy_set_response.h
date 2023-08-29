#ifndef __P2_COMPACT_PROXY_SET_RESPONSE_H_ 
#define __P2_COMPACT_PROXY_SET_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_sequence_of.h"


#define kP2CompactProxySetResponseName		"compact_proxy_set_response"

/*
CompactProxySetResponse∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 若干个对象属性描述符及结果 SEQUENCE OF DAR
 }
*/
// 
#define kP2CompactProxySetResponsePiidAcdOffset		(0)
#define kP2CompactProxySetResponsePiidAcdSize		(kP2PiidAcdSize)

#define kP2CompactProxySetResponseSequenceOfDarOffset		\
	(kP2CompactProxySetResponsePiidAcdOffset + kP2CompactProxySetResponsePiidAcdSize)
#define kP2CompactProxySetResponseSequenceOfDarSize(_sod_size)	(_sod_size)

#define kP2CompactProxySetResponseWholeSize(_sod_size)		\
	(kP2CompactProxySetResponseSequenceOfDarOffset + kP2CompactProxySetResponseSequenceOfDarSize(_sod_size))

// piid-acd
uint8_t P2CompactProxySetResponsePiidAcd(const char *whole);


//{{{ cut
typedef enum
{
	kP2CompactProxySetResponseCutIxPiidAcd,
	kP2CompactProxySetResponseCutIxSequenceOfDar,
	kP2CompactProxySetResponseCutNum				
} P2CompactProxySetResponseCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxySetResponseCutNum];

	P2SequenceOfPcut sequence_of_dar;
} P2CompactProxySetResponsePcut;
#define kP2CompactProxySetResponsePcutDef { kPcutDef, { kPcutItemDef }, kP2SequenceOfPcutDef }

cp_t P2CompactProxySetResponsePcutOpen(P2CompactProxySetResponsePcut *m);
cp_t P2CompactProxySetResponsePcutClose(P2CompactProxySetResponsePcut *m);
cp_t P2CompactProxySetResponsePcutValid(const P2CompactProxySetResponsePcut *m);

cp_t P2CompactProxySetResponsePcutOpenBase(Pcut *base);
cp_t P2CompactProxySetResponsePcutCloseBase(Pcut *base);
//}}}


//{{{ value
#define kP2CompactProxySetResponseDarNumMax		(10)
typedef struct
{
	int num;
	uint8_t list[kP2CompactProxySetResponseDarNumMax];
} P2CompactProxySetResponseDarValue;
#define kP2CompactProxySetResponseDarValueDef { 0, {0} }
typedef struct 
{
	uint8_t piid_acd;
	P2CompactProxySetResponseDarValue dar;
} P2CompactProxySetResponseValue;
#define kP2CompactProxySetResponseValueDef { 0, kP2CompactProxySetResponseDarValueDef }
cp_t P2CompactProxySetResponsePcutValue(P2CompactProxySetResponseValue *value, const char *whole);

void P2CompactProxySetResponsePcutValuePrint(const P2CompactProxySetResponseValue *value);
//}}}

// test
cp_t P2CompactProxySetResponseTest(QTEST_ARG);
#endif

