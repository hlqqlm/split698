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

20230512-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

struct
*/

#include "qos/qcp.h"
#include "qos/qstr.h"
#include "qos/qtest.h"
#include "qos/qmem.h"
#include "a_xdr.h"
#include "p2_struct.h"
#include "p2_struct.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2StructCutNum)



//{{{ subs
static cp_t SubsOpen(P2StructPcut *m, int num)
{
	ifbr(NULL == m->subs);

	const int size = num * sizeof(P2DataChoicePcut);
	m->subs = (P2DataChoicePcut*)qmalloc(size);
	ifbr(NULL != m->subs);

	const P2DataChoicePcut kSubDef = kP2DataChoicePcutDef;
	for (int i = 0; i < num; ++i)
	{
		memcpy(&m->subs[i], &kSubDef, sizeof(kSubDef));
		ifer(P2DataChoicePcutOpen(&m->subs[i]));
	}
	return 0;
}
static cp_t SubsClose(P2StructPcut *m, int num)
{
	ifbr(NULL != m->subs);

	for (int i = 0; i < num; ++i)
		ifer(P2DataChoicePcutClose(&m->subs[i]));
	qfree(m->subs);
	m->subs = NULL;
	return 0;
}
//}}}


//{{{ nvar
static cp_t NvarOpen(P2StructPcut *m, int num)
{
	dve(P2StructPcutValid(m));

	if (m->nvar_open_cnt)
	{
		dvb(1 == m->nvar_open_cnt);
		return 0;
	}

	dvb(0 == m->nvar_open_cnt);
	++m->nvar_open_cnt;
	// ifer(P2DataChoicePcutOpen(&m->one_data_choice));
	ifer(SubsOpen(m, num));
	ifer(P2NvarPcutOpen(&m->nvar_part, num, NULL, NULL));
	for (int i = 0; i < num; ++i)
		ifer(P2NvarPcutSetSub(&m->nvar_part, i, &m->subs[i].choice.base, "struct_item"));
	PcutSubSet(&m->base, kP2StructCutIxContent, &m->nvar_part.base, kP2StructNameContent);
	return 0;
}
static cp_t NvarClose(P2StructPcut *m)
{
	dve(P2StructPcutValid(m));
	if (0 == m->nvar_open_cnt)
		return 0;

	dvb(1 == m->nvar_open_cnt);
	--m->nvar_open_cnt;
	PcutSubSet(&m->base, kP2StructCutIxContent, NULL, NULL);
	//for (int i = 0; i < num; ++i)
	//	ifer(P2NvarPcutSetSub(&m->nvar_part, i, NULL, NULL));
	const int num = P2NvarPcutNum(&m->nvar_part);
	ifer(P2NvarPcutClose(&m->nvar_part));
	//ifer(P2DataChoicePcutClose(&m->one_data_choice));
	ifer(SubsClose(m, num));
	return 0;
}
//}}}


//{{{ misc
//static const P2StructPcut *ToDerive(const Pcut *part)
//{
//	return (P2StructPcut*)(part);
//}
//}}}


//{{{ var_len
static int VarlenOffset(const char *whole)
{
	return kP2StructVarlenOffset;
}
static const char *VarlenMem(const char *whole)
{
	const char *mem = whole + VarlenOffset(whole);
	return mem;
}
int P2StructVarlenSize(const char *whole)
{
	const char *mem = VarlenMem(whole);
	return VariableLenIntByteNum(*mem);
}
static int LenVarlen(Pcut *part, int ix, const char *whole) { return P2StructVarlenSize(whole); }
static int OffsetVarlen(Pcut *part, int ix, const char *whole) { return VarlenOffset(whole); }
static cp_t ValidVarlen(Pcut *part, int ix, const char *whole) 
{ 
	const char *mem = VarlenMem(whole);
	ifbr(kVariableLenInvalidLen != VariableLenIntValue(mem));
	return 0; 
}
static cp_t ExplainVarlen(Pcut *part, int ix, const char *whole) 
{
	const char *mem = VarlenMem(whole);
	const int value = VariableLenIntValue(mem);
	qos_printf("%dD", value);
	return 0;
}
//}}}


//{{{ content
int P2StructContentOffset(const char *whole)
{
	const int varlen_size = P2StructVarlenSize(whole);
	return kP2StructContentOffset(varlen_size);
}
static int LenContent(Pcut *part, int ix, const char *whole) 
{ 
	P2StructPcut *derive = (P2StructPcut*)part;
	const int num = P2StructNum(whole);

	ifed(NvarClose(derive));
	ifed(NvarOpen(derive, num));
	return PcutItemLenBySub(part, ix, whole);
}
#define OffsetContent		PcutItemOffsetDef
static cp_t ValidContent(Pcut *part, int ix, const char *whole) 
{
	P2StructPcut *derive = (P2StructPcut*)part;
	const int num = P2StructNum(whole);

	ifed(NvarClose(derive));
	ifer(NvarOpen(derive, num));
	return PcutItemValidBySub(part, ix, whole);
}
//}}}


//{{{ all
int P2StructCutSize(const char *whole)
{
	// 用P2DataChoice来解析每个元素大小
	P2StructPcut array = kP2StructPcutDef;
	const cp_t cp = P2StructPcutOpen(&array);
	if (0 != cp)
	{
		dve(cp);
		return 0;
	}

	const int size = PcutIxLen(&array.base, kPcutIxAll, whole);
	ifed(P2StructPcutClose(&array));
	return size;
}
int P2StructNum(const char *whole)
{
	const char *mem = whole + kP2StructVarlenOffset;
	return VariableLenIntValue(mem);
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2StructNameVarlen, LenVarlen, OffsetVarlen, ValidVarlen, ExplainVarlen },
	{ kP2StructNameContent, LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2StructCutIxVarlen]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2StructCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2StructPcutOpen(P2StructPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	dvb(NULL == m->subs);
	return 0;
}
cp_t P2StructPcutClose(P2StructPcut *m)
{
	dve(P2StructPcutValid(m));
	ifer(PcutClose(&m->base));
	ifer(NvarClose(m));
	dvb(NULL == m->subs);
	return 0;
}
cp_t P2StructPcutValid(const P2StructPcut *m)
{
	ifer(PcutValid(&m->base));
	ifbr(0 == m->nvar_open_cnt || 1 == m->nvar_open_cnt);
	if (0 < m->nvar_open_cnt)
	{
		ifer(P2NvarPcutValid(&m->nvar_part));
		// ifer(P2DataChoicePcutValid(&m->one_data_choice));
	}
	return 0;
}

cp_t P2StructPcutOpenBase(Pcut *base)
{
	P2StructPcut *m = (P2StructPcut*)base;
	return P2StructPcutOpen(m);
}
cp_t P2StructPcutCloseBase(Pcut *base)
{
	P2StructPcut *m = (P2StructPcut*)base;
	return P2StructPcutClose(m);
}



// struct item下标是否合法
cp_t P2StructPcutStructIxValid(int ix, const char *whole)
{
	const int num = P2StructNum(whole);
	ifbr(0 <= ix);
	ifbr(ix < num);
	return 0;
}
int P2StructPcutIxLen(P2StructPcut *m, int ix, const char *whole)
{
	dve(P2StructPcutStructIxValid(ix, whole));
	// const int content_offset = P2StructContentOffset(whole);
	const char *content = PcutIxPtrConst(&m->base, kP2StructCutIxContent, whole);
	return PcutIxLen(&m->nvar_part.base, ix, content);
}
int P2StructPcutIxOffset(P2StructPcut *m, int ix, const char *whole)
{
	dve(P2StructPcutStructIxValid(ix, whole));
	// const int content_offset = kP2StructContentOffset(P2StructVarlenSize(whole));
	const int content_offset = P2StructContentOffset(whole);
	const char *content = PcutIxPtrConst(&m->base, kP2StructCutIxContent, whole);
	return content_offset + PcutIxOffset(&m->nvar_part.base, ix, content);
}
cp_t P2StructPcutIxValid(P2StructPcut *m, int ix, const char *whole)
{
	dve(P2StructPcutStructIxValid(ix, whole));
	const char *content = PcutIxPtrConst(&m->base, kP2StructCutIxContent, whole);
	return PcutIxValid(&m->nvar_part.base, ix, content);
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
cp_t P2StructTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2StructTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


