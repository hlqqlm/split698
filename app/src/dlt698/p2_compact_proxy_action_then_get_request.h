#ifndef __P2_COMPACT_PROXY_ACTION_THEN_GET_REQUEST_H_ 
#define __P2_COMPACT_PROXY_ACTION_THEN_GET_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"

#include "p2_tsa.h"
#include "p2_omd_data_oad_unsigned.h"
#include "p2_sequence_of_var_len.h"


#define kP2CompactProxyActionThenGetRequestName				"compact_proxy_action_then_get_request"
#define kP2CompactProxyActionThenGetRequestNamePiid			"piid"
#define kP2CompactProxyActionThenGetRequestNameTimeout		"timeout"
#define kP2CompactProxyActionThenGetRequestNameTsa			"tsa"
#define kP2CompactProxyActionThenGetRequestNameSequenceOfOmdDataOadUnsigned	"sequence_of_omd_data_oad_unsigned"

//{{{ base
/*
CompactProxyActionThenGetRequest∷=SEQUENCE
{
 服务序号-优先级 PIID，
  代理整个请求的超时时间 long-unsigned，
  目标服务器地址 TSA，
  若干个对象方法及属性的操作后读取 SEQUENCE OF
  {
   操作的对象方法描述符 OMD，
    方法参数 Data，
	 读取的对象属性描述符 OAD，
	  延时读取时间 unsigned
	   }
	   }
*/
#define kP2CompactProxyActionThenGetRequestPiidOffset	(0)
#define kP2CompactProxyActionThenGetRequestPiidSize		(kP2PiidSize)

#define kP2CompactProxyActionThenGetRequestTimeoutOffset	\
	(kP2CompactProxyActionThenGetRequestPiidOffset + kP2CompactProxyActionThenGetRequestPiidSize)
#define kP2CompactProxyActionThenGetRequestTimeoutSize		(kDlt698LongUnsignedSize)

#define kP2CompactProxyActionThenGetRequestTsaOffset	\
	(kP2CompactProxyActionThenGetRequestTimeoutOffset + kP2CompactProxyActionThenGetRequestTimeoutSize)
#define kP2CompactProxyActionThenGetRequestTsaSize(_tsa_size)		(_tsa_size)

#define kP2CompactProxyActionThenGetRequestSequenceOfOmdDataOadUnsignedOffset(_tsa_size)	\
	(kP2CompactProxyActionThenGetRequestPiidOffset + kP2CompactProxyActionThenGetRequestPiidSize(_tsa_size))
#define kP2CompactProxyActionThenGetRequestSequenceOfOmdDataOadUnsignedSize(_soodou_size)		(_soodou_size)


#define kP2CompactProxyActionThenGetRequestWholeSize(_tsa_size, _soodou_size)	\
	(kP2CompactProxyActionThenGetRequestSequenceOfOmdDataOadUnsignedOffset(_tsa_size) + kP2CompactProxyActionThenGetRequestSequenceOfOmdDataOadUnsignedSize(_soodou_size))
//}}}


//{{{ cut
typedef enum
{
	kP2CompactProxyActionThenGetRequestCutIxPiid,
	kP2CompactProxyActionThenGetRequestCutIxTimeout,
	kP2CompactProxyActionThenGetRequestCutIxTsa,
	kP2CompactProxyActionThenGetRequestCutIxSequenceOf,
	kP2CompactProxyActionThenGetRequestCutNum				
} P2CompactProxyActionThenGetRequestCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxyActionThenGetRequestCutNum];

	// sub
	P2TsaPcut tsa;
	P2OmdDataOadUnsignedPcut omd_data_oad_unsigned_cut;
	P2SequenceOfVarLenPcut sequence_of_omd_data_oad_unsigned_cut;
} P2CompactProxyActionThenGetRequestPcut;
#define kP2CompactProxyActionThenGetRequestPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2TsaPcutDef, kP2OmdDataOadUnsignedPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2CompactProxyActionThenGetRequestPcutOpen(P2CompactProxyActionThenGetRequestPcut *m);
cp_t P2CompactProxyActionThenGetRequestPcutClose(P2CompactProxyActionThenGetRequestPcut *m);
cp_t P2CompactProxyActionThenGetRequestPcutValid(const P2CompactProxyActionThenGetRequestPcut *m);

cp_t P2CompactProxyActionThenGetRequestPcutOpenBase(Pcut *base);
cp_t P2CompactProxyActionThenGetRequestPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxyActionThenGetRequestTest(QTEST_ARG);
#endif

