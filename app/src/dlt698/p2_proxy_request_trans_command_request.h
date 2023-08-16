#ifndef __P2_PROXY_REQUEST_TRANS_COMMAND_REQUEST_H_ 
#define __P2_PROXY_REQUEST_TRANS_COMMAND_REQUEST_H_ 



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_comdcb.h"
#include "p2_octet_string.h"


#define kP2ProxyRequestTransCommandRequestName				"proxy_request.trans_command_request"
#define kP2ProxyRequestTransCommandRequestNamePiid			"piid"
#define kP2ProxyRequestTransCommandRequestNameOad			"oad"
#define kP2ProxyRequestTransCommandRequestNameComdcb		"comdcb"
#define kP2ProxyRequestTransCommandRequestNameRxTimeoutSec	"rx_timeout_sec"
#define kP2ProxyRequestTransCommandRequestNameRxByteTimeoutMs		"rx_byte_timeout_ms"
#define kP2ProxyRequestTransCommandRequestNameCmd			"cmd"

//{{{ base
/*
Proxy
Trans Command Request ∷∷=SEQUENCE
{
服务序号 优先级 PIID
转 发 端口 对象 描述符 OAD
端口通信控制块 COMDCB
接收等待报文超时时间（秒） long unsigned
接收等待字节超时时间（毫秒） long unsigned
转发 至 目标端口 的 命令 octet string
}
*/
// Arn = Proxy Request TransCommandRequest
#define kP2ProxyRequestTransCommandRequestPiidOffset	(0)
#define kP2ProxyRequestTransCommandRequestPiidSize		(kP2PiidSize)

#define kP2ProxyRequestTransCommandRequestOadOffset		(kP2ProxyRequestTransCommandRequestPiidOffset + kP2ProxyRequestTransCommandRequestPiidSize)
#define kP2ProxyRequestTransCommandRequestOadSize		(OAD_MEM_SIZE)

#define kP2ProxyRequestTransCommandRequestComdcbOffset		(kP2ProxyRequestTransCommandRequestOadOffset + kP2ProxyRequestTransCommandRequestOadSize)
#define kP2ProxyRequestTransCommandRequestComdcbSize		(kP2ComdcbSize)

#define kP2ProxyRequestTransCommandRequestRxTimeoutSecOffset	(kP2ProxyRequestTransCommandRequestComdcbOffset + kP2ProxyRequestTransCommandRequestComdcbSize)
#define kP2ProxyRequestTransCommandRequestRxTimeoutSecSize		(kDlt698LongUnsignedSize)

#define kP2ProxyRequestTransCommandRequestRxByteTimeoutMsOffset	(kP2ProxyRequestTransCommandRequestRxTimeoutSecOffset + kP2ProxyRequestTransCommandRequestRxTimeoutSecSize)
#define kP2ProxyRequestTransCommandRequestRxByteTimeoutMsSize	(kDlt698LongUnsignedSize)

#define kP2ProxyRequestTransCommandRequestCmdOffset		(kP2ProxyRequestTransCommandRequestRxByteTimeoutMsOffset + kP2ProxyRequestTransCommandRequestRxByteTimeoutMsSize)
#define kP2ProxyRequestTransCommandRequestCmdSize(_cmd_size)	(_cmd_size)


#define kP2ProxyRequestTransCommandRequestWholeSize(_cmd_size)	(kP2ProxyRequestTransCommandRequestCmdOffset + kP2ProxyRequestTransCommandRequestCmdSize(_cmd_size))
//}}}

// rx_timeout_sec
uint16_t P2ProxyRequestTransCommandRequestRxTimeoutSecValue(const char *whole);

// rx_byte_timeout_ms
uint16_t P2ProxyRequestTransCommandRequestRxByteTimeoutMsValue(const char *whole);


//{{{ cut
typedef enum
{
	kP2ProxyRequestTransCommandRequestCutIxPiid,
	kP2ProxyRequestTransCommandRequestCutIxOad,
	kP2ProxyRequestTransCommandRequestCutIxComdcb,
	kP2ProxyRequestTransCommandRequestCutIxRxTimeoutSec,
	kP2ProxyRequestTransCommandRequestCutIxRxByteTimeoutMs,
	kP2ProxyRequestTransCommandRequestCutIxCmd,
	kP2ProxyRequestTransCommandRequestCutNum				
} P2ProxyRequestTransCommandRequestCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ProxyRequestTransCommandRequestCutNum];

	// sub
	P2ComdcbPcut comdcb_cut;
	P2OctetStringPcut cmd_cut;
} P2ProxyRequestTransCommandRequestPcut;
#define kP2ProxyRequestTransCommandRequestPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2ComdcbPcutDef, kP2OctetStringPcutDef			\
}

cp_t P2ProxyRequestTransCommandRequestPcutOpen(P2ProxyRequestTransCommandRequestPcut *m);
cp_t P2ProxyRequestTransCommandRequestPcutClose(P2ProxyRequestTransCommandRequestPcut *m);
cp_t P2ProxyRequestTransCommandRequestPcutValid(const P2ProxyRequestTransCommandRequestPcut *m);

cp_t P2ProxyRequestTransCommandRequestPcutOpenBase(Pcut *base);
cp_t P2ProxyRequestTransCommandRequestPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ProxyRequestTransCommandRequestTest(QTEST_ARG);
#endif

