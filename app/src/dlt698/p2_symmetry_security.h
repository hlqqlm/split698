#ifndef __P2_SYMMETRY_SECURITY_H_
#define __P2_SYMMETRY_SECURITY_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_octet_string.h"




/*
SymmetrySecurity∷=SEQUENCE
{
 密文 1 octet-string，
  客户机签名 1 octet-string
}

密文 1 为对客户机产生的随机数加密得到的密文。
*/
#define kP2SymmetrySecurityName					"symmetry_secrurity"
#define kP2SymmetrySecurityNameCiphertext1		"ciphertext1"
#define kP2SymmetrySecurityNameSign1			"sign1"


//{{{ basic information
#define kP2SymmetrySecurityCiphertext1Offset					(0)
#define kP2SymmetrySecurityCiphertext1Size(_ciphertext1_size)	(_ciphertext1_size)

#define kP2SymmetrySecuritySign1Offset(_ciphertext1_size)		(kP2SymmetrySecurityCiphertext1Offset + kP2SymmetrySecurityCiphertext1Size(_ciphertext1_size))
#define kP2SymmetrySecuritySign1Size(_sign1_size)				(_sign1_size)
//}}}


//{{{ cut
typedef enum
{
	kP2SymmetrySecurityCutIxCiphertext1,
	kP2SymmetrySecurityCutIxSign1,
	kP2SymmetrySecurityCutNum				
} P2SymmetrySecurityCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SymmetrySecurityCutNum];

	P2OctetStringPcut ciphertext1;
	P2OctetStringPcut sign1;
} P2SymmetrySecurityPcut;
#define kP2SymmetrySecurityPcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2OctetStringPcutDef, kP2OctetStringPcutDef	\
}

cp_t P2SymmetrySecurityPcutOpen(P2SymmetrySecurityPcut *m);
cp_t P2SymmetrySecurityPcutClose(P2SymmetrySecurityPcut *m);
cp_t P2SymmetrySecurityPcutValid(const P2SymmetrySecurityPcut *m);

cp_t P2SymmetrySecurityPcutOpenBase(Pcut *base);
cp_t P2SymmetrySecurityPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2SymmetrySecurityTest(QTEST_ARG);
#endif

