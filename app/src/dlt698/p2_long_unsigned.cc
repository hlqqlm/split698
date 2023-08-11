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

20230614-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

long unsigned
*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "qos/qcp.h"
#include "qos/qmem.h"
#include "p2_long_unsigned.h"
#include "p2_long_unsigned.xcp.h"
//#define this_file_id	0x493800ec  // echo -n dlt698_45_long_unsigned.c | rhash --simple -


#define TEST_EN					(1)


//{{{ misc
static const P2LongUnsignedPcut *ToDerive(const Pcut *part)
{
	return (P2LongUnsignedPcut*)(part);
}
static int GetDatatypeSize(bool datatype_exist)
{
	return datatype_exist ? kDlt698DatatypeSize : 0;
}
//}}}


//{{{ datatype
static int DatatypeSize(const Pcut *part, const char *whole) 
{
	const P2LongUnsignedPcut *derive = ToDerive(part);
	if (!derive->datatype_exist)
		return 0;

	dvb(kDlt698DataTypeLongUnsigned == *whole);
	return kDlt698DatatypeSize;
}
static int LenDatatype(Pcut *part, int ix, const char *whole) 
{ 
	return DatatypeSize(part, whole);
}
static int OffsetDatatype(Pcut *part, int ix, const char *whole) 
{ 
	return kP2LongUnsignedDatatypeOffset; 
}
static cp_t ValidDatatype(Pcut *part, int ix, const char *whole) 
{ 
	const P2LongUnsignedPcut *derive = ToDerive(part);
	if (derive->datatype_exist)
	{
		ifbr(kDlt698DataTypeLongUnsigned == *whole);
		return 0;
	}
	return 0; 
}
//}}}


//{{{ content
static int LenContent(Pcut *part, int ix, const char *whole) 
{ 
	return kP2LongUnsignedContentSize;
}
static int OffsetContent(Pcut *part, int ix, const char *whole) 
{ 
	const int datatype_size = DatatypeSize(part, whole);
	const int content_offset = kP2LongUnsignedContentOffset(datatype_size);
	return content_offset;
}
static cp_t ValidContent(Pcut *part, int ix, const char *whole) 
{
	return 0;
}
//}}}


//{{{ all
int P2LongUnsignedPartSize(bool datatype_exist)
{
	const int datatype_size = GetDatatypeSize(datatype_exist);
	return kP2LongUnsignedWholeSize(datatype_size);
}
uint16_t P2LongUnsignedToValue(bool datatype_exist, const char *whole)
{
	const int content_offset = kP2LongUnsignedContentOffset(GetDatatypeSize(datatype_exist));
	const char *src = whole + content_offset;
	const char value[kP2LongUnsignedContentSize] = { src[1], src[0] };
	return *((uint16_t*)value);
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2LongUnsignedPartNum] = {
	// name len offset valid explain
	{ "datatype", LenDatatype, OffsetDatatype, ValidDatatype, NULL },
	{ "content", LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kPartItemsPattern[kP2LongUnsignedPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2LongUnsignedPartIxDatatype]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2LongUnsignedPartIxContent]),
};
static void PcutItemsInit(PcutItem items[kP2LongUnsignedPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}


cp_t P2LongUnsignedPcutOpen(P2LongUnsignedPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2LongUnsignedPartNum));
	return 0;
}
cp_t P2LongUnsignedPcutClose(P2LongUnsignedPcut *m)
{
	dve(P2LongUnsignedPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2LongUnsignedPcutValid(const P2LongUnsignedPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
void P2LongUnsignedPcutConfigDatatypeExist(P2LongUnsignedPcut *m, bool exist)
{
	dve(P2LongUnsignedPcutValid(m));
	m->datatype_exist = exist;
	return;
}

cp_t P2LongUnsignedPcutOpenBase(Pcut *base)
{
	P2LongUnsignedPcut *m = (P2LongUnsignedPcut*)base;
	return P2LongUnsignedPcutOpen(m);
}
cp_t P2LongUnsignedPcutCloseBase(Pcut *base)
{
	P2LongUnsignedPcut *m = (P2LongUnsignedPcut*)base;
	return P2LongUnsignedPcutClose(m);
}
//}}}


//{{{ pcut-datatype
// 有数据类型的pcut
cp_t P2LongUnsignedDatatypePcutOpen(P2LongUnsignedPcut *m)
{
	ifer(P2LongUnsignedPcutOpen(m));
	P2LongUnsignedPcutConfigDatatypeExist(m, true);
	return 0;
}
cp_t P2LongUnsignedDatatypePcutClose(P2LongUnsignedPcut *m)
{
	ifer(P2LongUnsignedPcutClose(m));
	return 0;
}
cp_t P2LongUnsignedDatatypePcutValid(const P2LongUnsignedPcut *m)
{
	ifer(P2LongUnsignedPcutValid(m));
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
	mem[offset] = kDlt698DataTypeLongUnsigned;
	*fill_size = 1;
	return 0;
}
#define kFillItemDataTypeDef {			\
	PFILL_ITEM(kP2LongUnsignedNameDatatype, PfillItemOffsetFix, FillItemProcessDataType, kP2LongUnsignedDatatypeOffset, NULL)			\
}


typedef struct 
{
	PfillItem base;
	uint16_t value;
} FillItemContent;
static cp_t FillItemProcessContent(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemContent *derive = (FillItemContent*)fi;

	char *dst = mem + offset;
	const char *src = (char*)(&derive->value);
	MemcpyReverse(dst, src, kP2LongUnsignedContentSize);
	*fill_size = kP2LongUnsignedContentSize;
	return 0;
}
#define kFillItemContentDef(_value) {			\
	PFILL_ITEM(kP2LongUnsignedNameContent, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_value)					\
}


cp_t P2LongUnsignedFillInit(Pfill *m, bool datatype_exist, uint16_t value)
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
	//		|-Data: long-unsigned, u16	12 08 CE	类型:18,值:2254 	
	const char whole[] = "\x12\x08\xce";
	const int whole_size = sizeof(whole) - 1;
	//const uint16_t value = 2254;

	P2LongUnsignedPcut lu = kP2LongUnsignedPcutDef;
	ifer(P2LongUnsignedPcutOpen(&lu));
	P2LongUnsignedPcutConfigDatatypeExist(&lu, true);	
	Pcut *m = &lu.base;

	ifbr(3 == whole_size);
	ifbr(3 == PcutIxLen(m, kPcutIxAll, whole));
	ifbr(3 == PcutIxOffset(m, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2LongUnsignedPcutClose(&lu));
	return 0;
}
static cp_t TestValue(void)
{
	//		|-Data: long-unsigned, u16	12 08 CE	类型:18,值:2254 	
	const char whole[] = "\x12\x08\xce";
	//const int whole_size = sizeof(whole) - 1;
	const uint16_t value = 2254;

	ifbr(value == P2LongUnsignedToValue(true, whole));
	return 0;
}
static cp_t TestFill(void)
{
	//		|-Data: long-unsigned, u16	12 08 CE	类型:18,值:2254 	
	const char whole[] = "\x12\x08\xce";
	const int whole_size = sizeof(whole) - 1;
	const uint16_t value = 2254;

	Pfill fill = kPfillDef;
	Pfill *m = &fill;
	ifer(PfillOpen(m));
	ifer(P2LongUnsignedFillInit(m, true, value));


	ifbr(3 == whole_size);
	char mem[3] = { 0 };
	ifer(PfillDo(m, 0, mem, 3));
	ifbr(3 == PfillIxLen(m, kPfillIxAll));
	ifbr(0 == memcmp(mem, whole, 3));

	ifer(PfillClose(m));
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
	TestFill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2LongUnsignedTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2LongUnsignedTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


