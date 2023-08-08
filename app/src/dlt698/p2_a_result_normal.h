#ifndef __DLT698_45_A_RESULT_NORMAL_H_ 
#define __DLT698_45_A_RESULT_NORMAL_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_get_result.h"


/*
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

// name
#define kP2AResultNormalName			"a_result_normal"
#define kP2AResultNormalNameOad			"oad"
#define kP2AResultNormalNameGetResult	kP2GetResultName

// basic information
#define kP2AResultNormalOadOffset		(0)
#define kP2AResultNormalOadSize			(OAD_MEM_SIZE)

#define kP2AResultNormalGetResultOffset	(kP2AResultNormalOadOffset + kP2AResultNormalOadSize)
#define kP2AResultNormalGetResultSize(_get_result_size)		(_get_result_size)





// fill
typedef struct 
{
	PfillItem base;
	OadT oad;
} P2AResultNormalFillItemOad;
cp_t P2AResultNormalFillItemProcessOad(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2AResultNormalFillItemOadDef(_offset_fun, _offset_value, _oad) {			\
	PFILL_ITEM(kP2AResultNormalNameOad, (_offset_fun), P2AResultNormalFillItemProcessOad, (_offset_value), NULL),	\
	(_oad)								\
}




typedef struct 
{
	PfillItem base;
} P2AResultNormalFillItemGetResult;
cp_t P2AResultNormalFillItemProcessGetResult(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2AResultNormalFillItemGetResultDef {			\
	PFILL_ITEM(kP2AResultNormalNameGetResult, PfillItemOffsetFix, PfillItemProcessBySub, kP2AResultNormalGetResultOffset, NULL),	\
}


// if data=NULL, choice=[0] DAR
// if data!=NULL, choice=[1] Data
cp_t P2AResultNormalFillInit(Pfill *m, OadT oad, Pfill *sub_get_result);


// test
cp_t P2AResultNormalTest(QTEST_ARG);
#endif

