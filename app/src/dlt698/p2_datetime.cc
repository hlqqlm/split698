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

20230712-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

datetime
*/
#include "p2_datatype_with_content.h"

#include "p2_datetime.h"
#include "p2_datetime.xcp.h"


#define TEST_EN					(1)
#define kThisDatatype			(kDlt698DataTypeDatetime)
#define kThisCutNum				(kP2DatetimeCutNum)

//{{{ misc
static const P2DatetimePcut *ToDerive(const Pcut *part)
{
	return (P2DatetimePcut*)(part);
}
//}}}


//{{{ main
cp_t P2DatetimeExplain(const char *whole) 
{
	Dlt698Datetime dt = kDlt698DatetimeDef;
	Dlt698DatetimeValue(&dt, whole);
	//std::cout << " \t" << dt;
	std::cout << dt;
	return 0;
}

static int LenMain(Pcut *part, int ix, const char *whole) { return kP2DatetimeSize; }
static int OffsetMain(Pcut *part, int ix, const char *whole) { return kP2DatetimeOffset; }
static cp_t ValidMain(Pcut *part, int ix, const char *whole) 
{
	return 0;
}
static cp_t ExplainMain(Pcut *part, int ix, const char *whole) 
{
	const char *mem = whole + kP2DatetimeOffset;
	P2DatetimeExplain(mem);
	return 0;
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2DatetimeName, LenMain, OffsetMain, ValidMain, ExplainMain },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2DatetimeCutIxMain]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2DatetimePcutOpen(P2DatetimePcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2DatetimePcutClose(P2DatetimePcut *m)
{
	dve(P2DatetimePcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2DatetimePcutValid(const P2DatetimePcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2DatetimePcutOpenBase(Pcut *base)
{
	P2DatetimePcut *m = (P2DatetimePcut*)base;
	return P2DatetimePcutOpen(m);
}
cp_t P2DatetimePcutCloseBase(Pcut *base)
{
	P2DatetimePcut *m = (P2DatetimePcut*)base;
	return P2DatetimePcutClose(m);
}
//}}}


//{{{ fill
typedef struct 
{
	PfillItem base;
	Dlt698Datetime datetime;
} FillItemContent;
static cp_t FillItemProcessContent(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemContent *derive = (FillItemContent*)fi;

	char *dst = mem + offset;
	Dlt698DatetimeMem(dst, &derive->datetime);

	*fill_size = kDlt698DatetimeSize;
	return 0;
}
#define kFillItemContentDef(_value) {			\
	PFILL_ITEM(kP2DatetimeName, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_value)					\
}



cp_t P2DatetimeFillInit(Pfill *m, bool datatype_exist, const Dlt698Datetime *datetime)
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
	//   |-request_timedate_time(u16,u8,u8,week,u8,u8,u8,ms)	07 E7 07 0B 02 10 2B 3B 00 00	2023-07-11 16:43:59    0ms 周3	
	const char whole[] = "\x19\x07\xe7\x07\x0b\x02\x10\x2b\x3b\x00\x00";
	const int whole_size = sizeof(whole) - 1;

	P2DatatypeWithContentPcut dwc = kP2DatatypeWithContentPcutDef;
	ifer(P2DatatypeWithContentPcutOpen(&dwc));

	P2DatetimePcut lu = kP2DatetimePcutDef;
	ifer(P2DatetimePcutOpen(&lu));
	ifer(P2DatatypeWithContentPcutSetContent(&dwc, kThisDatatype, &lu.base, kP2DatetimeName));
	Pcut *m = &dwc.base;

	ifbr(11 == whole_size);
	ifbr(11 == PcutIxLen(m, kPcutIxAll, whole));
	ifbr(11 == PcutIxOffset(m, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2DatetimePcutClose(&lu));
	ifer(P2DatatypeWithContentPcutClose(&dwc));
	return 0;
}
static cp_t TestValue(void)
{
	//   |-request_timedate_time(u16,u8,u8,week,u8,u8,u8,ms)	07 E7 07 0B 02 10 2B 3B 00 00	2023-07-11 16:43:59    0ms 周3	
	const char whole[] = "\x07\xe7\x07\x0b\x02\x10\x2b\x3b\x00\x00";
	const int whole_size = sizeof(whole) - 1;
	Dlt698Datetime dt = kDlt698DatetimeDef;
	Dlt698DatetimeValue(&dt, whole);

	const Dlt698Datetime dt_expect = {2023, 7, 11, 2, 16, 43, 59, 0};
	// std::cout << CpStr(Dlt698DatetimeEqual(dt, dt_expect)) << std::endl;
	ifbr(0 == Dlt698DatetimeEqual(dt, dt_expect));

	// std::cout << "dt=" << dt << std::endl;
	return 0;
}
static cp_t TestFill(void)
{
	//   |-request_timedate_time(u16,u8,u8,week,u8,u8,u8,ms)	07 E7 07 0B 02 10 2B 3B 00 00	2023-07-11 16:43:59    0ms 周3	
	Pfill fill = kPfillDef;
	Pfill *m = &fill;
	ifer(PfillOpen(m));
	const Dlt698Datetime dt = {2023, 7, 11, 2, 16, 43, 59, 0};
	ifer(P2DatetimeFillInit(m, true, &dt));

	//   |-request_timedate_time(u16,u8,u8,week,u8,u8,u8,ms)	07 E7 07 0B 02 10 2B 3B 00 00	2023-07-11 16:43:59    0ms 周3	
	const char whole[] = "\x19\x07\xe7\x07\x0b\x02\x10\x2b\x3b\x00\x00";
	const int whole_size = sizeof(whole) - 1;

	ifbr(11 == whole_size);
	char mem[11] = { 0 };
	ifer(PfillDo(m, 0, mem, 11));
	ifbr(11 == PfillIxLen(m, kPfillIxAll));
	// printf_hex_ex("mem:", "\r\n", mem, 5, "");
	ifbr(0 == memcmp(mem, whole, 11));

	ifer(PfillClose(m));
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
	TestFill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2DatetimeTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2DatetimeTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


