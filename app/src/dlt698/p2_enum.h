#ifndef __P2_ENUM_H_ 
#define __P2_ENUM_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"

#include "protocol/pcut.h"
#include "protocol/pfill.h"


// name
#define kP2EnumName		"enumerated"


// enum正常范围是0~0xff，用一个16位数，可以表达无效选择，不会和正常范围冲突。
#define kP2EnumInvalid	(0xffff)

#define kP2EnumOffset	(0)
#define kP2EnumSize		(1)


//{{{ enum
typedef struct P2EnumS
{
	uint8_t enumv;
	const char *name;
} P2Enum;
// 注：enum_list 必须按大小顺序排，加快搜索
int P2Enum2Ix(const P2Enum list[], int num, uint8_t enumv);
uint8_t P2EnumIx2Value(const P2Enum list[], int num, int enum_ix);
cp_t P2EnumIxValid(int num, int enum_ix);
cp_t P2EnumValid(const P2Enum list[], int num, uint8_t enumv);
const char *P2EnumStr(const P2Enum list[], int num, uint8_t enumv);

uint8_t P2EnumValue(const char *whole);
//}}}


//{{{ part
typedef enum
{
	kP2EnumPartIxEnum,
	kP2EnumPartNum				
} P2EnumPartIxT;



// 尽管enum比较简单，只有1个固定字节解析，可以不用P2EnumPcut作为pcut sub来进行解析。但用P2EnumPcut的优点有：
// 1. 隐藏一些enum公共的行为。
// 2. 和p2_choice类似，从而具有代码一致性/相似性。
// 3. 简化上层代码编写。上层基本只用定义enum_list即可。
typedef struct
{
	Pcut base;		
	PcutItem items[kP2EnumPartNum];
	
	const P2Enum *enum_list; 
	int enum_num;

	// uint16_t last_enum_value;					// 最后一次解析，对应的enum值
												// 正常范围0~0xff，定义为16位，可以描述invalid enum
	Pdo * const *enum_do_table;					// 长度为enum_num，下标对应enum选择后要执行的动作。
} P2EnumPcut;
#define kP2EnumPcutDef {			\
	kPcutDef, { kPcutItemDef },	\
	NULL, 0,						\
	NULL			\
}

// name		是enum的名称。例如"link_request_type"
cp_t P2EnumPcutOpen(P2EnumPcut *m, const char *enum_name, const P2Enum enum_list[], int enum_num);
cp_t P2EnumPcutClose(P2EnumPcut *m);
cp_t P2EnumPcutValid(const P2EnumPcut *m);

// 最后一次解析出的enum值
uint16_t P2EnumPcutLastEnum(P2EnumPcut *m);
// 某个enum选择后，要执行的动作
cp_t P2EnumDoTableSet(P2EnumPcut *m, Pdo * const *do_table);



//}}}


//{{{ fill
typedef struct 
{
	PfillItem base;
	uint16_t enum_value;
} P2EnumFillItemEnum;
cp_t P2EnumFillItemProcessEnum(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2EnumFillItemEnumDef(_name, _offset_fun, _offset_value, _enumv) {			\
	PFILL_ITEM(_name, (_offset_fun), P2EnumFillItemProcessEnum, (_offset_value), NULL),	\
	(_enumv)											\
}

//}}}


// test
void P2EnumTest(void);
#endif

