#ifndef __DLT698_45_GET_RESPONSE_NORMAL_H_ 
#define __DLT698_45_GET_RESPONSE_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_a_result_normal.h"


#define kP2GetResponseNormalName			"get_response_normal"
#define kP2GetResponseNormalPiidAcd			"piid-acd"
#define kP2GetResponseNormalAResultNormal	kP2AResultNormalName


// basic information
#define kP2GetResponseNormalPiidAcdOffset		(0)
#define kP2GetResponseNormalPiidAcdSize			(1)

#define kP2GetResponseNormalAResultNormalOffset	(kP2GetResponseNormalPiidAcdOffset + kP2GetResponseNormalPiidAcdSize)
#define kP2GetResponseNormalAResultNormalSize(_a_result_normal_size)		(_a_result_normal_size)

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





cp_t P2GetResponseNormalFillInit(Pfill *m, const P2PiidAcd *piid_acd, Pfill *fill_a_result_normal);

// test
cp_t P2GetResponseNormalTest(QTEST_ARG);
#endif

