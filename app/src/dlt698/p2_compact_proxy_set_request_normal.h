#ifndef __P2_COMPACT_PROXY_SET_REQUEST_NORMAL_H_ 
#define __P2_COMPACT_PROXY_SET_REQUEST_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_tsa.h"
#include "p2_oad_data.h"
#include "p2_sequence_of_var_len.h"


#define kP2CompactProxySetRequestNormalName				"compact_proxy_set_request_normal"
#define kP2CompactProxySetRequestNormalNamePiid			"piid"
#define kP2CompactProxySetRequestNormalNameTimeout		"timeout"
#define kP2CompactProxySetRequestNormalNameTsa			"tsa"
#define kP2CompactProxySetRequestNormalNameSequenceOfOadData	"sequence_of_oad_data"

//{{{ base
/*
CompactProxySetRequestList∷=SEQUENCE
{
 服务序号-优先级 PIID，
 代理整个请求的超时时间 long-unsigned，
 目标服务器地址 TSA，
 若干个对象属性描述符及数据 SEQUENCE OF
 {
  对象属性描述符 OAD，
   设置数值 Data
   }
   }
*/
#define kP2CompactProxySetRequestNormalPiidOffset	(0)
#define kP2CompactProxySetRequestNormalPiidSize		(kP2PiidSize)

#define kP2CompactProxySetRequestNormalTimeoutOffset	\
	(kP2CompactProxySetRequestNormalPiidOffset + kP2CompactProxySetRequestNormalPiidSize)
#define kP2CompactProxySetRequestNormalTimeoutSize		(kDlt698LongUnsignedSize)

#define kP2CompactProxySetRequestNormalTsaOffset		\
	(kP2CompactProxySetRequestNormalTimeoutOffset + kP2CompactProxySetRequestNormalTimeoutSize)
#define kP2CompactProxySetRequestNormalTsaSize(_tsa_size)		(_tsa_size)

#define kP2CompactProxySetRequestNormalSequenceOfOadDataOffset(_tsa_size)		\
	(kP2CompactProxySetRequestNormalTsaOffset + kP2CompactProxySetRequestNormalTsaSize(_tsa_size))
#define kP2CompactProxySetRequestNormalSequenceOfOadDataSize(_sood_size)		(_sood_size)


#define kP2CompactProxySetRequestNormalWholeSize(_tsa_size, _sood_size)	\
	(kP2CompactProxySetRequestNormalSequenceOfOadDataOffset(_tsa_size) + kP2CompactProxySetRequestNormalSequenceOfOadDataSize(_sood_size))
//}}}


//{{{ cut
typedef enum
{
	kP2CompactProxySetRequestNormalCutIxPiid,
	kP2CompactProxySetRequestNormalCutIxTimeout,
	kP2CompactProxySetRequestNormalCutIxTsa,
	kP2CompactProxySetRequestNormalCutIxSequenceOfOadData,
	kP2CompactProxySetRequestNormalCutNum				
} P2CompactProxySetRequestNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxySetRequestNormalCutNum];

	// sub
	P2TsaPcut tsa;
	P2OadDataPcut oad_data_cut;
	P2SequenceOfVarLenPcut sequence_of_oad_data_cut;
} P2CompactProxySetRequestNormalPcut;
#define kP2CompactProxySetRequestNormalPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2TsaPcutDef, kP2OadDataPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2CompactProxySetRequestNormalPcutOpen(P2CompactProxySetRequestNormalPcut *m);
cp_t P2CompactProxySetRequestNormalPcutClose(P2CompactProxySetRequestNormalPcut *m);
cp_t P2CompactProxySetRequestNormalPcutValid(const P2CompactProxySetRequestNormalPcut *m);

cp_t P2CompactProxySetRequestNormalPcutOpenBase(Pcut *base);
cp_t P2CompactProxySetRequestNormalPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxySetRequestNormalTest(QTEST_ARG);
#endif

