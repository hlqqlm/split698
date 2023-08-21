#ifndef __P2_OMD_DAR_DATA_OPTIONAL_H_ 
#define __P2_OMD_DAR_DATA_OPTIONAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_dar.h"
#include "p2_optional.h"
#include "p2_data_choice.h"


#define kP2OmdDarDataOptionalName				"omd_dar_data_optional"
#define kP2OmdDarDataOptionalNameOmd			"omd"
#define kP2OmdDarDataOptionalNameDar			"dar"
#define kP2OmdDarDataOptionalNameDataOptional	"data_optional"





//{{{ base
/*
用来描述dlt698_45中的ActionResponseNormalList中sequence of单元

ActionResponseNormalList∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  若干个对象方法操作结果 SEQUENCE OF
   {
   一个对象方法描述符 OMD，
   操作执行结果 DAR，
   操作返回数据 Data OPTIONAL
   }
   }
*/

// basic information
#define kP2OmdDarDataOptionalOmdOffset	(0)
#define kP2OmdDarDataOptionalOmdSize	(OMD_MEM_SIZE)

#define kP2OmdDarDataOptionalDarOffset	(kP2OmdDarDataOptionalOmdOffset + kP2OmdDarDataOptionalOmdSize)
#define kP2OmdDarDataOptionalDarSize	(kP2DarSize)

#define kP2OmdDarDataOptionalDataOptionalOffset	(kP2OmdDarDataOptionalDarOffset + kP2OmdDarDataOptionalDarSize)
#define kP2OmdDarDataOptionalDataOptionalSize(_data_optional_size)		(_data_optional_size)
//}}}


//{{{ cut
typedef enum
{
	kP2OmdDarDataOptionalCutIxOmd,
	kP2OmdDarDataOptionalCutIxDar,
	kP2OmdDarDataOptionalCutIxDataOptional,
	kP2OmdDarDataOptionalCutNum				
} P2OmdDarDataOptionalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2OmdDarDataOptionalCutNum];

	// sub
	P2DarPcut dar_cut;

	P2OptionalPcut data_optional_cut;
	P2DataChoicePcut data_cut;
} P2OmdDarDataOptionalPcut;
#define kP2OmdDarDataOptionalPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2DarPcutDef, kP2OptionalPcutDef, kP2DataChoicePcutDef			\
}

cp_t P2OmdDarDataOptionalPcutOpen(P2OmdDarDataOptionalPcut *m);
cp_t P2OmdDarDataOptionalPcutClose(P2OmdDarDataOptionalPcut *m);
cp_t P2OmdDarDataOptionalPcutValid(const P2OmdDarDataOptionalPcut *m);

cp_t P2OmdDarDataOptionalPcutOpenBase(Pcut *base);
cp_t P2OmdDarDataOptionalPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2OmdDarDataOptionalTest(QTEST_ARG);
#endif

