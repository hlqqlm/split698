#ifndef __P2_CHOICE_H_ 
#define __P2_CHOICE_H_ 

#include "qos/qcp.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

// choice正常范围是0~0xff，用一个16位数，可以表达无效选择，不会和正常范围冲突。
#define kP2ChoiceInvalid	(0xffff)


#define kP2ChoiceOffset		(0)
#define kP2ChoiceSize		(1)

#define kP2ChoiceVarOffset			(kP2ChoiceOffset + kP2ChoiceSize)
#define kP2ChoiceVarSize(_var_size)	(_var_size)

// name
#define kP2ChoiceNameChoice		"choice"
#define kP2ChoiceNameVar		"var"

//{{{ choice
typedef struct P2ChoiceS
{
	uint8_t choice;
	const char *name;
} P2Choice;
// 注：choice_list 必须按大小顺序排，加快搜索
int P2Choice2Ix(const P2Choice list[], int num, uint8_t choice);
uint8_t P2ChoiceIx2Value(const P2Choice list[], int num, int choice_ix);
cp_t P2ChoiceIxValid(int num, int choice_ix);
cp_t P2ChoiceValid(const P2Choice list[], int num, uint8_t choice);
const char *P2ChoiceStr(const P2Choice list[], int num, uint8_t choice);

uint8_t P2ChoiceValue(const char *whole);
//}}}


//{{{ part
typedef enum
{
	kP2ChoicePartIxChoice,
	kP2ChoicePartIxVar,				// choice对应的变化部分
	kP2ChoicePartNum				
} P2ChoicePartIxT;

typedef struct
{
	Pcut base;		
	PcutItem items[kP2ChoicePartNum];
	
	const P2Choice *choice_list; 
	int choice_num;

	uint16_t choice;							// 正常choice范围0~0xff，定义为16位，可以描述invalid choice
	const PcutFactoryInfo *var_factory_info;	// 长度一定是choice_num
	Pcut *var_part;							// 指向choice后动态生成的指针
	Pdo * const *var_do_table;					// 长度为choice_num，下标对应choice选择后要执行的动作。
												// 名称中var强调这是var对应的行为
} P2ChoicePcut;
#define kP2ChoicePcutDef {			\
	kPcutDef, { kPcutItemDef },	\
	NULL, 0,						\
	kP2ChoiceInvalid, NULL,	NULL, NULL	\
}

// name		是choice的名称。例如“client_apdu_choice”
cp_t P2ChoicePcutOpen(P2ChoicePcut *m, const char *choice_name, const P2Choice choice_list[], int choice_num
		, const PcutFactoryInfo *var_factory_info);
cp_t P2ChoicePcutClose(P2ChoicePcut *m);
cp_t P2ChoicePcutValid(const P2ChoicePcut *m);

uint16_t P2ChoicePcutChoice(P2ChoicePcut *m);
Pcut *P2ChoicePcutVar(P2ChoicePcut *m);
// 某个choice选择后，var要执行的动作
cp_t P2ChoiceVarDoTableSet(P2ChoicePcut *m, Pdo * const *var_do_table);
//void P2ChoiceSetDo(P2ChoicePcut *m, int choice_ix, Pdo *var_do_table);
//cp_t P2ChoicePcutFactoryVar(P2ChoicePcut *m, const char *whole);
//}}}


//{{{ fill
typedef struct 
{
	PfillItem base;
	uint16_t choice;
} P2ChoiceFillItemChoice;
cp_t P2ChoiceFillItemProcessChoice(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2ChoiceFillItemChoiceDef(_name, _offset_fun, _offset_value, _choice) {			\
	PFILL_ITEM(_name, (_offset_fun), P2ChoiceFillItemProcessChoice, (_offset_value), NULL),	\
	(_choice)											\
}

//}}}



// test
void P2ChoiceTest(void);
#endif

