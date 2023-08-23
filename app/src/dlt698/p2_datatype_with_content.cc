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

有很多datatype + content的结构，例如double_long等。
用这个类来进行描述。
*/
#include "p2_datatype_with_content.h"
#include "p2_datatype_with_content.xcp.h"
// #define this_file_id	0x2a5cd78a  // echo -n dlt698_45_datatype_with_content.c | rhash --simple -


#define TEST_EN					(1)
#define kThisCutNum				(kP2DatatypeWithContentCutNum)


//{{{ misc
static const P2DatatypeWithContentPcut *ToDerive(const Pcut *part)
{
	return (P2DatatypeWithContentPcut*)(part);
}
//}}}


//{{{ datatype
uint8_t P2DatatypeWithContentDatatype(const char *whole)
{
	return whole[kP2DatatypeWithContentDatatypeOffset];
}
static int LenDatatype(Pcut *part, int ix, const char *whole) { return kP2DatatypeWithContentDatatypeSize; }
static int OffsetDatatype(Pcut *part, int ix, const char *whole) { return kP2DatatypeWithContentDatatypeOffset; }
static cp_t ValidDatatype(Pcut *part, int ix, const char *whole) 
{ 
	const P2DatatypeWithContentPcut *derive = ToDerive(part);
	const uint8_t datatype = whole[kP2DatatypeWithContentDatatypeOffset];
	ifbr(datatype == derive->datatype);
	return 0; 
}
static cp_t ExplainDatatype(Pcut *part, int ix, const char *whole) 
{
	qos_printf("todo");
	return 0;
}
//}}}


//{{{ content
const char *P2DatatypeWithContentContentMem(const char *whole)
{
	return whole + kP2DatatypeWithContentContentOffset;
}


#define LenContent		PcutItemLenBySub
static int OffsetContent(Pcut *part, int ix, const char *whole) { return kP2DatatypeWithContentContentOffset; }
#define ValidContent	PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "datatype", LenDatatype, OffsetDatatype, ValidDatatype, ExplainDatatype },
	{ "content", LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2DatatypeWithContentCutIxDatatype]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2DatatypeWithContentCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2DatatypeWithContentPcutOpen(P2DatatypeWithContentPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2DatatypeWithContentPcutClose(P2DatatypeWithContentPcut *m)
{
	dve(P2DatatypeWithContentPcutValid(m));
	ifer(P2DatatypeWithContentPcutSetContent(m, kDlt698DatatypeInvalid, NULL, NULL));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2DatatypeWithContentPcutValid(const P2DatatypeWithContentPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
cp_t P2DatatypeWithContentPcutSetContent(P2DatatypeWithContentPcut *m, uint16_t datatype, Pcut *content, const char *content_name)
{
	m->datatype = datatype;
	PcutSubSet(&m->base, kP2DatatypeWithContentCutIxContent, content, content_name);
	return 0;
}

cp_t P2DatatypeWithContentPcutOpenBase(Pcut *base)
{
	P2DatatypeWithContentPcut *m = (P2DatatypeWithContentPcut*)base;
	return P2DatatypeWithContentPcutOpen(m);
}
cp_t P2DatatypeWithContentPcutCloseBase(Pcut *base)
{
	P2DatatypeWithContentPcut *m = (P2DatatypeWithContentPcut*)base;
	return P2DatatypeWithContentPcutClose(m);
}
//}}}


//{{{ fill_by_string
typedef struct 
{
	PfillItem base;
	uint8_t datatype;
} FillItemDatatype;
static cp_t FillItemProcessDatatype(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemDatatype *derive = (FillItemDatatype*)fi;

	mem[offset] = derive->datatype;
	*fill_size = 1;
	return 0;
}
#define kFillItemDatatypeDef(_datatype) {			\
	PFILL_ITEM(kP2DatatypeWithContentNameDatatype, PfillItemOffsetFix, FillItemProcessDatatype, kP2DatatypeWithContentDatatypeOffset, NULL),				\
	(_datatype)					\
}

/*
cp_t P2DatatypeWithContentFillInit(Pfill *m, bool datatype_exist, uint16_t datatype, int content_size, const void *content_def)
{
	if (datatype_exist)
	{
		const FillItemDatatype kFiDt = kFillItemDatatypeDef((uint8_t)datatype);
		ifbr(NULL != PfillItemFactory(m, sizeof(FillItemDatatype), &kFiDt));
	}

	ifbr(NULL != PfillItemFactory(m, content_size, content_def));
	return 0;
}
*/
cp_t P2DatatypeWithContentFillInit(Pfill *m, bool datatype_exist, uint16_t datatype)
{
	if (datatype_exist)
	{
		const FillItemDatatype kFiDt = kFillItemDatatypeDef((uint8_t)datatype);
		ifbr(NULL != PfillItemFactory(m, sizeof(FillItemDatatype), &kFiDt));
	}

	// ifbr(NULL != PfillItemFactory(m, content_size, content_def));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
#if 0
	//	|-Data: double_long, s32	05 FF FF FF FD	类型:5,值:-3 	
	//	|-Data: double_long, s32	05 00 00 00 03	类型:5,值:3 	
	const char whole[] = "\x05\xff\xff\xff\xfd";
	const int whole_size = sizeof(whole) - 1;

	P2DatatypeWithContentPcut lu = kP2DatatypeWithContentPcutDef;
	ifer(P2DatatypeWithContentPcutOpen(&lu));
	P2DatatypeWithContentPcutConfigDatatypeExist(&lu, true);	
	Pcut *m = &lu.base;

	ifbr(5 == whole_size);
	ifbr(5 == PcutIxLen(m, kPcutIxAll, whole));
	ifbr(5 == PcutIxOffset(m, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2DatatypeWithContentPcutClose(&lu));
#endif
	return 0;
}
static cp_t TestFillPos(void)
{
#if 0
	Pfill fill = kPfillDef;
	Pfill *m = &fill;
	ifer(PfillOpen(m));
	ifer(P2DatatypeWithContentFillInit(m, true, 3));

	//	|-Data: double_long, s32	05 00 00 00 03	类型:5,值:3 	
	const char whole[] = "\x05\x00\x00\x00\x03";
	const int whole_size = sizeof(whole) - 1;

	ifbr(5 == whole_size);
	char mem[5] = { 0 };
	ifer(PfillDo(m, 0, mem, 5));
	ifbr(5 == PfillIxLen(m, kPfillIxAll));
	// printf_hex_ex("mem:", "\r\n", mem, 5, "");
	ifbr(0 == memcmp(mem, whole, 5));

	ifer(PfillClose(m));
#endif
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestFillPos,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2DatatypeWithContentTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2DatatypeWithContentTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


