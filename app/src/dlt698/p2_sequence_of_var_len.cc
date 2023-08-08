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

20230522-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 sequence of 部分报文解析
本文件解析content变长类型。定长类型用dlt698_45_sequence_of_fix_len来解析

*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "a_xdr.h"
#include "p2_sequence_of_var_len.h"
#include "p2_sequence_of_var_len.xcp.h"
//#define this_file_id	0x8335bd1c		// echo -n dlt698_45_sequence_of_var_len.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(0)

//{{{ nvar
static cp_t NvarOpen(P2SequenceOfVarLenPcut *m, int num)
{
	dve(P2SequenceOfVarLenPcutValid(m));
	dvb(0 == m->nvar_open_cnt);
	++m->nvar_open_cnt;
	ifer(P2NvarPcutOpen(&m->nvar_part, num, m->part_one, m->content_name));
	PcutSubSet(&m->base, kP2SequenceOfVarLenPartIxContent, &m->nvar_part.base, NULL);
	return 0;
}
static cp_t NvarClose(P2SequenceOfVarLenPcut *m)
{
	dve(P2SequenceOfVarLenPcutValid(m));
	if (0 == m->nvar_open_cnt)
		return 0;

	dvb(1 == m->nvar_open_cnt);
	--m->nvar_open_cnt;
	PcutSubSet(&m->base, kP2SequenceOfVarLenPartIxContent, NULL, NULL);
	ifer(P2NvarPcutClose(&m->nvar_part));
	return 0;
}
//}}}


// {{{ num
int P2SequenceOfVarLenNum(const char *whole)
{
	return VariableLenIntValue(whole);
}
static int NumSize(const char *whole)
{
	return VariableLenIntByteNum(*whole);
}

static int LenNum(Pcut *part, int ix, const char *whole) { return NumSize(whole); }
static int OffsetNum(Pcut *part, int ix, const char *whole) { return kP2SequenceOfVarLenNumOffset; }
static cp_t ValidNum(Pcut *part, int ix, const char *whole) 
{ 
	ifbr(kVariableLenInvalidLen != VariableLenIntValue(whole));
	return 0; 
}
//}}}


//{{{ content
static int LenContent(Pcut *part, int ix, const char *whole) 
{ 
	P2SequenceOfVarLenPcut *derive = (P2SequenceOfVarLenPcut*)part;
	const int num = P2SequenceOfVarLenNum(whole);

	ifed(NvarClose(derive));
	ifed(NvarOpen(derive, num));
	return PcutItemLenBySub(part, ix, whole);
}
static int OffsetContent(Pcut *part, int ix, const char *whole) 
{ 
	return P2SequenceOfVarLenContentOffset(whole);
}
static cp_t ValidContent(Pcut *part, int ix, const char *whole)
{
	P2SequenceOfVarLenPcut *derive = (P2SequenceOfVarLenPcut*)part;
	const int num = P2SequenceOfVarLenNum(whole);
	ifed(NvarClose(derive));
	ifed(NvarOpen(derive, num));
	return PcutItemValidBySub(part, ix, whole);
}


cp_t P2SequenceOfVarLenContentIxValid(const char *whole, int content_ix)
{
	ifbr(0 <= content_ix);
	ifbr(content_ix < P2SequenceOfVarLenNum(whole));
	return 0;
}
int P2SequenceOfVarLenContentOffset(const char *whole)
{
	return kP2SequenceOfVarLenContentOffset(NumSize(whole));
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2SequenceOfVarLenPartNum] = {
	// name len offset valid
	{ "num", LenNum, OffsetNum, ValidNum },
	{ "content", LenContent, OffsetContent, ValidContent },
};
	

static const PcutItem kPartItemsPattern[kP2SequenceOfVarLenPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2SequenceOfVarLenPartIxNum]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2SequenceOfVarLenPartIxContent]),
};
static void PcutItemsInit(PcutItem items[kP2SequenceOfVarLenPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}

cp_t P2SequenceOfVarLenPcutOpen(P2SequenceOfVarLenPcut *m, Pcut *part_one, const char *content_name)
{
	m->part_one = part_one;	
	m->content_name = content_name;

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2SequenceOfVarLenPartNum));
	dvb(0 == m->nvar_open_cnt);
	return 0;
}
cp_t P2SequenceOfVarLenPcutClose(P2SequenceOfVarLenPcut *m)
{
	dve(P2SequenceOfVarLenPcutValid(m));

	ifer(NvarClose(m));
	ifer(PcutClose(&m->base));
	dvb(0 == m->nvar_open_cnt);
	return 0;
}
cp_t P2SequenceOfVarLenPcutValid(const P2SequenceOfVarLenPcut *m)
{
	ifer(PcutValid(&m->base));
	ifbr(0 == m->nvar_open_cnt || 1 == m->nvar_open_cnt);
	if (1 == m->nvar_open_cnt)
		ifer(P2NvarPcutValid(&m->nvar_part));
	return 0;
}
//}}}


//{{{ fill
typedef struct 
{
	PfillItem base;
	int num;
} FillItemNum;
static cp_t FillItemProcessNum(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const FillItemNum *fi = (FillItemNum *)PfillIxItemConst(fill, ix);
	*fill_size = N2AXdr(mem + offset, fi->num);
	return 0;
}
#define kFillItemNumDef(_num) {								\
	PFILL_ITEM(kP2SequenceOfVarLenNameNum, PfillItemOffsetFix, FillItemProcessNum, kP2SequenceOfVarLenNumOffset, NULL),	\
	(_num),	\
}



cp_t P2SequenceOfVarLenFillInit(Pfill *m, PfillRepository *fill_repository)
{
	const int num = PfillRepositoryNum(fill_repository);
	// qos_printf("fill_repository num=%d sizeof(PfillItem)=%d sizeof(FillItemNum)=%d\r\n", num, sizeof(PfillItem), sizeof(FillItemNum));
	const FillItemNum kFiNum = kFillItemNumDef(num);
	// qos_printf("kFiNum.num=%d\r\n", kFiNum.num);
	const FillItemNum *fi_num = (const FillItemNum *)PfillItemFactory(m, sizeof(kFiNum), &kFiNum);
	// qos_printf("fi_num=%p num=%d\r\n", fi_num, fi_num->num);
	ifbr(NULL != fi_num);

	for (int i = 0; i < num; ++i)
	{
		const PfillRepositoryItem *fr_item = PfillRepositoryIxItemConst(fill_repository, i);
		dvb(NULL != fr_item->fill);
		const PfillItem kFi = PFILL_ITEM(kP2SequenceOfVarLenNameContent, PfillItemOffsetFollow, PfillItemProcessBySub, 0, fr_item->fill);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFi), &kFi));
	}

	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
#if 0
	// 03 00 10 02 00 20 00 02 00 20 04 02 00
	// 3个oad
	const char whole[] = "\x03\x00\x10\x02\x00\x20\x00\x02\x00\x20\x04\x02\x00";
	const int whole_size = sizeof(whole) - 1;

	P2SequenceOfPcut so = kP2SequenceOfPcutDef;
	ifer(P2SequenceOfPcutOpen(&so, 4, "oad"));
	Pcut * const m = &so.base;		

	ifbr(1 == PcutIxLen(m, kP2SequenceOfPartIxNum, whole));
	ifbr(12 == PcutIxLen(m, kP2SequenceOfPartIxContent, whole));
	ifbr(whole_size == PcutIxLen(m, kPcutIxAll, whole));
	ifbr(whole_size == 13);

	ifbr(0 == PcutIxOffset(m, kP2SequenceOfPartIxNum, whole));
	ifbr(1 == PcutIxOffset(m, kP2SequenceOfPartIxContent, whole));
	ifbr(13 == PcutIxOffset(m, kPcutIxAll, whole));

	ifer(PcutIxValid(m, kP2SequenceOfPartIxNum, whole));
	ifer(PcutIxValid(m, kP2SequenceOfPartIxContent, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	if (PRINT_PART_IN_TEST_EN)
		PcutAllPrint(m, 0, whole);

	ifer(P2SequenceOfPcutClose(&so));
	return 0;
#endif
}

static const QtestItem kTestItem[] = {
	TestPcut,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2SequenceOfVarLenTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SequenceOfVarLenTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

