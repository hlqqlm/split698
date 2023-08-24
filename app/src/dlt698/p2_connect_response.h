#ifndef __P2_CONNECT_RESPONSE_H_ 
#define __P2_CONNECT_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"
#include "qdlt698.h"
#include "p2_factory_version.h"
#include "p2_connect_response_info.h"


#define kP2ConnectResponseName			"connect_response"


//{{{ base
/*
CONNECT-Response∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  服务器厂商版本信息 FactoryVersion，
   商定的协议版本号 long-unsigned，
   商定的协议一致性块 ProtocolConformance，
   商定的功能一致性块 FunctionConformance，
   服务器发送帧最大尺寸 long-unsigned，
   服务器接收帧最大尺寸 long-unsigned，
   服务器接收帧最大窗口尺寸 unsigned，
    服务器最大可处理帧尺寸 long-unsigned，
	商定的应用连接超时时间 double-long-unsigned，
	连接响应对象 ConnectResponseInfo
	}
*/
#define kP2ConnectResponsePiidAcdOffset					(0)
#define kP2ConnectResponsePiidAcdSize					(kP2PiidSize)

#define kP2ConnectResponseFactoryVersionOffset			(kP2ConnectResponsePiidAcdOffset + kP2ConnectResponsePiidAcdSize)
#define kP2ConnectResponseFactoryVersionSize			(kP2FactoryVersionSize)

#define kP2ConnectResponseProtocolVersionOffset			\
	(kP2ConnectResponseFactoryVersionOffset + kP2ConnectResponseFactoryVersionSize)
#define kP2ConnectResponseProtocolVersionSize				(kDlt698LongUnsignedSize)

#define kP2ConnectResponseProtocolConformanceOffset	\
	(kP2ConnectResponseProtocolVersionOffset + kP2ConnectResponseProtocolVersionSize)
#define kP2ConnectResponseProtocolConformanceSize	(16)

#define kP2ConnectResponseFunctionConformanceOffset	\
	(kP2ConnectResponseProtocolConformanceOffset + kP2ConnectResponseProtocolConformanceSize)
#define kP2ConnectResponseFunctionConformanceSize	(16)

#define kP2ConnectResponseServerTxSizeMaxOffset		\
	(kP2ConnectResponseFunctionConformanceOffset + kP2ConnectResponseFunctionConformanceSize)
#define kP2ConnectResponseServerTxSizeMaxSize				(kDlt698LongUnsignedSize)

#define kP2ConnectResponseServerRxSizeMaxOffset		\
	(kP2ConnectResponseServerTxSizeMaxOffset + kP2ConnectResponseServerTxSizeMaxSize)
#define kP2ConnectResponseServerRxSizeMaxSize				(kDlt698LongUnsignedSize)

#define kP2ConnectResponseFrameNumMaxOffset	\
	(kP2ConnectResponseServerRxSizeMaxOffset + kP2ConnectResponseServerRxSizeMaxSize)
#define kP2ConnectResponseFrameNumMaxSize				(kDlt698UnsignedSize)

#define kP2ConnectResponseFrameSizeMaxOffset	\
	(kP2ConnectResponseFrameNumMaxOffset + kP2ConnectResponseFrameNumMaxSize)
#define kP2ConnectResponseFrameSizeMaxSize				(kDlt698LongUnsignedSize)

#define kP2ConnectResponseConnectionTimeoutOffset		\
	(kP2ConnectResponseFrameSizeMaxOffset + kP2ConnectResponseFrameSizeMaxSize)
#define kP2ConnectResponseConnectionTimeoutSize				(kDlt698DoubleLongUnsignedSize)

#define kP2ConnectResponseConnectResponseInfoOffset	\
	(kP2ConnectResponseConnectionTimeoutOffset + kP2ConnectResponseConnectionTimeoutSize)
#define kP2ConnectResponseConnectResponseInfoSize(_cri_size)	(_cri_size)
//}}}


//{{{ cut
typedef enum
{
	kP2ConnectResponseCutIxPiidAcd,
	kP2ConnectResponseCutIxFactoryVersion,
	kP2ConnectResponseCutIxProtocolVersion,
	kP2ConnectResponseCutIxProtocolConformance,
	kP2ConnectResponseCutIxFunctionConformance,
	kP2ConnectResponseCutIxServerTxSizeMax,
	kP2ConnectResponseCutIxServerRxSizeMax,
	kP2ConnectResponseCutIxFrameNumMax,
	kP2ConnectResponseCutIxFrameSizeMax,
	kP2ConnectResponseCutIxConnectionTimeout,
	kP2ConnectResponseCutIxConnectResponseInfo,
	kP2ConnectResponseCutNum				
} P2ConnectResponseCutIxT;

typedef struct 
{
	Pcut base;
	PcutItem items[kP2ConnectResponseCutNum];

	P2FactoryVersionPcut factory_version_cut;
	P2ConnectResponseInfoPcut connect_response_info;
} P2ConnectResponsePcut;
#define kP2ConnectResponsePcutDef { kPcutDef, { kPcutItemDef }, \
	kP2FactoryVersionPcutDef, kP2ConnectResponseInfoPcutDef }

cp_t P2ConnectResponsePcutOpen(P2ConnectResponsePcut *m);
cp_t P2ConnectResponsePcutClose(P2ConnectResponsePcut *m);
cp_t P2ConnectResponsePcutValid(const P2ConnectResponsePcut *m);

cp_t P2ConnectResponsePcutOpenBase(Pcut *base);
cp_t P2ConnectResponsePcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ConnectResponseTest(QTEST_ARG);
#endif

