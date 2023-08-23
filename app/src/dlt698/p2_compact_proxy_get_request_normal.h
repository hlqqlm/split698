#ifndef __P2_COMPACT_PROXY_GET_REQUEST_NORMAL_H_ 
#define __P2_COMPACT_PROXY_GET_REQUEST_NORMAL_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_tsa.h"
#include "p2_sequence_of.h"


#define kP2CompactProxyGetRequestNormalName		"compact_proxy_get_request_normal"
#define kP2CompactProxyGetRequestNormalNameTsa	"tsa"

/*
CompactProxyGetRequestNormal∷=SEQUENCE
{
 服务序号-优先级 PIID，
 代理整个请求的超时时间 long-unsigned，
  目标服务器地址 TSA，
  若干个对象属性描述符 SEQUENCE OF OAD
  }
*/
#define kP2CompactProxyGetRequestNormalPiidOffset	(0)
#define kP2CompactProxyGetRequestNormalPiidSize		(1)

#define kP2CompactProxyGetRequestNormalTimeoutOffset	(kP2CompactProxyGetRequestNormalPiidOffset + kP2CompactProxyGetRequestNormalPiidSize)
#define kP2CompactProxyGetRequestNormalTimeoutSize		(kDlt698LongUnsignedSize)

#define kP2CompactProxyGetRequestNormalTsaOffset			(kP2CompactProxyGetRequestNormalTimeoutOffset + kP2CompactProxyGetRequestNormalTimeoutSize)
#define kP2CompactProxyGetRequestNormalTsaSize(_tsa_size)	(_tsa_size)

#define kP2CompactProxyGetRequestNormalSequenceOfOffset(_tsa_size)	(kP2CompactProxyGetRequestNormalTsaOffset + kP2CompactProxyGetRequestNormalTsaSize(_tsa_size))
#define kP2CompactProxyGetRequestNormalSequenceOfSize(_so_size)		(_so_size)



//{{{ cut
typedef enum
{
	kP2CompactProxyGetRequestNormalCutIxPiid,
	kP2CompactProxyGetRequestNormalCutIxTimeout,
	kP2CompactProxyGetRequestNormalCutIxTsa,
	kP2CompactProxyGetRequestNormalCutIxSequenceOfOad,
	kP2CompactProxyGetRequestNormalCutNum				
} P2CompactProxyGetRequestNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxyGetRequestNormalCutNum];

	P2TsaPcut tsa_cut;
	P2SequenceOfPcut sequence_of_oad_cut;
} P2CompactProxyGetRequestNormalPcut;
#define kP2CompactProxyGetRequestNormalPcutDef { kPcutDef, { kPcutItemDef }, kP2TsaPcutDef, kP2SequenceOfPcutDef }

cp_t P2CompactProxyGetRequestNormalPcutOpen(P2CompactProxyGetRequestNormalPcut *m);
cp_t P2CompactProxyGetRequestNormalPcutClose(P2CompactProxyGetRequestNormalPcut *m);
cp_t P2CompactProxyGetRequestNormalPcutValid(const P2CompactProxyGetRequestNormalPcut *m);

cp_t P2CompactProxyGetRequestNormalPcutOpenBase(Pcut *base);
cp_t P2CompactProxyGetRequestNormalPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxyGetRequestNormalTest(QTEST_ARG);
#endif

