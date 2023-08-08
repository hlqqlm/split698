#ifndef __DLT698_45_GET_RESULT_H_ 
#define __DLT698_45_GET_RESULT_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_choice.h"



#define kP2GetResultName		"get_result"
#define kP2GetResultNameChoice	"choice"
#define kP2GetResultNameDar		"dar"
#define kP2GetResultNameData	"data"

// basic information
#define kP2GetResultChoiceOffset		(0)
#define kP2GetResultChoiceSize			(1)

#define kP2GetResultVarOffset	(kP2GetResultChoiceOffset + kP2GetResultChoiceSize)
#define kP2GetResultVarSize(_get_result_var_size)		(_get_result_var_size)

/*
Get-Result∷=CHOICE
{
 错误信息 [0] DAR，
 数据 [1] Data
}
*/
typedef struct 
{
	PfillItem base;
	uint16_t choice;
} P2GetResultFillItemChoice;
cp_t P2GetResultFillItemProcessChoice(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2GetResultFillItemChoiceDef(_offset_fun, _offset_value, _choice) {			\
	PFILL_ITEM(kP2GetResultNameChoice, _offset_fun, P2GetResultFillItemProcessChoice, _offset_value, NULL),	\
	_choice											\
}

typedef struct 
{
	PfillItem base;
	uint8_t dar;
} P2GetResultFillItemDar;
cp_t P2GetResultFillItemProcessDar(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2GetResultFillItemDarDef(_dar) {			\
	PFILL_ITEM(kP2GetResultNameDar, PfillItemOffsetFix, P2GetResultFillItemProcessDar, kP2GetResultVarOffset, NULL),	\
	(_dar)											\
}


typedef struct 
{
	PfillItem base;
} P2GetResultFillItemData;
#define kP2GetResultFillItemDataDef {			\
	PFILL_ITEM(kP2GetResultNameData, PfillItemOffsetFix, PfillItemProcessBySub, 1, NULL),	\
}


// if sub_octet_string=NULL, choice=[0] DAR
// if sub_octet_string!=NULL, choice=[1] Data
cp_t P2GetResultFillInit(Pfill *m, uint8_t dar, Pfill *sub_data);

// test
cp_t P2GetResultTest(QTEST_ARG);
#endif

