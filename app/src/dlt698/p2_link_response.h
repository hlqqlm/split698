#ifndef __P2_LINK_RESPONSE_H_ 
#define __P2_LINK_RESPONSE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_enum.h"
#include "qdlt698.h"




/*
LINK-Response∷=SEQUENCE
{
	服务序号-优先级 PIID，
	结果 Result，
	请求时间 date_time，
	收到时间 date_time，
	响应时间 date_time，
}

 PIID——见 7.3.3 。
 date_time——见 0。
 时钟可信标志——用于表示响应方的时钟是否可信，
 bit7=0：不可信，bit7=1：可信。
 bit0„bit2——二进制编码表示结果，0：成功，1：地址重复，2：非法设备，3：容量不足，其它值：保留。

 Result∷=bit-string（SIZE（8））
 {
	 bit7（时钟可信标志） （0），
	 bit6（保留） （1），
	 bit5（保留） （2），
	 bit4（保留） （3），
	 bit3（保留） （4），
	 bit2（结果） （5），
	 bit1（结果） （6），
	 bit0（结果） （7）
 }
*/
#define kP2LinkResponseName					"link_response"
#define kP2LinkResponseNamePiid				"piid"
#define kP2LinkResponseNameResult			"result"
#define kP2LinkResponseNameRequestDatetime	"request_datetime"
#define kP2LinkResponseNameReceiveDatetime	"receive_datetime"
#define kP2LinkResponseNameResponseDatetime	"response_datetime"


// basic information
#define kP2LinkResponsePiidOffset				(0)
#define kP2LinkResponsePiidSize					(kP2PiidSize)

#define kP2LinkResponseResultOffset				(kP2LinkResponsePiidOffset + kP2LinkResponsePiidSize)
#define kP2LinkResponseResultSize				(1)

#define kP2LinkResponseRequestDatetimeOffset	(kP2LinkResponseResultOffset + kP2LinkResponseResultSize)
#define kP2LinkResponseRequestDatetimeSize		(kDlt698DatetimeSize)

#define kP2LinkResponseReceiveDatetimeOffset	(kP2LinkResponseRequestDatetimeOffset + kP2LinkResponseRequestDatetimeSize)
#define kP2LinkResponseReceiveDatetimeSize		(kDlt698DatetimeSize)

#define kP2LinkResponseResponseDatetimeOffset	(kP2LinkResponseReceiveDatetimeOffset + kP2LinkResponseReceiveDatetimeSize)
#define kP2LinkResponseResponseDatetimeSize		(kDlt698DatetimeSize)


//{{{ result
#define kP2LinkResponseResultBit7Mask		(0x80)
#define kP2LinkResponseResultBit0To2Mask	(0x07)
const char *P2LinkResponseResultBit7Str(uint8_t bit7);
const char *P2LinkResponseResultBit0To2Str(uint8_t bit0_2);
uint8_t P2LinkResponseResult(const char *whole);
//}}}


//{{{ cut
typedef enum
{
	kP2LinkResponseCutIxPiid,
	kP2LinkResponseCutIxResult,
	kP2LinkResponseCutIxRequestDatetime,
	kP2LinkResponseCutIxReceiveDatetime,
	kP2LinkResponseCutIxResponseDatetime,
	kP2LinkResponseCutNum				
} P2LinkResponseCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2LinkResponseCutNum];
} P2LinkResponsePcut;
#define kP2LinkResponsePcutDef {				\
	kPcutDef, { kPcutItemDef }					\
}

cp_t P2LinkResponsePcutOpen(P2LinkResponsePcut *m);
cp_t P2LinkResponsePcutClose(P2LinkResponsePcut *m);
cp_t P2LinkResponsePcutValid(const P2LinkResponsePcut *m);

cp_t P2LinkResponsePcutOpenBase(Pcut *base);
cp_t P2LinkResponsePcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2LinkResponseTest(QTEST_ARG);
#endif

