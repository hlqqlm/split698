#ifndef __P2_SECURITY_DATA_H_
#define __P2_SECURITY_DATA_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_rn.h"
#include "p2_octet_string.h"




/*
SecurityData∷=SEQUENCE
{
 服务器随机数 RN，
  服务器签名信息 octet-string
  }
*/
#define kP2SecurityDataName				"security_data"
#define kP2SecurityDataNameRandom		"random"
#define kP2SecurityDataNameSign			"sign"


//{{{ basic information
#define kP2SecurityDataRandomOffset					(0)
#define kP2SecurityDataRandomSize(_random_size)		(_random_size)

#define kP2SecurityDataSignOffset(_Random_size)		\
	(kP2SecurityDataRandomOffset + kP2SecurityDataRandomSize(_random_size))
#define kP2SecurityDataSignSize(_sign_size)			(_sign_size)
//}}}


//{{{ cut
typedef enum
{
	kP2SecurityDataCutIxRandom,
	kP2SecurityDataCutIxSign,
	kP2SecurityDataCutNum				
} P2SecurityDataCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SecurityDataCutNum];

	P2RnPcut random;
	P2OctetStringPcut sign;
} P2SecurityDataPcut;
#define kP2SecurityDataPcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2RnPcutDef, kP2OctetStringPcutDef	\
}

cp_t P2SecurityDataPcutOpen(P2SecurityDataPcut *m);
cp_t P2SecurityDataPcutClose(P2SecurityDataPcut *m);
cp_t P2SecurityDataPcutValid(const P2SecurityDataPcut *m);

cp_t P2SecurityDataPcutOpenBase(Pcut *base);
cp_t P2SecurityDataPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2SecurityDataTest(QTEST_ARG);
#endif

