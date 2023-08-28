#ifndef __P2_COMPACT_PROXY_SET_THEN_GET_REQUEST_H_ 
#define __P2_COMPACT_PROXY_SET_THEN_GET_REQUEST_H_ 


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"

#include "p2_tsa.h"
#include "p2_oad_data_oad_unsigned.h"
#include "p2_sequence_of_var_len.h"


#define kP2CompactProxySetThenGetRequestName				"compact_proxy_set_then_get_request"
#define kP2CompactProxySetThenGetRequestNamePiid			"piid"
#define kP2CompactProxySetThenGetRequestNameTimeout			"timeout"
#define kP2CompactProxySetThenGetRequestNameTsa				"tsa"
#define kP2CompactProxySetThenGetRequestNameSequenceOf		"sequence_of_oad_data_oad_unsigned"

//{{{ base
/*
   CompactProxySetThenGetRequest∷=SEQUENCE
   {
    服务序号-优先级 PIID，
	代理整个请求的超时时间 long-unsigned，
	 目标服务器地址 TSA，
	 若干个对象属性的设置后读取 SEQUENCE OF
	 {
	  设置的对象属性描述符 OAD，
	   设置数值 Data，
	    读取的对象属性描述符 OAD，
		 延时读取时间 unsigned
		  }
		  }
*/
#define kP2CompactProxySetThenGetRequestPiidOffset		(0)
#define kP2CompactProxySetThenGetRequestPiidSize		(kP2PiidSize)

#define kP2CompactProxySetThenGetRequestTimeoutOffset		\
	(kP2CompactProxySetThenGetRequestPiidOffset + kP2CompactProxySetThenGetRequestPiidSize)
#define kP2CompactProxySetThenGetRequestTimeoutSize			(kDlt698LongUnsignedSize)

#define kP2CompactProxySetThenGetRequestTsaOffset			\
	(kP2CompactProxySetThenGetRequestTimeoutOffset + kP2CompactProxySetThenGetRequestTimeoutSize)
#define kP2CompactProxySetThenGetRequestTsaSize(_tsa_size)	(_tsa_size)

#define kP2CompactProxySetThenGetRequestSequenceOfOffset(_tsa_size)	\
	(kP2CompactProxySetThenGetRequestTsaOffset + kP2CompactProxySetThenGetRequestTsaSize(_tsa_size))
#define kP2CompactProxySetThenGetRequestSequenceOfSize(_so_size)		(_so_size)


#define kP2CompactProxySetThenGetRequestWholeSize(_tsa_size, _so_size)	\
	(kP2CompactProxySetThenGetRequestSequenceOfOffset(_tsa_size) + kP2CompactProxySetThenGetRequestSequenceOfSize(_so_size))
//}}}


//{{{ cut
typedef enum
{
	kP2CompactProxySetThenGetRequestCutIxPiid,
	kP2CompactProxySetThenGetRequestCutIxTimeout,
	kP2CompactProxySetThenGetRequestCutIxTsa,
	kP2CompactProxySetThenGetRequestCutIxSequenceOf,
	kP2CompactProxySetThenGetRequestCutNum				
} P2CompactProxySetThenGetRequestCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxySetThenGetRequestCutNum];

	// sub
	P2TsaPcut tsa;
	P2OadDataOadUnsignedPcut oad_data_oad_unsigned_cut;
	P2SequenceOfVarLenPcut sequence_of_cut;
} P2CompactProxySetThenGetRequestPcut;
#define kP2CompactProxySetThenGetRequestPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2TsaPcutDef, kP2OadDataOadUnsignedPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2CompactProxySetThenGetRequestPcutOpen(P2CompactProxySetThenGetRequestPcut *m);
cp_t P2CompactProxySetThenGetRequestPcutClose(P2CompactProxySetThenGetRequestPcut *m);
cp_t P2CompactProxySetThenGetRequestPcutValid(const P2CompactProxySetThenGetRequestPcut *m);

cp_t P2CompactProxySetThenGetRequestPcutOpenBase(Pcut *base);
cp_t P2CompactProxySetThenGetRequestPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxySetThenGetRequestTest(QTEST_ARG);
#endif

