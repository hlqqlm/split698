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

double long unsigned
*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "qos/qmem.h"
#include "p2_double_long_unsigned.h"
#include "p2_double_long_unsigned.xcp.h"
//#define this_file_id	0x49cd56cb  // echo -n dlt698_45_double_long_unsigned.c | rhash --simple -


#define TEST_EN					(0)
#define kThisCutNum				(kP2DoubleLongUnsignedCutNum)

//{{{ misc
static const P2DoubleLongUnsignedPcut *ToDerive(const Pcut *part)
{
	return (P2DoubleLongUnsignedPcut*)(part);
}
static int GetDatatypeSize(bool datatype_exist)
{
	return datatype_exist ? kDlt698DatatypeSize : 0;
}
//}}}


//{{{ datatype
static int DatatypeSize(const Pcut *part, const char *whole) 
{
	const P2DoubleLongUnsignedPcut *derive = ToDerive(part);
	if (!derive->datatype_exist)
		return 0;

	dvb(kDlt698DataTypeDoubleLongUnsigned == *whole);
	return kDlt698DatatypeSize;
}
static int LenDatatype(Pcut *part, int ix, const char *whole) 
{ 
	return DatatypeSize(part, whole);
}
static int OffsetDatatype(Pcut *part, int ix, const char *whole) 
{ 
	return kP2DoubleLongUnsignedDatatypeOffset; 
}
static cp_t ValidDatatype(Pcut *part, int ix, const char *whole) 
{ 
	const P2DoubleLongUnsignedPcut *derive = ToDerive(part);
	if (derive->datatype_exist)
	{
		ifbr(kDlt698DataTypeDoubleLongUnsigned == *whole);
		return 0;
	}
	return 0; 
}
//}}}


//{{{ content
static int LenContent(Pcut *part, int ix, const char *whole) 
{ 
	return kP2DoubleLongUnsignedContentSize;
}
static int OffsetContent(Pcut *part, int ix, const char *whole) 
{ 
	const int datatype_size = DatatypeSize(part, whole);
	const int content_offset = kP2DoubleLongUnsignedContentOffset(datatype_size);
	return content_offset;
}
static cp_t ValidContent(Pcut *part, int ix, const char *whole) 
{
	return 0;
}
//}}}


//{{{ all
int P2DoubleLongUnsignedCutSize(bool datatype_exist)
{
	const int datatype_size = GetDatatypeSize(datatype_exist);
	return kP2DoubleLongUnsignedWholeSize(datatype_size);
}
uint32_t P2DoubleLongUnsignedToValue(bool datatype_exist, const char *whole)
{
	const int content_offset = kP2DoubleLongUnsignedContentOffset(GetDatatypeSize(datatype_exist));
	const char *src = whole + content_offset;
	const char value[kP2DoubleLongUnsignedContentSize] = { src[3], src[2], src[1], src[0] };
	return *((uint32_t*)value);
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "datatype", LenDatatype, OffsetDatatype, ValidDatatype, NULL },
	{ "content", LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2DoubleLongUnsignedCutIxDatatype]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2DoubleLongUnsignedCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2DoubleLongUnsignedPcutOpen(P2DoubleLongUnsignedPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2DoubleLongUnsignedPcutClose(P2DoubleLongUnsignedPcut *m)
{
	dve(P2DoubleLongUnsignedPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2DoubleLongUnsignedPcutValid(const P2DoubleLongUnsignedPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
void P2DoubleLongUnsignedPcutConfigDatatypeExist(P2DoubleLongUnsignedPcut *m, bool exist)
{
	dve(P2DoubleLongUnsignedPcutValid(m));
	m->datatype_exist = exist;
	return;
}

cp_t P2DoubleLongUnsignedPcutOpenBase(Pcut *base)
{
	P2DoubleLongUnsignedPcut *m = (P2DoubleLongUnsignedPcut*)base;
	return P2DoubleLongUnsignedPcutOpen(m);
}
cp_t P2DoubleLongUnsignedPcutCloseBase(Pcut *base)
{
	P2DoubleLongUnsignedPcut *m = (P2DoubleLongUnsignedPcut*)base;
	return P2DoubleLongUnsignedPcutClose(m);
}
//}}}


//{{{ pcut-datatype
// 有数据类型的pcut
cp_t P2DoubleLongUnsignedDatatypePcutOpen(P2DoubleLongUnsignedPcut *m)
{
	ifer(P2DoubleLongUnsignedPcutOpen(m));
	P2DoubleLongUnsignedPcutConfigDatatypeExist(m, true);
	return 0;
}
cp_t P2DoubleLongUnsignedDatatypePcutClose(P2DoubleLongUnsignedPcut *m)
{
	ifer(P2DoubleLongUnsignedPcutClose(m));
	return 0;
}
cp_t P2DoubleLongUnsignedDatatypePcutValid(const P2DoubleLongUnsignedPcut *m)
{
	ifer(P2DoubleLongUnsignedPcutValid(m));
	ifbr(m->datatype_exist);
	return 0;
}
//}}}


//{{{ fill_by_string
typedef struct 
{
	PfillItem base;
} FillItemDataType;
static cp_t FillItemProcessDataType(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	mem[offset] = kDlt698DataTypeDoubleLongUnsigned;
	*fill_size = 1;
	return 0;
}
#define kFillItemDataTypeDef {			\
	PFILL_ITEM(kP2DoubleLongUnsignedNameDatatype, PfillItemOffsetFix, FillItemProcessDataType, kP2DoubleLongUnsignedDatatypeOffset, NULL)			\
}


typedef struct 
{
	PfillItem base;
	uint32_t value;
} FillItemContent;
static cp_t FillItemProcessContent(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemContent *derive = (FillItemContent*)fi;

	char *dst = mem + offset;
	const char *src = (char*)(&derive->value);
	// memcpy(dst, , kP2DoubleLongUnsignedContentSize);
	MemcpyReverse(dst, src, kP2DoubleLongUnsignedContentSize);
	*fill_size = kP2DoubleLongUnsignedContentSize;
	return 0;
}
#define kFillItemContentDef(_value) {			\
	PFILL_ITEM(kP2DoubleLongUnsignedNameContent, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_value)					\
}


cp_t P2DoubleLongUnsignedFillInit(Pfill *m, bool datatype_exist, uint32_t value)
{
	if (datatype_exist)
	{
		const FillItemDataType kFiDt = kFillItemDataTypeDef;
		ifbr(NULL != PfillItemFactory(m, sizeof(FillItemDataType), &kFiDt));
	}

	const FillItemContent kFiContent = kFillItemContentDef(value);
	ifbr(NULL != PfillItemFactory(m, sizeof(FillItemContent), &kFiContent));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	//		|-Data: double_long_unsigned, u32	06 00 00 00 02	类型:6,值:2 	
	const char whole[] = "\x06\x00\x00\x00\x02";
	const int whole_size = sizeof(whole) - 1;

	P2DoubleLongUnsignedPcut dlu = kP2DoubleLongUnsignedPcutDef;
	ifer(P2DoubleLongUnsignedPcutOpen(&dlu));
	P2DoubleLongUnsignedPcutConfigDatatypeExist(&dlu, true);	
	Pcut *m = &dlu.base;

	ifbr(5 == whole_size);
	ifbr(5 == PcutIxLen(m, kPcutIxAll, whole));
	ifbr(5 == PcutIxOffset(m, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2DoubleLongUnsignedPcutClose(&dlu));
	return 0;
}
static cp_t TestValue(void)
{
	//		|-Data: double_long_unsigned, u32	06 00 00 00 02	类型:6,值:2 	
	const char whole[] = "\x06\x00\x00\x00\x02";

	const uint32_t value = P2DoubleLongUnsignedToValue(true, whole);
	ifbr(2 == value);
	return 0;
}
static cp_t TestFill(void)
{
	Pfill fill = kPfillDef;
	Pfill *m = &fill;
	ifer(PfillOpen(m));
	ifer(P2DoubleLongUnsignedFillInit(m, true, 2));

	//		|-Data: double_long_unsigned, u32	06 00 00 00 02	类型:6,值:2 	
	const char whole[] = "\x06\x00\x00\x00\x02";
	const int whole_size = sizeof(whole) - 1;

	ifbr(5 == whole_size);
	char mem[5] = { 0 };
	ifer(PfillDo(m, 0, mem, 5));
	ifbr(5 == PfillIxLen(m, kPfillIxAll));
	ifbr(0 == memcmp(mem, whole, 5));

	ifer(PfillClose(m));
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
	TestFill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2DoubleLongUnsignedTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2DoubleLongUnsignedTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


