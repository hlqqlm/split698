#ifndef __P2_RCSD_H_ 
#define __P2_RCSD_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"

#include "p2_csd.h"
#include "p2_sequence_of_var_len.h"


#define kP2RcsdName					"rcsd"
#define kP2RcsdNameSequenceOfCsd	"sequence_of_csd"

//{{{ base
/*
记录列选择描述符RCSD（Record Column Selection Descriptor）的数据类型定义见表24 。
RCSD∷=SEQUENCE OF CSD 

RCSD 用于选择记录型对象属性中记录的某列
或某几列内容，即二维记录表的列选择。
当无 OAD 时，RCSD=0，即 SEQUENCE OF 的数据
项个数为 0，表示全选。
*/
#define kP2RcsdSequenceOfCsdOffset				(0)
#define kP2RcsdSequenceOfCsdSize(_soc_size)		(_soc_size)
//}}}


//{{{ cut
typedef enum
{
	kP2RcsdCutIxSequenceOfCsd,
	kP2RcsdCutNum				
} P2RcsdCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2RcsdCutNum];

	// sub
	P2CsdPcut csd_cut;
	P2SequenceOfVarLenPcut sequence_of_csd_cut;
} P2RcsdPcut;
#define kP2RcsdPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2CsdPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2RcsdPcutOpen(P2RcsdPcut *m);
cp_t P2RcsdPcutClose(P2RcsdPcut *m);
cp_t P2RcsdPcutValid(const P2RcsdPcut *m);

cp_t P2RcsdPcutOpenBase(Pcut *base);
cp_t P2RcsdPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2RcsdTest(QTEST_ARG);
#endif

