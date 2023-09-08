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

array
*/

#include "qos/qcp.h"
#include "qos/qstr.h"
#include "qos/qtest.h"
#include "qos/qmem.h"
#include "a_xdr.h"
#include "p2_array.h"
#include "p2_array.xcp.h"
//#define this_file_id	0x975d05d7  // echo -n dlt698_45_array.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ArrayCutNum)

//{{{ nvar
static cp_t NvarOpen(P2ArrayPcut *m, int num)
{
	dve(P2ArrayPcutValid(m));

	if (m->nvar_open_cnt)
	{
		dvb(1 == m->nvar_open_cnt);
		return 0;
	}

	dvb(0 == m->nvar_open_cnt);
	++m->nvar_open_cnt;
	ifer(P2DataChoicePcutOpen(&m->one_data_choice));
	ifer(P2NvarPcutOpen(&m->nvar_cut, num, &m->one_data_choice.choice.base, "content"));
	PcutSubSet(&m->base, kP2ArrayCutIxContent, &m->nvar_cut.base, NULL);
	return 0;
}
static cp_t NvarClose(P2ArrayPcut *m)
{
	dve(P2ArrayPcutValid(m));
	if (0 == m->nvar_open_cnt)
		return 0;

	dvb(1 == m->nvar_open_cnt);
	--m->nvar_open_cnt;
	PcutSubSet(&m->base, kP2ArrayCutIxContent, NULL, NULL);
	ifer(P2NvarPcutClose(&m->nvar_cut));
	ifer(P2DataChoicePcutClose(&m->one_data_choice));
	return 0;
}
//}}}


//{{{ misc
//static const P2ArrayPcut *ToDerive(const Pcut *cut)
//{
//	return (P2ArrayPcut*)(cut);
//}
//}}}


#if 0
//{{{ datatype
static int DatatypeSize(const char *whole) 
{
	dvb(kDlt698DataTypeArray == *whole);
	return kDlt698DatatypeSize;
}
static int LenDatatype(Pcut *cut, int ix, const char *whole) 
{ 
	return DatatypeSize(whole);
}
static int OffsetDatatype(Pcut *cut, int ix, const char *whole) 
{ 
	return kP2ArrayDatatypeOffset; 
}
static cp_t ValidDatatype(Pcut *cut, int ix, const char *whole) 
{ 
	ifbr(kDlt698DataTypeArray == *whole);
	return 0;
}
//}}}
#endif


//{{{ var_len
static int VarlenOffset(const char *whole)
{
	return kP2ArrayVarlenOffset;
}
static const char *VarlenMem(const char *whole)
{
	const char *mem = whole + VarlenOffset(whole);
	return mem;
}
int P2ArrayVarlenSize(const char *whole)
{
	const char *mem = VarlenMem(whole);
	return VariableLenIntByteNum(*mem);
}
static int LenVarlen(Pcut *cut, int ix, const char *whole) { return P2ArrayVarlenSize(whole); }
static int OffsetVarlen(Pcut *cut, int ix, const char *whole) { return VarlenOffset(whole); }
static cp_t ValidVarlen(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = VarlenMem(whole);
	ifbr(kVariableLenInvalidLen != VariableLenIntValue(mem));
	return 0; 
}
static cp_t ExplainVarlen(Pcut *cut, int ix, const char *whole) 
{
	const char *mem = VarlenMem(whole);
	const int value = VariableLenIntValue(mem);
	qos_printf("%dD", value);
	return 0;
}
//}}}


//{{{ content
int P2ArrayContentOffset(const char *whole)
{
	const int varlen_size = P2ArrayVarlenSize(whole);
	return kP2ArrayContentOffset(varlen_size);
}
static int LenContent(Pcut *cut, int ix, const char *whole) 
{ 
	P2ArrayPcut *derive = (P2ArrayPcut*)cut;
	const int num = P2ArrayNum(whole);

	ifed(NvarOpen(derive, num));
	return PcutItemLenBySub(cut, ix, whole);
}
#define OffsetContent		PcutItemOffsetDef
static cp_t ValidContent(Pcut *cut, int ix, const char *whole) 
{
	P2ArrayPcut *derive = (P2ArrayPcut*)cut;
	const int num = P2ArrayNum(whole);

	ifer(NvarOpen(derive, num));
	return PcutItemValidBySub(cut, ix, whole);
}
//}}}


//{{{ all
int P2ArrayCutSize(const char *whole)
{
	// 用P2DataChoice来解析每个元素大小
	P2ArrayPcut array = kP2ArrayPcutDef;
	const cp_t cp = P2ArrayPcutOpen(&array);
	if (0 != cp)
	{
		dve(cp);
		return 0;
	}

	const int size = PcutIxLen(&array.base, 0, kPcutIxAll, whole);
	ifed(P2ArrayPcutClose(&array));
	return size;
}
int P2ArrayNum(const char *whole)
{
	const char *mem = whole + kP2ArrayVarlenOffset;
	return VariableLenIntValue(mem);
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	//{ kP2ArrayNameDatatype, LenDatatype, OffsetDatatype, ValidDatatype, NULL },
	{ kP2ArrayNameVarlen, LenVarlen, OffsetVarlen, ValidVarlen, ExplainVarlen },
	{ kP2ArrayNameContent, LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	//PCUT_ITEM_NO_SUB(&kCutFix[kP2ArrayCutIxDatatype]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ArrayCutIxVarlen]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ArrayCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2ArrayPcutOpen(P2ArrayPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2ArrayPcutClose(P2ArrayPcut *m)
{
	dve(P2ArrayPcutValid(m));
	ifer(PcutClose(&m->base));
	ifer(NvarClose(m));
	return 0;
}
cp_t P2ArrayPcutValid(const P2ArrayPcut *m)
{
	ifer(PcutValid(&m->base));
	ifbr(0 == m->nvar_open_cnt || 1 == m->nvar_open_cnt);
	if (0 < m->nvar_open_cnt)
	{
		ifer(P2NvarPcutValid(&m->nvar_cut));
		ifer(P2DataChoicePcutValid(&m->one_data_choice));
	}
	return 0;
}

cp_t P2ArrayPcutOpenBase(Pcut *base)
{
	P2ArrayPcut *m = (P2ArrayPcut*)base;
	return P2ArrayPcutOpen(m);
}
cp_t P2ArrayPcutCloseBase(Pcut *base)
{
	P2ArrayPcut *m = (P2ArrayPcut*)base;
	return P2ArrayPcutClose(m);
}



// 数组下标是否合法
cp_t P2ArrayPcutArrayIxValid(int ix, const char *whole)
{
	const int num = P2ArrayNum(whole);
	ifbr(0 <= ix);
	ifbr(ix < num);
	return 0;
}
int P2ArrayPcutIxLen(P2ArrayPcut *m, int ix, const char *whole)
{
	dve(P2ArrayPcutArrayIxValid(ix, whole));
	// const int content_offset = P2ArrayContentOffset(whole);
	const char *content = PcutIxPtrConst(&m->base, kP2ArrayCutIxContent, whole);
	return PcutIxLen(&m->nvar_cut.base, 0, ix, content);
}
int P2ArrayPcutIxOffset(P2ArrayPcut *m, int ix, const char *whole)
{
	dve(P2ArrayPcutArrayIxValid(ix, whole));
	// const int content_offset = kP2ArrayContentOffset(P2ArrayVarlenSize(whole));
	const int content_offset = P2ArrayContentOffset(whole);
	const char *content = PcutIxPtrConst(&m->base, kP2ArrayCutIxContent, whole);
	return content_offset + PcutIxOffset(&m->nvar_cut.base, 0, ix, content);
}
cp_t P2ArrayPcutIxValid(P2ArrayPcut *m, int ix, const char *whole)
{
	dve(P2ArrayPcutArrayIxValid(ix, whole));
	const char *content = PcutIxPtrConst(&m->base, kP2ArrayCutIxContent, whole);
	return PcutIxValid(&m->nvar_cut.base, ix, content);
}
//}}}


//{{{ fill
#define kP2ArrayDatatypeOffset		(0)
typedef struct 
{
	PfillItem base;
} FillItemDataType;
static cp_t FillItemProcessDataType(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	mem[offset] = kDlt698DataTypeArray;
	*fill_size = 1;
	return 0;
}
#define kFillItemDataTypeDef {			\
	PFILL_ITEM(kP2ArrayNameDatatype, PfillItemOffsetFix, FillItemProcessDataType, kP2ArrayDatatypeOffset, NULL)			\
}


typedef struct 
{
	PfillItem base;
} FillItemVarLen;
static cp_t FillItemProcessVarLen(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	//const PfillItem *fi = PfillIxItemConst(fill, ix);
	//const FillItemVarLen *derive = (FillItemVarLen*)fi;
	const int fill_item_num = PfillItemNum(fill);

	// 前两个item是：datatype + varlen
	ifbr(2 <= fill_item_num);
	const int content_item_num = fill_item_num - 2;

	const int byte_num = N2AXdr(mem + offset, content_item_num);
	*fill_size = byte_num;
	return 0;
}
#define kFillItemVarLenDef {			\
	PFILL_ITEM(kP2ArrayNameVarlen, PfillItemOffsetFollow, FillItemProcessVarLen, 0, NULL),			\
}



// 暂时不考虑datatype不存在的情况
cp_t P2ArrayFillInitThenAdd(Pfill *m, Pfill *fill_one_content)
{
	dve(PfillValid(m));
	dve(PfillValid(fill_one_content));

	// 如果item数目为0，说明需要加datatype + varlen
	if (0 == PfillItemNum(m))
	{
		const FillItemDataType kFiDt = kFillItemDataTypeDef;
		ifbr(NULL != PfillItemFactory(m, sizeof(FillItemDataType), &kFiDt));

		const FillItemVarLen kFiVarLen = kFillItemVarLenDef;
		ifbr(NULL != PfillItemFactory(m, sizeof(FillItemVarLen), &kFiVarLen));
	}

	const PfillItem kFiItem = PFILL_ITEM(kP2ArrayNameContent, PfillItemOffsetFollow, PfillItemProcessBySub, 0, fill_one_content); 
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiItem), &kFiItem));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
#include "p2_double_long_unsigned.h"
// 01 05 06 00 00 00 0b 06 00 00 00 00 06 00 00 00 06 06 00 00 00 02 06 00 00 00 02
// |-Data: array, len	01 05	类型:1,SequenceOf长度=5 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 0B	类型:6,值:11 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 00	类型:6,值:0 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 06	类型:6,值:6 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 02	类型:6,值:2 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 02	类型:6,值:2 	
static cp_t TestPcut(void)
{
	P2ArrayPcut pcut = kP2ArrayPcutDef;
	Pcut * const m = &pcut.base;
	ifer(P2ArrayPcutOpen(&pcut));
	ifer(P2ArrayPcutValid(&pcut));

	// 01 05 06 00 00 00 0b 06 00 00 00 00 06 00 00 00 06 06 00 00 00 02 06 00 00 00 02
	const char whole[] = "\x01\x05\x06\x00\x00\x00\x0b\x06\x00\x00\x00\x00\x06\x00\x00\x00\x06\x06\x00\x00\x00\x02\x06\x00\x00\x00\x02";
	const int whole_size = sizeof(whole) - 1;

	ifbr(whole_size == PcutIxLen(m, kPcutIxAll, whole));
	ifbr(whole_size == PcutIxOffset(m, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2ArrayPcutArrayIxValid(0, whole));
	ifer(P2ArrayPcutArrayIxValid(4, whole));
	ifbr(0 != P2ArrayPcutArrayIxValid(5, whole));

	ifbr(5 == P2ArrayPcutIxLen(&pcut, 0, whole));
	ifbr(5 == P2ArrayPcutIxLen(&pcut, 1, whole));
	ifbr(5 == P2ArrayPcutIxLen(&pcut, 2, whole));
	ifbr(5 == P2ArrayPcutIxLen(&pcut, 3, whole));
	ifbr(5 == P2ArrayPcutIxLen(&pcut, 4, whole));


	ifbr(2 + 0*5 == P2ArrayPcutIxOffset(&pcut, 0, whole));
	ifbr(2 + 1*5 == P2ArrayPcutIxOffset(&pcut, 1, whole));
	ifbr(2 + 2*5 == P2ArrayPcutIxOffset(&pcut, 2, whole));
	ifbr(2 + 3*5 == P2ArrayPcutIxOffset(&pcut, 3, whole));
	ifbr(2 + 4*5 == P2ArrayPcutIxOffset(&pcut, 4, whole));

	ifer(P2ArrayPcutIxValid(&pcut, 0, whole));
	ifer(P2ArrayPcutIxValid(&pcut, 1, whole));
	ifer(P2ArrayPcutIxValid(&pcut, 2, whole));
	ifer(P2ArrayPcutIxValid(&pcut, 3, whole));
	ifer(P2ArrayPcutIxValid(&pcut, 4, whole));

	ifer(P2ArrayPcutClose(&pcut));

	return 0;
}
static cp_t TestAll(void)
{
	// 01 05 06 00 00 00 0b 06 00 00 00 00 06 00 00 00 06 06 00 00 00 02 06 00 00 00 02
	const char whole[] = "\x01\x05\x06\x00\x00\x00\x0b\x06\x00\x00\x00\x00\x06\x00\x00\x00\x06\x06\x00\x00\x00\x02\x06\x00\x00\x00\x02";
	const int whole_size = sizeof(whole) - 1;

	ifbr(2 == P2ArrayContentOffset(whole));
	ifbr(whole_size == P2ArrayCutSize(whole));
	ifbr(5 == P2ArrayNum(whole));
	return 0;
}
static cp_t TestFill(void)
{
// 01 05 06 00 00 00 0b 06 00 00 00 00 06 00 00 00 06 06 00 00 00 02 06 00 00 00 02
// |-Data: array, len	01 05	类型:1,SequenceOf长度=5 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 0B	类型:6,值:11 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 00	类型:6,值:0 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 06	类型:6,值:6 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 02	类型:6,值:2 	
//		|-Data: double_long_unsigned, u32	06 00 00 00 02	类型:6,值:2 	

	Pfill fill = kPfillDef;
	ifer(PfillOpen(&fill));

	Pfill f0 = kPfillDef;
	ifer(PfillOpen(&f0));
	ifer(P2DoubleLongUnsignedFillInit(&f0, true, 11));
	ifer(P2ArrayFillInitThenAdd(&fill, &f0));

	Pfill f1 = kPfillDef;
	ifer(PfillOpen(&f1));
	ifer(P2DoubleLongUnsignedFillInit(&f1, true, 0));
	ifer(P2ArrayFillInitThenAdd(&fill, &f1));

	Pfill f2 = kPfillDef;
	ifer(PfillOpen(&f2));
	ifer(P2DoubleLongUnsignedFillInit(&f2, true, 6));
	ifer(P2ArrayFillInitThenAdd(&fill, &f2));

	Pfill f3 = kPfillDef;
	ifer(PfillOpen(&f3));
	ifer(P2DoubleLongUnsignedFillInit(&f3, true, 2));
	ifer(P2ArrayFillInitThenAdd(&fill, &f3));

	Pfill f4 = kPfillDef;
	ifer(PfillOpen(&f4));
	ifer(P2DoubleLongUnsignedFillInit(&f4, true, 2));
	ifer(P2ArrayFillInitThenAdd(&fill, &f4));

	// 01 05 06 00 00 00 0b 06 00 00 00 00 06 00 00 00 06 06 00 00 00 02 06 00 00 00 02
	const char result[] = "\x01\x05\x06\x00\x00\x00\x0b\x06\x00\x00\x00\x00\x06\x00\x00\x00\x06\x06\x00\x00\x00\x02\x06\x00\x00\x00\x02";
	const int result_size = sizeof(result) - 1;
	char mem[result_size];

	// 打开注释，可以测试组帧过程的打印
	if (PRINT_FILL_IN_TEST_EN)
		PfillSetPrintFillWithSubs(&fill, PfillPrintFillNormal);
	ifer(PfillDo(&fill, 0, mem, result_size));
	
	ifbr(0 == memcmp(mem, result, result_size));
	ifbr(result_size == 27);

	ifer(PfillClose(&f0));
	ifer(PfillClose(&fill));
	return 0;
}

static const QtestItem kTestItem[] = {
	TestPcut,
	TestAll,
	TestFill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ArrayTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2ArrayTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


