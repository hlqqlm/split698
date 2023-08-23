#ifndef __P2_CONNECT_REQUEST_H_ 
#define __P2_CONNECT_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"
#include "qdlt698.h"
#include "p2_connect_mechanism_info_choice.h"



#define kP2ConnectRequestName			"connect_request"

/*
Connect-Request∷=SEQUENCE
{
	服务序号-优先级           PIID，
	期望的应用层协议版本号    long-unsigned，
	期望的协议一致性块        ProtocolConformance，
	期望的功能一致性块        FunctionConformance，
	客户机发送帧最大尺寸      long-unsigned，
	客户机接收帧最大尺寸      long-unsigned，
	客户机接收帧最大窗口尺寸  unsigned，
	客户机最大可处理APDU尺寸 long-unsigned，
	期望的应用连接超时时间    double-long-unsigned，
	认证请求对象              ConnectMechanismInfo
}
*/
#define kP2ConnectRequestPiidOffset					(0)
#define kP2ConnectRequestPiidSize					(kP2PiidSize)

#define kP2ConnectRequestAppProtocolVersionOffset	(kP2ConnectRequestPiidOffset + kP2ConnectRequestPiidSize)
#define kP2ConnectRequestAppProtocolVersionSize		(kDlt698LongUnsignedSize)

#define kP2ConnectRequestProtocolConformanceOffset	(kP2ConnectRequestAppProtocolVersionOffset + kP2ConnectRequestAppProtocolVersionSize)
#define kP2ConnectRequestProtocolConformanceSize	(8)

#define kP2ConnectRequestFunctionConformanceOffset	(kP2ConnectRequestProtocolConformanceOffset + kP2ConnectRequestProtocolConformanceSize)
#define kP2ConnectRequestFunctionConformanceSize	(16)

#define kP2ConnectRequestClientTxSizeMaxOffset		(kP2ConnectRequestFunctionConformanceOffset + kP2ConnectRequestFunctionConformanceSize)
#define kP2ConnectRequestClientTxSizeMaxSize		(kDlt698LongUnsignedSize)

#define kP2ConnectRequestClientRxSizeMaxOffset		(kP2ConnectRequestClientTxSizeMaxOffset + kP2ConnectRequestClientTxSizeMaxSize)
#define kP2ConnectRequestClientRxSizeMaxSize		(kDlt698LongUnsignedSize)

#define kP2ConnectRequestFrameNumMaxOffset			(kP2ConnectRequestClientRxSizeMaxOffset + kP2ConnectRequestClientRxSizeMaxSize)
#define kP2ConnectRequestFrameNumMaxSize			(kDlt698UnsignedSize)

#define kP2ConnectRequestApduSizeMaxOffset			(kP2ConnectRequestFrameNumMaxOffset + kP2ConnectRequestFrameNumMaxSize)
#define kP2ConnectRequestApduSizeMaxSize			(kDlt698LongUnsignedSize)

#define kP2ConnectRequestConnectionTimeoutOffset	(kP2ConnectRequestApduSizeMaxOffset + kP2ConnectRequestApduSizeMaxSize)
#define kP2ConnectRequestConnectionTimeoutSize		(kDlt698DoubleLongUnsignedSize)

#define kP2ConnectRequestConnectMechanismInfoOffset	(kP2ConnectRequestConnectionTimeoutOffset + kP2ConnectRequestConnectionTimeoutSize)
#define kP2ConnectRequestConnectMechanismInfoSize(_cmi_size)	(_cmi_size)


uint8_t P2ConnectRequestPiid(const char *whole);
uint16_t P2ConnectRequestAppProtocolVersion(const char *whole);



//{{{ cut
typedef enum
{
	kP2ConnectRequestCutIxPiid,
	kP2ConnectRequestCutIxAppProtocolVersion,
	kP2ConnectRequestCutIxProtocolConformance,
	kP2ConnectRequestCutIxFunctionConformance,
	kP2ConnectRequestCutIxClientTxSizeMax,
	kP2ConnectRequestCutIxClientRxSizeMax,
	kP2ConnectRequestCutIxFrameNumMax,
	kP2ConnectRequestCutIxApduSizeMax,
	kP2ConnectRequestCutIxConnectionTimeout,
	kP2ConnectRequestCutIxConnectMechanismInfo,
	kP2ConnectRequestCutNum				
} P2ConnectRequestCutIxT;

typedef struct 
{
	Pcut base;
	PcutItem items[kP2ConnectRequestCutNum];

	P2ConnectMechanismInfoChoicePcut cmi;
} P2ConnectRequestPcut;
#define kP2ConnectRequestPcutDef { kPcutDef, { kPcutItemDef }, kP2ConnectMechanismInfoChoicePcutDef }

//cp_t P2ConnectRequestPcutOpen(P2ConnectRequestPcut *m);
//cp_t P2ConnectRequestPcutOpen(PcutBase *base);
cp_t P2ConnectRequestPcutOpen(P2ConnectRequestPcut *m);
cp_t P2ConnectRequestPcutClose(P2ConnectRequestPcut *m);
cp_t P2ConnectRequestPcutValid(const P2ConnectRequestPcut *m);

cp_t P2ConnectRequestPcutOpenBase(Pcut *base);
cp_t P2ConnectRequestPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ConnectRequestTest(QTEST_ARG);
#endif

