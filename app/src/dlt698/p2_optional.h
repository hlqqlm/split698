#ifndef __P2_OPTIONAL_H_ 
#define __P2_OPTIONAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#define kP2OptionalName		"optional"

#define kP2OptionalOffset	(0)
#define kP2OptionalSize		(1)

#define kP2OptionalVarOffset			(kP2OptionalOffset + kP2OptionalSize)
#define kP2OptionalVarSize(_var_size)	(_var_size)


//{{{ optional
uint8_t P2OptionalValue(const char *whole);
//}}}


//{{{ cut
typedef enum
{
	kP2OptionalCutIxOptional,
	kP2OptionalCutIxVar,				// optional对应的变化部分
	kP2OptionalCutNum				
} P2OptionalCutIxT;

typedef struct
{
	Pcut base;		
	PcutItem items[kP2OptionalCutNum];

	Pcut *var_cut;
	const char *var_name;
} P2OptionalPcut;
#define kP2OptionalPcutDef {		\
	kPcutDef, { kPcutItemDef },	\
	NULL, NULL,						\
}

cp_t P2OptionalPcutOpen(P2OptionalPcut *m, Pcut *var_cut, const char *var_name);
cp_t P2OptionalPcutClose(P2OptionalPcut *m);
cp_t P2OptionalPcutValid(const P2OptionalPcut *m);
//}}}


//{{{ fill
typedef struct 
{
	PfillItem base;
	uint8_t optional;
} P2OptionalFillItemOptional;
cp_t P2OptionalFillItemProcessOptional(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2OptionalFillItemOptional(_name, _offset_fun, _offset_value, _optional) {			\
	PFILL_ITEM((_name), (_offset_fun), P2OptionalFillItemProcessOptional, (_offset_value), NULL),	\
	(_optional)			\
}
//}}}

// test
cp_t P2OptionalTest(QTEST_ARG);
#endif

