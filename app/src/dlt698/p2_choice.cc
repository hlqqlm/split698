/*
This program is licensed under the GPLv2 License:
Copyright 2023 Huanglin <hlqqlm@qq.com>


This file is part of Split698.

Split698 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

Split698 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Split698. If not, see <https://www.gnu.org/licenses/>.

20230407-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 choice 部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qos/qmem.h"
#include "qos/qstr.h"
#include "p2_choice.h"
#include "p2_choice.xcp.h"
//#define this_file_id	0x1696bff6	// echo -n dlt698_45_choice.c | rhash --simple -


#define TEST_EN				(0)
#define kThisCutNum			(kP2ChoiceCutNum)

//{{{ factory_var
static cp_t PcutChoiceValid(const P2ChoicePcut *m, uint8_t choice);
static cp_t CutCloseVar(P2ChoicePcut *m);
static cp_t CutFactoryVar(P2ChoicePcut *m, uint8_t choice);

static cp_t CutFactoryVarOnWhole(P2ChoicePcut *m, const char *whole)
{
	dve(P2ChoicePcutValid(m));
	const uint8_t choice = P2ChoiceValue(whole);
	ifer(PcutChoiceValid(m, choice));

	ifer(CutFactoryVar(m, choice));
	return 0;
}
//}}}


// {{{ choice
int P2Choice2Ix(const P2Choice list[], int num, uint8_t choice)
{
	for (int i = 0; i < num; ++i)
	{
		const uint8_t curr = list[i].choice;
		if (choice == curr)
			return i;		
		// list是从小往大排序的
		if (choice < curr)
			return -1;
	}

	return -1;
}
uint8_t P2ChoiceIx2Value(const P2Choice list[], int num, int choice_ix)
{
	dve(P2ChoiceIxValid(num, choice_ix));
	return list[choice_ix].choice;
}
cp_t P2ChoiceIxValid(int num, int choice_ix)
{
	ifbr(0 <= choice_ix);
	ifbr(choice_ix < num);
	return 0;
}
cp_t P2ChoiceValid(const P2Choice list[], int num, uint8_t choice)
{
	const int choice_ix = P2Choice2Ix(list, num, choice);
	ifer(P2ChoiceIxValid(num, choice_ix));
	return 0;
}
const char *P2ChoiceStr(const P2Choice list[], int num, uint8_t choice)
{
	const int choice_ix = P2Choice2Ix(list, num, choice);
	if (0 != P2ChoiceIxValid(num, choice_ix))
		return "unknow";
	return list[choice_ix].name;
}

uint8_t P2ChoiceValue(const char *whole)
{
	return whole[kP2ChoiceOffset];
}
static int LenChoice(Pcut *part, int ix, const char *whole) { return kP2ChoiceSize; }
static int OffsetChoice(Pcut *part, int ix, const char *whole) { return kP2ChoiceOffset; }
static cp_t ValidChoice(Pcut *part, int ix, const char *whole) 
{ 
	P2ChoicePcut * const cp = (P2ChoicePcut*)part;

	const uint8_t choice = P2ChoiceValue(whole);
	ifer(P2ChoiceValid(cp->choice_list, cp->choice_num, choice));
	return 0; 
}
static cp_t ExplainChoice(Pcut *part, int ix, const char *whole) 
{
	P2ChoicePcut * const cp = (P2ChoicePcut*)part;

	const uint8_t choice = P2ChoiceValue(whole);
	const char *str = P2ChoiceStr(cp->choice_list, cp->choice_num, choice);
	qos_printf("%uD(%s)", (uint32_t)choice, str);
	
	return 0;
}
//}}}


//{{{ var
static int LenVar(Pcut *part, int ix, const char *whole) 
{ 
	P2ChoicePcut * const p2cp = (P2ChoicePcut*)part;
	const uint8_t choice = P2ChoiceValue(whole);
	//qos_printf("choice=%02xH ix=%d\r\n", choice, ix);
	//printf_hex(whole, 10, " ");
	if (0 != PcutChoiceValid(p2cp, choice))
		return -1;

	const cp_t cp = CutFactoryVarOnWhole(p2cp, whole);
	dve(cp);

	if (0 != cp)
		return -1;

	dvb(NULL != p2cp->var_part);
	const char *mem = whole + kP2ChoiceVarOffset;
	return PcutIxLen(p2cp->var_part, kPcutIxAll, mem);
}
static int OffsetVar(Pcut *part, int ix, const char *whole) { return kP2ChoiceVarOffset; }
static cp_t ValidVar(Pcut *part, int ix, const char *whole) 
{ 
	P2ChoicePcut * const cp = (P2ChoicePcut*)part;
	ifer(CutFactoryVarOnWhole(cp, whole));
	dvb(NULL != cp->var_part);

	const char *mem = whole + kP2ChoiceVarOffset;
	ifer(PcutIxValid(cp->var_part, kPcutIxAll, mem));
	return 0; 
}
//}}}


//{{{ info
static cp_t PcutChoiceValid(const P2ChoicePcut *m, uint8_t choice)
{
	return P2ChoiceValid(m->choice_list, m->choice_num, choice);
}
static int PcutChoiceIx(const P2ChoicePcut *m, uint8_t choice)
{
	return P2Choice2Ix(m->choice_list, m->choice_num, choice);
}
static const PcutFactoryInfo *FactoryInfo(P2ChoicePcut *m, uint8_t choice)
{
	if (0 != PcutChoiceValid(m, choice))
		return NULL;
	const int ix = PcutChoiceIx(m, choice);
	dvb(ix >= 0);
	const PcutFactoryInfo *vfi = &m->var_factory_info[ix];
	dve(PcutFactoryInfoValid(vfi));
	return vfi;
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ChoiceNameChoice, LenChoice, OffsetChoice, ValidChoice, ExplainChoice },
	{ kP2ChoiceNameVar, LenVar, OffsetVar, ValidVar, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ChoiceCutIxChoice]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ChoiceCutIxVar]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


static cp_t CutCloseVar(P2ChoicePcut *m)
{
	if (NULL == m->var_part)
		return 0;

	// 首先，断开关联
	PcutSubSet(&m->base, kP2ChoiceCutIxVar, NULL, kCutFix[kP2ChoiceCutIxVar].fix_name);
	PcutDoSet(&m->base, kP2ChoiceCutIxVar, NULL);
	
	const PcutFactoryInfo * const vfi = FactoryInfo(m, m->choice);
	ifer(PcutFactoryInfoValid(vfi));
	ifer(PcutDeriveDestory(m->var_part, vfi));

	// 清理掉内容，避免再次调用close
	m->var_part = NULL;
	m->choice = kP2ChoiceInvalid;
	return 0;
}
cp_t P2ChoicePcutValid(const P2ChoicePcut *m)
{
	ifer(PcutValid(&m->base));
	ifbr(NULL != m->var_factory_info);

	if (NULL == m->var_part)
	{
		ifbr(kP2ChoiceInvalid == m->choice);
	}
	else
	{
		ifer(PcutChoiceValid(m, m->choice));
		ifbr(NULL != m->var_part);
		ifer(PcutValid(m->var_part));
	}
	return 0;
}
cp_t P2ChoicePcutClose(P2ChoicePcut *m)
{
	dve(P2ChoicePcutValid(m));

	ifer(PcutClose(&m->base));
	ifer(CutCloseVar(m));
	m->var_do_table = NULL;
	return 0;
}
cp_t P2ChoicePcutOpen(P2ChoicePcut *m, const char *choice_name, const P2Choice choice_list[], int choice_num
		, const PcutFactoryInfo *var_factory_info)
{
	m->choice_list = choice_list;
	m->choice_num = choice_num;
	m->var_factory_info = var_factory_info;

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	ifer(PcutNameSetIx(&m->base, kP2ChoiceCutIxChoice, choice_name));

	m->choice = kP2ChoiceInvalid;
	m->var_part = NULL;
	m->var_do_table = NULL;
	return 0;
}
// 两种情况，需要刷新var对应的do对象：
// 1 var_do_table变化
// 2 choice变化，即对应的var pcut对象变化。
static cp_t UpdateVarDo(P2ChoicePcut *m)
{
	// 当前还没有选择任何choice
	if (kP2ChoiceInvalid == m->choice)
		return 0;

	dve(PcutChoiceValid(m, m->choice));
	const int choice_ix = PcutChoiceIx(m, m->choice);
	
	Pdo * const doa = (NULL == m->var_do_table) ? NULL : m->var_do_table[choice_ix];
	PcutDoSet(&m->base, kP2ChoiceCutIxVar, doa);
	return 0;
}	
static cp_t CutFactoryVar(P2ChoicePcut *m, uint8_t choice)
{
	dve(P2ChoicePcutValid(m));

	// close旧的var
	if (NULL != m->var_part)
	{
		// 已经生成了var，且记录的choice和当前解析帧中的一样，没必要反复生成
		if (choice == m->choice)
			return 0;

		// choice不一样
		ifer(CutCloseVar(m));
	}

	const PcutFactoryInfo * const vfi = FactoryInfo(m, choice);
	ifer(PcutFactoryInfoValid(vfi));

	Pcut *var = NULL;
	ifer(PcutDeriveFactoryByInfo(&var, vfi));
	ifbr(NULL != var);
	m->choice = choice;
	m->var_part = var;
	const int choice_ix = PcutChoiceIx(m, choice);

	// 关联起来
	PcutSubSet(&m->base, kP2ChoiceCutIxVar, m->var_part, vfi->name);
	// 设置var do对象
	ifer(UpdateVarDo(m));
	return 0;
}


uint16_t P2ChoicePcutChoice(P2ChoicePcut *m)
{
	return m->choice;
}
Pcut *P2ChoicePcutVar(P2ChoicePcut *m)
{
	return m->var_part;
}
cp_t P2ChoiceVarDoTableSet(P2ChoicePcut *m, Pdo * const *var_do_table)
{
	dve(P2ChoicePcutValid(m));
	if (NULL != var_do_table)
	{
		dve(PdoTableValid(var_do_table, m->choice_num));
	}

	m->var_do_table = var_do_table;
	// 因为CutFactoryVar()只在choice变化时，才会调用。因此，在改变var_do_table时，也要刷新var对应的do对象。
	ifer(UpdateVarDo(m));
	return 0;
}
//}}}


#if 0
//{{{ qpack
static cp_t FillChoice(Qpack *pack, int ix, char *mem, int mem_size, int offset, void *value, int value_size, int *fill_len) 
{ 
	P2ChoiceQpack *m = (P2ChoiceQpack*)pack;
	mem[offset] = m->choice;
	*fill_len = kP2ChoiceSize;
	return 0; 
}
#define FillVar QpackItemFillBySub
// 为了节约内存，const部分集中在一起
// 固定部分
// pack顺序，要保证前一部分填写完毕后，要能推算出后一部分的偏移量。即后填的可以依赖先填的，但不能先填的依赖后填的
static const QpackItemFix kPackFix[kP2ChoicePackNum] = {
	// part_ix	fill
	{ kP2ChoiceNameChoice, QpackItemOffsetFollow, FillChoice },
	{ kP2ChoiceNameVar, QpackItemOffsetFollow, FillVar },
};
static const QpackItem kPackItemsPattern[kP2ChoicePackNum] = {
	QPACK_ITEM_NO_SUB(&kPackFix[kP2ChoicePackIxChoice]),
	QPACK_ITEM_NO_SUB(&kPackFix[kP2ChoicePackIxVar]),
};
static void QpackItemsInit(QpackItem items[kP2ChoicePackNum])
{
	memcpy(items, kPackItemsPattern, sizeof(kPackItemsPattern));
}


static cp_t PackChoiceValid(const P2ChoiceQpack *m, uint8_t choice)
{
	return P2ChoiceValid(m->choice_list, m->choice_num, choice);
}
cp_t P2ChoiceQpackValid(const P2ChoiceQpack *m)
{
	ifer(QpackValid(&m->base));

	if (NULL == m->var_pack)
	{
		ifbr(kP2ChoiceInvalid == m->choice);
	}
	else
	{
		ifer(PackChoiceValid(m, m->choice));
		ifer(QpackValid(m->var_pack));
	}
	return 0;
}
cp_t P2ChoiceQpackOpen(P2ChoiceQpack *m, const char *choice_name, const P2Choice choice_list[], int choice_num)
{
	// ifer(P2ChoicePcutOpen(&m->part, choice_name, choice_list, choice_num, var_factory_info));
	m->choice_list = choice_list; 
	m->choice_num = choice_num;

	QpackItemsInit(m->items);
	ifer(QpackOpen(&m->base, m->items, kP2ChoicePackNum, m, sizeof(P2ChoiceQpack)));
	//ifer(QpackNameSet(QpackBasePtr(&m->base), kP2ChoicePackIxInvalidChoice, choice_name));
	//ifer(QpackNameSet(QpackBasePtr(&m->base), kP2ChoicePackIxChoice, choice_name));

	m->choice = kP2ChoiceInvalid; 
	m->var_pack = NULL;
	return 0;
}
cp_t P2ChoiceQpackClose(P2ChoiceQpack *m)
{
	dve(P2ChoiceQpackValid(m));

	ifer(QpackClose(&m->base));
	m->choice = kP2ChoiceInvalid; 
	m->var_pack = NULL;
	return 0;
}
cp_t P2ChoiceQpackSetChoice(P2ChoiceQpack *m, uint8_t choice, Qpack *var_pack)
{
	dve(P2ChoiceQpackValid(m));
	ifer(PackChoiceValid(m, choice));

	ifbr(NULL != var_pack);
	dve(QpackValid(var_pack));
	m->choice = choice;
	m->var_pack = var_pack;
	QpackSubSet(&m->base, kP2ChoicePackIxVar, var_pack);
	return 0;
}
//}}}
#endif

//{{{ fill
cp_t P2ChoiceFillItemProcessChoice(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const P2ChoiceFillItemChoice *derive = (P2ChoiceFillItemChoice*)fi;

	mem[offset] = derive->choice;
	*fill_size = kP2ChoiceSize;
	return 0;
}

//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
void P2ChoiceTest(void)
{
	QtestTest(__FILE__, TEST_RUN_TIMES, kTestItem, kTestItemNum);
}

#else
void P2ChoiceTest(void)
{
	return;
}
#endif
//}}}

