#ifndef __P2_GET_RESPONSE_NORMAL_LIST_H_ 
#define __P2_GET_RESPONSE_NORMAL_LIST_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_a_result_normal.h"


#define kP2GetResponseNormalListName			"get_response_normal_list"
#define kP2GetResponseNormalListPiidAcd			"piid-acd"
#define kP2GetResponseNormalListSequenceOfAResultNormal	"sequence_of_a_result_normal"


// basic information
#define kP2GetResponseNormalListPiidAcdOffset		(0)
#define kP2GetResponseNormalListPiidAcdSize			(1)

#define kP2GetResponseNormalListSequenceOfAResultNormalOffset	(kP2GetResponseNormalListPiidAcdOffset + kP2GetResponseNormalListPiidAcdSize)
#define kP2GetResponseNormalListSequenceOfAResultNormalSize(_sequence_of_a_result_normal_size)		(_sequence_of_a_result_normal_size)

/*
GetResponseNormalList∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 若干个对象属性及结果 SEQUENCE OF A-ResultNormal
}
*/

// fill
cp_t P2GetResponseNormalListFillInit(Pfill *m, const P2PiidAcd *piid_acd, Pfill *fill_sequence_of_a_result_normal);

// test
cp_t P2GetResponseNormalListTest(QTEST_ARG);
#endif

