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

20230815-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

datetimes
*/
#include "p2_datatype_with_content.h"

#include "p2_datetimes.h"
#include "p2_datetimes.xcp.h"
// #define this_file_id	0x2a5cd78a  // echo -n dlt698_45_datetime.c | rhash --simple -


#define TEST_EN					(1)
#define kThisDatatype			(kDlt698DataTypeDatetimeS)
#define kThisCutNum				(kP2DatetimesCutNum)

//{{{ misc
static const P2DatetimesPcut *ToDerive(const Pcut *part)
{
	return (P2DatetimesPcut*)(part);
}
//}}}


//{{{ main
cp_t P2DatetimesExplain(const char *whole) 
{
	Dlt698DatetimeS dts = kDlt698DatetimeSDef;
	Dlt698DatetimesValue(&dts, whole);
	//std::cout << " \t" << dt;
	std::cout << dts;
	return 0;
}

static int LenMain(Pcut *part, int ix, const char *whole) { return kP2DatetimesSize; }
static int OffsetMain(Pcut *part, int ix, const char *whole) { return kP2DatetimesOffset; }
static cp_t ValidMain(Pcut *part, int ix, const char *whole) 
{
	return 0;
}
static cp_t ExplainMain(Pcut *part, int ix, const char *whole) 
{
	const char *mem = whole + kP2DatetimesOffset;
	P2DatetimesExplain(mem);
	return 0;
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2DatetimesName, LenMain, OffsetMain, ValidMain, ExplainMain },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2DatetimesCutIxMain]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2DatetimesPcutOpen(P2DatetimesPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2DatetimesPcutClose(P2DatetimesPcut *m)
{
	dve(P2DatetimesPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2DatetimesPcutValid(const P2DatetimesPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2DatetimesPcutOpenBase(Pcut *base)
{
	P2DatetimesPcut *m = (P2DatetimesPcut*)base;
	return P2DatetimesPcutOpen(m);
}
cp_t P2DatetimesPcutCloseBase(Pcut *base)
{
	P2DatetimesPcut *m = (P2DatetimesPcut*)base;
	return P2DatetimesPcutClose(m);
}
//}}}


//{{{ fill
typedef struct 
{
	PfillItem base;
	Dlt698DatetimeS date_time_s;
} FillItemContent;
static cp_t FillItemProcessContent(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemContent *derive = (FillItemContent*)fi;

	char *dst = mem + offset;
	Dlt698DatetimesMem(dst, &derive->date_time_s);

	*fill_size = kDlt698DatetimeSSize;
	return 0;
}
#define kFillItemContentDef(_value) {			\
	PFILL_ITEM(kP2DatetimesName, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_value)					\
}



cp_t P2DatetimesFillInit(Pfill *m, bool datatype_exist, const Dlt698DatetimeS *date_time_s)
{
	ifer(P2DatatypeWithContentFillInit(m, datatype_exist, kThisDatatype));

	const FillItemContent kFiContent = kFillItemContentDef(*date_time_s);
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
static cp_t TestFill(void)
{
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
	TestFill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2DatetimesTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2DatetimesTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


