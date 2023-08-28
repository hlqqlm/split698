#ifndef __P2_COMPACT_PROXY_GET_REQUEST_RECORD_H_ 
#define __P2_COMPACT_PROXY_GET_REQUEST_RECORD_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_tsa.h"
#include "p2_rsd.h"
#include "p2_rcsd.h"


#define kP2CompactProxyGetRequestRecordName		"compact_proxy_get_request_record"

/*
CompactProxyGetRequestRecord∷=SEQUENCE
{
 服务序号-优先级 PIID，
 代理请求的超时时间 long-unsigned，
  目标服务器地址 TSA，
   对象属性描述符 OAD，
    记录行选择描述符 RSD，
	记录列选择描述符 RCSD
	}
*/
#define kP2CompactProxyGetRequestRecordPiidOffset	(0)
#define kP2CompactProxyGetRequestRecordPiidSize				(kP2PiidSize)

#define kP2CompactProxyGetRequestRecordTimeoutOffset	\
	(kP2CompactProxyGetRequestRecordPiidOffset + kP2CompactProxyGetRequestRecordPiidSize)
#define kP2CompactProxyGetRequestRecordTimeoutSize			(kDlt698LongUnsignedSize)

#define kP2CompactProxyGetRequestRecordTsaOffset		\
	(kP2CompactProxyGetRequestRecordTimeoutOffset + kP2CompactProxyGetRequestRecordTimeoutSize)
#define kP2CompactProxyGetRequestRecordTsaSize(_tsa_size)	(_tsa_size)

#define kP2CompactProxyGetRequestRecordOadOffset(_tsa_size)	\
	(kP2CompactProxyGetRequestRecordTsaOffset + kP2CompactProxyGetRequestRecordTsaSize(_tsa_size))
#define kP2CompactProxyGetRequestRecordOadSize				(OAD_MEM_SIZE)

#define kP2CompactProxyGetRequestRecordRsdOffset(_tsa_size)	\
	(kP2CompactProxyGetRequestRecordOadOffset(_tsa_size) + kP2CompactProxyGetRequestRecordOadSize)
#define kP2CompactProxyGetRequestRecordRsdSize				(_rsd_size)

#define kP2CompactProxyGetRequestRecordRcsdOffset(_tsa_size, _rsd_size)	\
	(kP2CompactProxyGetRequestRecordRsdOffset(_tsa_size) + kP2CompactProxyGetRequestRecordRsdSize(_rsd_size))
#define kP2CompactProxyGetRequestRecordRcsdSize(_rcsd_size)	(_rcsd_size)

//{{{ cut
typedef enum
{
	kP2CompactProxyGetRequestRecordCutIxPiid,
	kP2CompactProxyGetRequestRecordCutIxTimeout,
	kP2CompactProxyGetRequestRecordCutIxTsa,
	kP2CompactProxyGetRequestRecordCutIxOad,
	kP2CompactProxyGetRequestRecordCutIxRsd,
	kP2CompactProxyGetRequestRecordCutIxRcsd,
	kP2CompactProxyGetRequestRecordCutNum				
} P2CompactProxyGetRequestRecordCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxyGetRequestRecordCutNum];

	P2TsaPcut tsa;
	P2RsdPcut rsd;
	P2RcsdPcut rcsd;
} P2CompactProxyGetRequestRecordPcut;
#define kP2CompactProxyGetRequestRecordPcutDef { \
	kPcutDef, { kPcutItemDef }, kP2TsaPcutDef, kP2RsdPcutDef, kP2RcsdPcutDef }

cp_t P2CompactProxyGetRequestRecordPcutOpen(P2CompactProxyGetRequestRecordPcut *m);
cp_t P2CompactProxyGetRequestRecordPcutClose(P2CompactProxyGetRequestRecordPcut *m);
cp_t P2CompactProxyGetRequestRecordPcutValid(const P2CompactProxyGetRequestRecordPcut *m);

cp_t P2CompactProxyGetRequestRecordPcutOpenBase(Pcut *base);
cp_t P2CompactProxyGetRequestRecordPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxyGetRequestRecordTest(QTEST_ARG);
#endif

