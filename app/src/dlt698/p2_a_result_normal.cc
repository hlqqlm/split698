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
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的GetResponseNormal
*/
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qdlt698.h"

#include "p2_a_result_normal.h"
#include "p2_a_result_normal.xcp.h"


#define TEST_EN				(0)
#define kThisCutNum			(kP2AResultNormalCutNum)

// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2AResultNormalOadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2AResultNormalOadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ get_result
#define LenGetResult			PcutItemLenBySub
static int OffsetGetResult(Pcut *cut, int ix, const char *whole) { return kP2AResultNormalGetResultOffset; }
//#define OffsetGetResult			PcutItemOffsetDef
#define ValidGetResult			PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2AResultNormalNameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2AResultNormalNameGetResult, LenGetResult, OffsetGetResult, ValidGetResult, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2AResultNormalCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2AResultNormalCutIxGetResult]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2AResultNormalPcutOpen(P2AResultNormalPcut *m)
{

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2GetResultPcutOpen(&m->get_result_cut));
	PcutSubSet(&m->base, kP2AResultNormalCutIxGetResult, &m->get_result_cut.choice.base, NULL);
	return 0;
}
cp_t P2AResultNormalPcutClose(P2AResultNormalPcut *m)
{
	dve(P2AResultNormalPcutValid(m));

	PcutSubSet(&m->base, kP2AResultNormalCutIxGetResult, NULL, NULL);
	ifer(P2GetResultPcutClose(&m->get_result_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2AResultNormalPcutValid(const P2AResultNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2GetResultPcutValid(&m->get_result_cut));
	return 0;
}


cp_t P2AResultNormalPcutOpenBase(Pcut *base)
{
	P2AResultNormalPcut *m = (P2AResultNormalPcut*)base;
	return P2AResultNormalPcutOpen(m);
}
cp_t P2AResultNormalPcutCloseBase(Pcut *base)
{
	P2AResultNormalPcut *m = (P2AResultNormalPcut*)base;
	return P2AResultNormalPcutClose(m);
}
//}}}


//{{{ fill
cp_t P2AResultNormalFillItemProcessOad(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const P2AResultNormalFillItemOad *derive = (P2AResultNormalFillItemOad*)fi;

	Dlt698Oad2Mem(mem + offset, derive->oad);
	*fill_size = OAD_MEM_SIZE;
	return 0;
}


cp_t P2AResultNormalFillInit(Pfill *m, OadT oad, Pfill *sub_get_result)
{
	/*
	P2AResultNormalFillItemOad *fi_oad = qmalloc(sizeof(P2AResultNormalFillItemOad));
	ifbr(NULL != fi_oad);
	*fi_oad = kP2AResultNormalFillItemOadDef(PfillItemOffsetFix, 0);
	fi_oad->oad = oad;
	PfillItemSetFreeEn(&fi_oad->base);
	ifer(PfillAdd(m, fi_oad));
	*/
	const P2AResultNormalFillItemOad kFiOad = kP2AResultNormalFillItemOadDef(PfillItemOffsetFix, kP2AResultNormalOadOffset, oad);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiOad), &kFiOad));

	/*
	ifer(P2GetResultFillInit(sub_get_result, dar, sub_octet_string, data, data_size));
	PfillItem *fi_gr = qmalloc(sizeof(PfillItem));
	*fi_data = PFILL_ITEM(kP2AResultNormalNameGetResult, PfillItemOffsetFix, PfillItemProcessBySub, kP2AResultNormalGetResultOffset, sub_get_result);
	PfillItemSetFreeEn(&fi_data);
	ifer(PfillAdd(m, fi_data));
	*/
	const PfillItem kFi = PFILL_ITEM(kP2AResultNormalNameGetResult, PfillItemOffsetFix, PfillItemProcessBySub, kP2AResultNormalGetResultOffset, sub_get_result);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFi), &kFi));

// dlt698_45_get_request.h
// #include "dlt698_45_get_result.h"

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
cp_t P2AResultNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2AResultNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
