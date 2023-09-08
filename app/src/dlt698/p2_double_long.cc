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

double long 
*/
/*
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
*/

#include "qos/qmem.h"

#include "p2_datatype_with_content.h"

#include "p2_double_long.h"
#include "p2_double_long.xcp.h"
// #define this_file_id	0x2a5cd78a  // echo -n dlt698_45_double_long.c | rhash --simple -


#define TEST_EN					(1)
#define kThisDatatype			(kDlt698DataTypeDoubleLong)
#define kThisCutNum				(kP2DoubleLongCutNum)


//{{{ main
static int LenMain(Pcut *cut, int ix, const char *whole) { return kP2DoubleLongSize; }
static int OffsetMain(Pcut *cut, int ix, const char *whole) { return kP2DoubleLongOffset; }
static cp_t ValidMain(Pcut *cut, int ix, const char *whole) 
{
	return 0;
}
static cp_t ExplainMain(Pcut *cut, int ix, const char *whole) 
{
	//const uint16_t value = P2LinkRequestHeartbeatIntervalValue(whole);
	const int32_t value = P2DoubleLongToValue(whole);
	qos_printf("%dD", value);
	return 0;
}
//}}}


//{{{ all
int32_t P2DoubleLongToValue(const char *whole)
{
	const char *src = whole + kP2DoubleLongOffset;
	const char value[kP2DoubleLongSize] = { src[3], src[2], src[1], src[0] };
	return *((int32_t*)value);
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2DoubleLongName, LenMain, OffsetMain, ValidMain, ExplainMain },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2DoubleLongCutIxMain]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2DoubleLongPcutOpen(P2DoubleLongPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2DoubleLongPcutClose(P2DoubleLongPcut *m)
{
	dve(P2DoubleLongPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2DoubleLongPcutValid(const P2DoubleLongPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}

cp_t P2DoubleLongPcutOpenBase(Pcut *base)
{
	P2DoubleLongPcut *m = (P2DoubleLongPcut*)base;
	return P2DoubleLongPcutOpen(m);
}
cp_t P2DoubleLongPcutCloseBase(Pcut *base)
{
	P2DoubleLongPcut *m = (P2DoubleLongPcut*)base;
	return P2DoubleLongPcutClose(m);
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
	mem[offset] = kDlt698DataTypeDoubleLong;
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
	int32_t value;
} FillItemContent;
static cp_t FillItemProcessContent(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemContent *derive = (FillItemContent*)fi;

	char *dst = mem + offset;
	const char *src = (char*)(&derive->value);
	MemcpyReverse(dst, src, kP2DoubleLongSize);
	*fill_size = kP2DoubleLongSize;
	return 0;
}
#define kFillItemContentDef(_value) {			\
	PFILL_ITEM(kP2DoubleLongName, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_value)					\
}

#if 0
cp_t P2DoubleLongFillInit(Pfill *m, bool datatype_exist, int32_t value)
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

cp_t P2DoubleLongFillInit(Pfill *m, bool datatype_exist, int32_t value)
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
	//	|-Data: double_long, s32	05 FF FF FF FD	类型:5,值:-3 	
	//	|-Data: double_long, s32	05 00 00 00 03	类型:5,值:3 	
	const char whole[] = "\x05\xff\xff\xff\xfd";
	const int whole_size = sizeof(whole) - 1;

	P2DatatypeWithContentPcut dwc = kP2DatatypeWithContentPcutDef;
	ifer(P2DatatypeWithContentPcutOpen(&dwc));

	P2DoubleLongPcut lu = kP2DoubleLongPcutDef;
	ifer(P2DoubleLongPcutOpen(&lu));
	ifer(P2DatatypeWithContentPcutSetContent(&dwc, kThisDatatype, &lu.base, kP2DoubleLongName));
	Pcut *m = &dwc.base;

	ifbr(5 == whole_size);
	ifbr(5 == PcutIxLen(m, 0, kPcutIxAll, whole));
	ifbr(5 == PcutIxOffset(m, 0, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2DoubleLongPcutClose(&lu));
	ifer(P2DatatypeWithContentPcutClose(&dwc));
	return 0;
}
static cp_t TestValue(void)
{
	//	|-Data: double_long, s32	05 FF FF FF FD	类型:5,值:-3 	
	{
	const char whole[] = "\xff\xff\xff\xfd";
	const int32_t value = P2DoubleLongToValue(whole);
	ifbr(-3 == value);
	}

	//	|-Data: double_long, s32	05 00 00 00 03	类型:5,值:3 	
	{
	const char whole[] = "\x00\x00\x00\x03";
	const int32_t value = P2DoubleLongToValue(whole);
	ifbr(3 == value);
	}
	return 0;
}
static cp_t TestFillPos(void)
{
	Pfill fill = kPfillDef;
	Pfill *m = &fill;
	ifer(PfillOpen(m));
	ifer(P2DoubleLongFillInit(m, true, 3));

	//	|-Data: double_long, s32	05 00 00 00 03	类型:5,值:3 	
	const char whole[] = "\x05\x00\x00\x00\x03";
	const int whole_size = sizeof(whole) - 1;

	ifbr(5 == whole_size);
	char mem[5] = { 0 };
	ifer(PfillDo(m, 0, mem, 5));
	ifbr(5 == PfillIxLen(m, kPfillIxAll));
	// printf_hex_ex("mem:", "\n", mem, 5, "");
	ifbr(0 == memcmp(mem, whole, 5));

	ifer(PfillClose(m));
	return 0;
}
static cp_t TestFillNeg(void)
{
	Pfill fill = kPfillDef;
	Pfill *m = &fill;
	ifer(PfillOpen(m));
	ifer(P2DoubleLongFillInit(m, true, -3));

	//	|-Data: double_long, s32	05 FF FF FF FD	类型:5,值:-3 	
	const char whole[] = "\x05\xff\xff\xff\xfd";
	const int whole_size = sizeof(whole) - 1;

	ifbr(5 == whole_size);
	char mem[5] = { 0 };
	ifer(PfillDo(m, 0, mem, 5));
	ifbr(5 == PfillIxLen(m, kPfillIxAll));
	// printf_hex_ex("mem:", "\n", mem, 5, "");
	ifbr(0 == memcmp(mem, whole, 5));

	ifer(PfillClose(m));
	return 0;
}
static cp_t TestFillNoDatatype(void)
{
	Pfill fill = kPfillDef;
	Pfill *m = &fill;
	ifer(PfillOpen(m));
	ifer(P2DoubleLongFillInit(m, false, 3));

	//	|-Data: double_long, s32	05 00 00 00 03	类型:5,值:3 	
	const char whole[] = "\x00\x00\x00\x03";
	const int whole_size = sizeof(whole) - 1;

	ifbr(4 == whole_size);
	char mem[4] = { 0 };
	ifer(PfillDo(m, 0, mem, 4));
	ifbr(4 == PfillIxLen(m, kPfillIxAll));
	// printf_hex_ex("mem:", "\n", mem, 5, "");
	ifbr(0 == memcmp(mem, whole, 4));

	ifer(PfillClose(m));
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
cp_t P2DoubleLongTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2DoubleLongTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


