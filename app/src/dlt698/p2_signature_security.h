#ifndef __P2_SIGNATURE_SECURITY_H_
#define __P2_SIGNATURE_SECURITY_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_octet_string.h"




/*

SignatureSecurity∷=SEQUENCE
{
	密文 2 octet-string，
	客户机签名 2 octet-string
}

密文 2 为客户机对服务器产生的主站证书等数据加密信息。
客户机签名 2 为客户机对密文 2 的签名。
*/
#define kP2SignatureSecurityName				"signature_secrurity"
#define kP2SignatureSecurityNameCiphertext2		"ciphertext2"
#define kP2SignatureSecurityNameSign2			"sign2"


// basic information
#define kP2SignatureSecurityCiphertext2Offset					(0)
#define kP2SignatureSecurityCiphertext2Size(_ciphertext2_size)	(_ciphertext2_size)

#define kP2SignatureSecuritySign2Offset(_ciphertext2_size)		(kP2SignatureSecurityCiphertext2Offset + kP2SignatureSecurityCiphertext2Size(_ciphertext2_size))
#define kP2SignatureSecuritySign2Size(_sign2_size)				(_sign2_size)




//{{{ part
typedef enum
{
	kP2SignatureSecurityCutIxCiphertext2,
	kP2SignatureSecurityCutIxSign2,
	kP2SignatureSecurityCutNum				
} P2SignatureSecurityCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SignatureSecurityCutNum];

	P2OctetStringPcut ciphertext2;
	P2OctetStringPcut sign2;
} P2SignatureSecurityPcut;
#define kP2SignatureSecurityPcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2OctetStringPcutDef, kP2OctetStringPcutDef	\
}

cp_t P2SignatureSecurityPcutOpen(P2SignatureSecurityPcut *m);
cp_t P2SignatureSecurityPcutClose(P2SignatureSecurityPcut *m);
cp_t P2SignatureSecurityPcutValid(const P2SignatureSecurityPcut *m);

cp_t P2SignatureSecurityPcutOpenBase(Pcut *base);
cp_t P2SignatureSecurityPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2SignatureSecurityTest(QTEST_ARG);
#endif

