#ifndef __DLT698_45_SECURITY_RESPONSE_VERIFY_H_ 
#define __DLT698_45_SECURITY_RESPONSE_VERIFY_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_optional.h"
#include "p2_octet_string.h"

// 注意：本单元是特殊的"数据验证信息 CHOICE OPTIONAL", 即有optional，也有choice，注意：是先optional，后choice

#define kP2SecurityResponseVerifyName			"verify"
#define kP2SecurityResponseVerifyNameOptional	"security_response_verify_mac_optional"
#define kP2SecurityResponseVerifyNameChoice		"security_response_verify_mac_choice"
#define kP2SecurityResponseVerifyNameMac		"mac"


#define kP2SecurityResponseVerifyOptionalOffset					(0)
#define kP2SecurityResponseVerifyOptionalSize					(1)

#define kP2SecurityResponseVerifyChoiceOffset		(kP2SecurityResponseVerifyOptionalOffset + kP2SecurityResponseVerifyOptionalSize)
#define kP2SecurityResponseVerifyChoiceSize			(1)

#define kP2SecurityResponseVerifyMacOffset			(kP2SecurityResponseVerifyChoiceOffset + kP2SecurityResponseVerifyChoiceSize)
#define kP2SecurityResponseVerifyMacSize(_mac_size)	(_mac_size)


/*
数据验证信息 CHOICE OPTIONAL
{
数据MAC [0] MAC

*/

//{{{ choice
typedef enum 
{
	kP2SecurityResponseVerifyChoiceMac = 0,		// mac
} P2SecurityResponseVerifyChoiceT;

typedef enum 
{
	kP2SecurityResponseVerifyChoiceIxMac,	// mac
	kP2SecurityResponseVerifyChoiceNum
} P2SecurityResponseVerifyChoiceIxT;
int P2SecurityResponseVerifyChoice2Ix(uint8_t choice);
uint8_t P2SecurityResponseVerifyChoiceIx2Value(int choice_ix);
cp_t P2SecurityResponseVerifyChoiceIxValid(int choice_ix);
cp_t P2SecurityResponseVerifyChoiceValid(uint8_t choice);
//}}}


//{{{ part
typedef enum
{
	kP2SecurityResponseVerifyPartIxOptionalMac,
	kP2SecurityResponseVerifyPartNum				
} P2SecurityResponseVerifyPartIxT;


typedef struct
{
	Pcut base;
	PcutItem items[kP2SecurityResponseVerifyPartNum];

	P2OptionalPcut optional_part;
	P2OctetStringPcut mac_part;
} P2SecurityResponseVerifyPcut;
//参考 P2ClientApduPcut;
#define kP2SecurityResponseVerifyPcutDef {				\
	kPcutDef, { kPcutItemDef },						\
	kP2OptionalPcutDef, kP2OctetStringPcutDef			\
}

cp_t P2SecurityResponseVerifyPcutOpen(P2SecurityResponseVerifyPcut *m);
cp_t P2SecurityResponseVerifyPcutClose(P2SecurityResponseVerifyPcut *m);
cp_t P2SecurityResponseVerifyPcutValid(const P2SecurityResponseVerifyPcut *m);
//}}}


// pfill
// fill_mac		为NULL时，表示optional为0；否则optional为1。其为octet_string类型
cp_t P2SecurityResponseVerifyFillInit(Pfill *m, Pfill *fill_mac);

// test
cp_t P2SecurityResponseVerifyTest(QTEST_ARG);
#endif

