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

20230907-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

long64
*/
#include "qos/qmem.h"

#include "p2_datatype_with_content.h"

#include "p2_long64.h"
#include "p2_long64.xcp.h"


#define TEST_EN					(1)
#define kThisDatatype			(kDlt698DataTypeLong64)
#define kThisCutNum				(kP2Long64CutNum)


//{{{ main
static int LenMain(Pcut *cut, int ix, const char *whole) { return kP2Long64Size; }
static int OffsetMain(Pcut *cut, int ix, const char *whole) { return kP2Long64Offset; }
static cp_t ValidMain(Pcut *cut, int ix, const char *whole) 
{
	return 0;
}
static cp_t ExplainMain(Pcut *cut, int ix, const char *whole) 
{
	//const uint16_t value = P2LinkRequestHeartbeatIntervalValue(whole);
	const int64_t value = P2Long64ToValue(whole);
	qos_printf("%ldD", value);
	return 0;
}
//}}}


//{{{ all
int64_t P2Long64ToValue(const char *whole)
{
	const char *src = whole + kP2Long64Offset;
	const char value[kP2Long64Size] = { src[7], src[6], src[5], src[4], src[3], src[2], src[1], src[0] };
	return *((int64_t*)value);
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2Long64Name, LenMain, OffsetMain, ValidMain, ExplainMain },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2Long64CutIxMain]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2Long64PcutOpen(P2Long64Pcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2Long64PcutClose(P2Long64Pcut *m)
{
	dve(P2Long64PcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2Long64PcutValid(const P2Long64Pcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}

cp_t P2Long64PcutOpenBase(Pcut *base)
{
	P2Long64Pcut *m = (P2Long64Pcut*)base;
	return P2Long64PcutOpen(m);
}
cp_t P2Long64PcutCloseBase(Pcut *base)
{
	P2Long64Pcut *m = (P2Long64Pcut*)base;
	return P2Long64PcutClose(m);
}
//}}}


//{{{ fill
#if 0
typedef struct 
{
	PfillItem base;
} FillItemDatatype;
static cp_t FillItemProcessDatatype(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	mem[offset] = kDlt698DataTypeLong64;
	*fill_size = 1;
	return 0;
}
#define kFillItemDatatypeDef {			\
	PFILL_ITEM(kDlt698DatatypeName, PfillItemOffsetFix, FillItemProcessDatatype, 0, NULL)			\
}
#endif


typedef struct 
{
	PfillItem base;
	int64_t value;
} FillItemContent;
static cp_t FillItemProcessContent(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemContent *derive = (FillItemContent*)fi;

	char *dst = mem + offset;
	const char *src = (char*)(&derive->value);
	MemcpyReverse(dst, src, kP2Long64Size);
	*fill_size = kP2Long64Size;
	return 0;
}
#define kFillItemContentDef(_value) {			\
	PFILL_ITEM(kP2Long64Name, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_value)					\
}

#if 0
cp_t P2Long64FillInit(Pfill *m, bool datatype_exist, int32_t value)
{
	if (datatype_exist)
	{
		const FillItemDatatype kFiDt = kFillItemDatatypeDef;
		ifbr(NULL != PfillItemFactory(m, sizeof(FillItemDatatype), &kFiDt));
	}

	const FillItemContent kFiContent = kFillItemContentDef(value);
	ifbr(NULL != PfillItemFactory(m, sizeof(FillItemContent), &kFiContent));
	return 0;
}
#endif

cp_t P2Long64FillInit(Pfill *m, bool datatype_exist, int64_t value)
{
	//ifer(P2DatatypeWithContentFillInit(m, datatype_exist, kThisDatatype, sizeof(kFiContent), &kFiContent));
	ifer(P2DatatypeWithContentFillInit(m, datatype_exist, kThisDatatype));

	const FillItemContent kFiContent = kFillItemContentDef(value);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiContent), &kFiContent));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}
static cp_t TestValue(void)
{
	return 0;
}
static cp_t TestFillPos(void)
{
	return 0;
}
static cp_t TestFillNeg(void)
{
	return 0;
}
static cp_t TestFillNoDatatype(void)
{
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
	TestFillPos,
	TestFillNeg,
	TestFillNoDatatype,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2Long64Test(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2Long64Test(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


