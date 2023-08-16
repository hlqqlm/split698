#ifndef __P2_PROXY_RESPONSE_TRANS_COMMAND_RESPONSE_H_ 
#define __P2_PROXY_RESPONSE_TRANS_COMMAND_RESPONSE_H_ 



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_trans_result.h"


#define kP2ProxyResponseTransCommandResponseName				"proxy_response.trans_command_response"
#define kP2ProxyResponseTransCommandResponseNamePiidAcd			"piid-acd"
#define kP2ProxyResponseTransCommandResponseNameOad				"oad"
#define kP2ProxyResponseTransCommandResponseNameTransResult		"trans_result"

//{{{ base
/*
ProxyTransCommandResponse∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  转发端口对象描述符 OAD，
   转发命令返回结果 TransResult
   }

TransResult∷=CHOICE
{
 转发结果 [0] DAR，
  返回数据 [1] octet-string
  }
*/
#define kP2ProxyResponseTransCommandResponsePiidAcdOffset	(0)
#define kP2ProxyResponseTransCommandResponsePiidAcdSize		(kP2PiidAcdSize)

#define kP2ProxyResponseTransCommandResponseOadOffset		(kP2ProxyResponseTransCommandResponsePiidAcdOffset + kP2ProxyResponseTransCommandResponsePiidAcdSize)
#define kP2ProxyResponseTransCommandResponseOadSize			(OAD_MEM_SIZE)

#define kP2ProxyResponseTransCommandResponseTransResultOffset		(kP2ProxyResponseTransCommandResponseOadOffset + kP2ProxyResponseTransCommandResponseOadSize)
#define kP2ProxyResponseTransCommandResponseTransResultSize(_tr_size)		(_tr_size)


#define kP2ProxyResponseTransCommandResponseWholeSize(_tr_size)	(kP2ProxyResponseTransCommandResponseTransResultOffset + kP2ProxyResponseTransCommandResponseTransResultSize(_tr_size))
//}}}



//{{{ cut
typedef enum
{
	kP2ProxyResponseTransCommandResponseCutIxPiidAcd,
	kP2ProxyResponseTransCommandResponseCutIxOad,
	kP2ProxyResponseTransCommandResponseCutIxTransResult,
	kP2ProxyResponseTransCommandResponseCutNum				
} P2ProxyResponseTransCommandResponseCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ProxyResponseTransCommandResponseCutNum];

	// sub
	P2TransResultPcut trans_result_cut;
} P2ProxyResponseTransCommandResponsePcut;
#define kP2ProxyResponseTransCommandResponsePcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2TransResultPcutDef						\
}

cp_t P2ProxyResponseTransCommandResponsePcutOpen(P2ProxyResponseTransCommandResponsePcut *m);
cp_t P2ProxyResponseTransCommandResponsePcutClose(P2ProxyResponseTransCommandResponsePcut *m);
cp_t P2ProxyResponseTransCommandResponsePcutValid(const P2ProxyResponseTransCommandResponsePcut *m);

cp_t P2ProxyResponseTransCommandResponsePcutOpenBase(Pcut *base);
cp_t P2ProxyResponseTransCommandResponsePcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ProxyResponseTransCommandResponseTest(QTEST_ARG);
#endif

