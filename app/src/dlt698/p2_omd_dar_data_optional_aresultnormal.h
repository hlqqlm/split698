#ifndef __P2_OMD_DAR_DATA_OPTIONAL_ARESULTNORMAL_H_ 
#define __P2_OMD_DAR_DATA_OPTIONAL_ARESULTNORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_dar.h"
#include "p2_optional.h"
#include "p2_data_choice.h"
#include "p2_a_result_normal.h"


#define kP2OmdDarDataOptionalAresultnormalName				"omd_dar_data_optional"
#define kP2OmdDarDataOptionalAresultnormalNameOmd			"omd"
#define kP2OmdDarDataOptionalAresultnormalNameDar			"dar"
#define kP2OmdDarDataOptionalAresultnormalNameDataOptional	"data_optional"
#define kP2OmdDarDataOptionalAresultnormalNameAresultnormal	"a_result_normal"





//{{{ base
/*
用来描述dlt698_45中的ActionThenGetResponseNormalList中sequence of单元

ActionThenGetResponseNormalList∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  操作若干个对象方法后读取属性的结果 SEQUENCE OF
   {
   一个设置的对象方法描述符 OMD，
   操作执行结果 DAR，
    操作返回数据 Data OPTIONAL，
	 一个对象属性及结果 A-ResultNormal
	 }
	 }
*/

// basic information
#define kP2OmdDarDataOptionalAresultnormalOmdOffset	(0)
#define kP2OmdDarDataOptionalAresultnormalOmdSize	(OMD_MEM_SIZE)

#define kP2OmdDarDataOptionalAresultnormalDarOffset	(kP2OmdDarDataOptionalAresultnormalOmdOffset + kP2OmdDarDataOptionalAresultnormalOmdSize)
#define kP2OmdDarDataOptionalAresultnormalDarSize	(kP2DarSize)

#define kP2OmdDarDataOptionalAresultnormalDataOptionalOffset	(kP2OmdDarDataOptionalAresultnormalDarOffset + kP2OmdDarDataOptionalAresultnormalDarSize)
#define kP2OmdDarDataOptionalAresultnormalDataOptionalSize(_data_optional_size)		(_data_optional_size)

#define kP2OmdDarDataOptionalAresultnormalAresultnormalOffset(_data_optional_size)	(kP2OmdDarDataOptionalAresultnormalDataOptionalOffset + kP2OmdDarDataOptionalAresultnormalDataOptionalSize(_data_optional_size))
#define kP2OmdDarDataOptionalAresultnormalAresultnormalSize(_aresultnormal_size)		(_aresultnormal_size)
//}}}


//{{{ cut
typedef enum
{
	kP2OmdDarDataOptionalAresultnormalCutIxOmd,
	kP2OmdDarDataOptionalAresultnormalCutIxDar,
	kP2OmdDarDataOptionalAresultnormalCutIxDataOptional,
	kP2OmdDarDataOptionalAresultnormalCutIxAresultnormal,
	kP2OmdDarDataOptionalAresultnormalCutNum				
} P2OmdDarDataOptionalAresultnormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2OmdDarDataOptionalAresultnormalCutNum];

	// sub
	P2DarPcut dar_cut;

	P2OptionalPcut data_optional_cut;
	P2DataChoicePcut data_cut;
	P2AResultNormalPcut aresultnormal_cut;
} P2OmdDarDataOptionalAresultnormalPcut;
#define kP2OmdDarDataOptionalAresultnormalPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2DarPcutDef, kP2OptionalPcutDef, kP2DataChoicePcutDef			\
}

cp_t P2OmdDarDataOptionalAresultnormalPcutOpen(P2OmdDarDataOptionalAresultnormalPcut *m);
cp_t P2OmdDarDataOptionalAresultnormalPcutClose(P2OmdDarDataOptionalAresultnormalPcut *m);
cp_t P2OmdDarDataOptionalAresultnormalPcutValid(const P2OmdDarDataOptionalAresultnormalPcut *m);

cp_t P2OmdDarDataOptionalAresultnormalPcutOpenBase(Pcut *base);
cp_t P2OmdDarDataOptionalAresultnormalPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2OmdDarDataOptionalAresultnormalTest(QTEST_ARG);
#endif

