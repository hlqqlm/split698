#ifndef __P2_COMPACT_PROXY_ACTION_REQUEST_H_ 
#define __P2_COMPACT_PROXY_ACTION_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"

#include "p2_tsa.h"
#include "p2_omd_data.h"
#include "p2_sequence_of_var_len.h"


#define kP2CompactProxyActionRequestName				"compact_proxy_action_request"
#define kP2CompactProxyActionRequestNamePiid			"piid"
#define kP2CompactProxyActionRequestNameTimeout			"timeout"
#define kP2CompactProxyActionRequestNameTsa				"tsa"
#define kP2CompactProxyActionRequestNameSequenceOfOmdData	"sequence_of_omd_data"

//{{{ base
/*
CompactProxyActionRequest∷=SEQUENCE
{
 服务序号-优先级 PIID，
 代理整个请求的超时时间 long-unsigned，
 目标服务器地址 TSA，
 若干个对象方法描述符及参数 SEQUENCE OF
  {
  对象方法描述符 OMD，
   方法参数 Data
    }
	}
*/
#define kP2CompactProxyActionRequestPiidOffset		(0)
#define kP2CompactProxyActionRequestPiidSize		(kP2PiidSize)

#define kP2CompactProxyActionRequestTimeoutOffset	\
	(kP2CompactProxyActionRequestPiidOffset + kP2CompactProxyActionRequestPiidSize)
#define kP2CompactProxyActionRequestTimeoutSize		(kDlt698LongUnsignedSize)

#define kP2CompactProxyActionRequestTsaOffset		\
	(kP2CompactProxyActionRequestTimeoutOffset + kP2CompactProxyActionRequestTimeoutSize)
#define kP2CompactProxyActionRequestTsaSize(_tsa_size)		(_tsa_size)

#define kP2CompactProxyActionRequestSequenceOfOmdDataOffset(_tsa_size)	\
	(kP2CompactProxyActionRequestTsaOffset + kP2CompactProxyActionRequestTsaSize(_tsa_size))
#define kP2CompactProxyActionRequestSequenceOfOmdDataSize(_sood_size)		(_sood_size)

#define kP2CompactProxyActionRequestWholeSize(_tsa_size, _sood_size)	\
	(kP2CompactProxyActionRequestSequenceOfOmdDataOffset(_tsa_size) + kP2CompactProxyActionRequestSequenceOfOmdDataSize(_sood_size))
//}}}


//{{{ cut
typedef enum
{
	kP2CompactProxyActionRequestCutIxPiid,
	kP2CompactProxyActionRequestCutIxTimeout,
	kP2CompactProxyActionRequestCutIxTsa,
	kP2CompactProxyActionRequestCutIxSequenceOfOmdData,
	kP2CompactProxyActionRequestCutNum				
} P2CompactProxyActionRequestCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxyActionRequestCutNum];

	// sub
	P2TsaPcut tsa;
	P2OmdDataPcut omd_data_cut;
	P2SequenceOfVarLenPcut sequence_of_omd_data_cut;
} P2CompactProxyActionRequestPcut;
#define kP2CompactProxyActionRequestPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2TsaPcutDef, kP2OmdDataPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2CompactProxyActionRequestPcutOpen(P2CompactProxyActionRequestPcut *m);
cp_t P2CompactProxyActionRequestPcutClose(P2CompactProxyActionRequestPcut *m);
cp_t P2CompactProxyActionRequestPcutValid(const P2CompactProxyActionRequestPcut *m);

cp_t P2CompactProxyActionRequestPcutOpenBase(Pcut *base);
cp_t P2CompactProxyActionRequestPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxyActionRequestTest(QTEST_ARG);
#endif

