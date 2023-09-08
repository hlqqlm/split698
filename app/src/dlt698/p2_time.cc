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

time
*/
#include "p2_datatype_with_content.h"

#include "p2_time.h"
#include "p2_time.xcp.h"


#define TEST_EN					(1)
#define kThisDatatype			(kDlt698DataTypeTime)
#define kThisCutNum				(kP2TimeCutNum)

//{{{ misc
static const P2TimePcut *ToDerive(const Pcut *cut)
{
	return (P2TimePcut*)(cut);
}
//}}}


//{{{ main
cp_t P2TimeExplain(const char *whole) 
{
	Dlt698Time time = kDlt698TimeDef;
	Dlt698TimeValue(&time, whole);
	//std::cout << " \t" << dt;
	std::cout << time;
	return 0;
}

static int LenMain(Pcut *cut, int ix, const char *whole) { return kP2TimeSize; }
static int OffsetMain(Pcut *cut, int ix, const char *whole) { return kP2TimeOffset; }
static cp_t ValidMain(Pcut *cut, int ix, const char *whole) 
{
	return 0;
}
static cp_t ExplainMain(Pcut *cut, int ix, const char *whole) 
{
	const char *mem = whole + kP2TimeOffset;
	P2TimeExplain(mem);
	return 0;
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2TimeName, LenMain, OffsetMain, ValidMain, ExplainMain },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2TimeCutIxMain]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2TimePcutOpen(P2TimePcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2TimePcutClose(P2TimePcut *m)
{
	dve(P2TimePcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2TimePcutValid(const P2TimePcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2TimePcutOpenBase(Pcut *base)
{
	P2TimePcut *m = (P2TimePcut*)base;
	return P2TimePcutOpen(m);
}
cp_t P2TimePcutCloseBase(Pcut *base)
{
	P2TimePcut *m = (P2TimePcut*)base;
	return P2TimePcutClose(m);
}
//}}}


//{{{ fill
typedef struct 
{
	PfillItem base;
	Dlt698Time datetime;
} FillItemContent;
static cp_t FillItemProcessContent(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemContent *derive = (FillItemContent*)fi;

	char *dst = mem + offset;
	Dlt698TimeMem(dst, &derive->datetime);

	*fill_size = kDlt698TimeSize;
	return 0;
}
#define kFillItemContentDef(_value) {			\
	PFILL_ITEM(kP2TimeName, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_value)					\
}



cp_t P2TimeFillInit(Pfill *m, bool datatype_exist, const Dlt698Time *datetime)
{
	ifer(P2DatatypeWithContentFillInit(m, datatype_exist, kThisDatatype));

	const FillItemContent kFiContent = kFillItemContentDef(*datetime);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiContent), &kFiContent));
	return 0;
}





//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	// |-Data: (u8,u8,u8)	1B 17 3B 3B	类型:27,值:23:59:59		
	const char whole[] = "\x1b\x17\x3b\x3b";
	const int whole_size = sizeof(whole) - 1;

	P2DatatypeWithContentPcut dwc = kP2DatatypeWithContentPcutDef;
	ifer(P2DatatypeWithContentPcutOpen(&dwc));

	P2TimePcut lu = kP2TimePcutDef;
	ifer(P2TimePcutOpen(&lu));
	ifer(P2DatatypeWithContentPcutSetContent(&dwc, kThisDatatype, &lu.base, kP2TimeName));
	Pcut *m = &dwc.base;

	ifbr(4 == whole_size);
	ifbr(4 == PcutIxLen(m, 0, kPcutIxAll, whole));
	ifbr(4 == PcutIxOffset(m, 0, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2TimePcutClose(&lu));
	ifer(P2DatatypeWithContentPcutClose(&dwc));
	return 0;
}
static cp_t TestValue(void)
{
	// |-Data: (u8,u8,u8)	1B 17 3B 3B	类型:27,值:23:59:59		
	const char whole[] = "\x17\x3b\x3b";
	const int whole_size = sizeof(whole) - 1;
	Dlt698Time time = kDlt698TimeDef;
	Dlt698TimeValue(&time, whole);

	const Dlt698Time time_expect = {23, 59, 59};
	// std::cout << CpStr(Dlt698TimeEqual(dt, dt_expect)) << std::endl;
	ifbr(0 == Dlt698TimeEqual(time, time_expect));

	// std::cout << "dt=" << dt << std::endl;
	return 0;
}
static cp_t TestFill(void)
{
	Pfill fill = kPfillDef;
	Pfill *m = &fill;
	ifer(PfillOpen(m));
	const Dlt698Time time = {23, 59, 59};
	ifer(P2TimeFillInit(m, true, &time));

	// |-Data: (u8,u8,u8)	1B 17 3B 3B	类型:27,值:23:59:59		
	const char whole[] = "\x1b\x17\x3b\x3b";
	const int whole_size = sizeof(whole) - 1;

	ifbr(4 == whole_size);
	char mem[4] = { 0 };
	ifer(PfillDo(m, 0, mem, 4));
	ifbr(11 == PfillIxLen(m, kPfillIxAll));
	// printf_hex_ex("mem:", "\n", mem, 5, "");
	ifbr(0 == memcmp(mem, whole, 4));

	ifer(PfillClose(m));
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
	TestFill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2TimeTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2TimeTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


