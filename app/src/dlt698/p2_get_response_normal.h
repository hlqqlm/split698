#ifndef __P2_GET_RESPONSE_NORMAL_H_ 
#define __P2_GET_RESPONSE_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_a_result_normal.h"


#define kP2GetResponseNormalName				"get_response_normal"
#define kP2GetResponseNormalNamePiidAcd			"piid-acd"
#define kP2GetResponseNormalNameAResultNormal	kP2AResultNormalName





//{{{ base
/*
GetResponseNormal∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 一个对象属性及结果 A-ResultNormal
}
PIID-ACD——见 0。
A-ResultNormal∷=SEQUENCE
{
 对象属性描述符 OAD，
 读取结果 Get-Result
}
OAD——见 0。
Get-Result∷=CHOICE
{
 错误信息 [0] DAR，
 数据 [1] Data
}
*/

// basic information
#define kP2GetResponseNormalPiidAcdOffset		(0)
#define kP2GetResponseNormalPiidAcdSize			(1)

#define kP2GetResponseNormalAResultNormalOffset	(kP2GetResponseNormalPiidAcdOffset + kP2GetResponseNormalPiidAcdSize)
#define kP2GetResponseNormalAResultNormalSize(_a_result_normal_size)		(_a_result_normal_size)
//}}}


//{{{ cut
typedef enum
{
	kP2GetResponseNormalCutIxPiidAcd,
	kP2GetResponseNormalCutIxAResultNormal,
	kP2GetResponseNormalCutNum				
} P2GetResponseNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2GetResponseNormalCutNum];

	// sub
	P2AResultNormalPcut a_result_normal_cut;
} P2GetResponseNormalPcut;
#define kP2GetResponseNormalPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2AResultNormalPcutDef						\
}

cp_t P2GetResponseNormalPcutOpen(P2GetResponseNormalPcut *m);
cp_t P2GetResponseNormalPcutClose(P2GetResponseNormalPcut *m);
cp_t P2GetResponseNormalPcutValid(const P2GetResponseNormalPcut *m);

cp_t P2GetResponseNormalPcutOpenBase(Pcut *base);
cp_t P2GetResponseNormalPcutCloseBase(Pcut *base);
//}}}



cp_t P2GetResponseNormalFillInit(Pfill *m, const P2PiidAcd *piid_acd, Pfill *fill_a_result_normal);

// test
cp_t P2GetResponseNormalTest(QTEST_ARG);
#endif

