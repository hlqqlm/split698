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

20230811-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

visible-string
*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "qos/qmem.h"

#include "a_xdr.h"
#include "p2_datatype_with_content.h"

#include "p2_visible_string.h"
#include "p2_visible_string.xcp.h"


#define TEST_EN					(1)
#define kThisDatatype			(kDlt698DataTypeVisibleString)
#define kThisCutNum				(kP2VisibleStringCutNum)


//{{{ misc
static const P2VisibleStringPcut *ToDerive(const Pcut *part)
{
	return (P2VisibleStringPcut*)(part);
}
//}}}


//{{{ datatype
/*
static int DatatypeSize(const Pcut *part, const char *whole) 
{
	const P2VisibleStringPcut *derive = ToDerive(part);
	if (!derive->datatype_exist)
		return 0;

	dvb(kDlt698DataTypeVisibleString == *whole);
	return kDlt698DatatypeSize;
}
*/
/*
static int LenDatatype(Pcut *part, int ix, const char *whole) 
{ 
	return DatatypeSize(part, whole);
}
static int OffsetDatatype(Pcut *part, int ix, const char *whole) 
{ 
	return kP2VisibleStringDatatypeOffset; 
}
static cp_t ValidDatatype(Pcut *part, int ix, const char *whole) 
{ 
	const P2VisibleStringPcut *derive = ToDerive(part);
	if (derive->datatype_exist)
	{
		ifbr(kDlt698DataTypeVisibleString == *whole);
		return 0;
	}
	return 0; 
}
*/
//}}}


//{{{ var_len
static int VarlenOffset(const Pcut *part, const char *whole)
{
	//const int datatype_size = DatatypeSize(part, whole);
	return kP2VisibleStringVarlenOffset;
}
static const char *VarlenMem(const Pcut *part, const char *whole)
{
	const char *mem = whole + VarlenOffset(part, whole);
	return mem;
}
static int VarlenSize(const Pcut *part, const char *whole)
{
	const char *mem = VarlenMem(part, whole);
	return Qdlt698VisibleStringLenSize(mem);
}
static int LenVarlen(Pcut *part, int ix, const char *whole) { return VarlenSize(part, whole); }
static int OffsetVarlen(Pcut *part, int ix, const char *whole) { return VarlenOffset(part, whole); }
static cp_t ValidVarlen(Pcut *part, int ix, const char *whole) 
{ 
	const char *mem = VarlenMem(part, whole);
	ifbr(kVariableLenInvalidLen != VariableLenIntValue(mem));
	return 0; 
}
//}}}


//{{{ content
static int LenContent(Pcut *part, int ix, const char *whole) 
{ 
	const char *mem = VarlenMem(part, whole);
	const int content_size = Qdlt698VisibleStringContentSize(mem);
	return content_size;
}
static int OffsetContent(Pcut *part, int ix, const char *whole) 
{ 
	//const int datatype_size = DatatypeSize(part, whole);
	const int varlen_size = VarlenSize(part, whole);
	return kP2VisibleStringContentOffset(varlen_size);
}
static cp_t ValidContent(Pcut *part, int ix, const char *whole) 
{
	return 0;
}
//}}}


//{{{ all
int P2VisibleStringCutSize(const char *whole)
{
	//if (datatype_exist)
	//	return Qdlt698VisibleStringTypeLenContentSize(whole);
	return Qdlt698VisibleStringLenContentSize(whole);
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	//{ "datatype", LenDatatype, OffsetDatatype, ValidDatatype },
	{ "varlen", LenVarlen, OffsetVarlen, ValidVarlen, NULL },
	{ "content", LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	// PCUT_ITEM_NO_SUB(&kCutFix[kP2VisibleStringCutIxDatatype]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2VisibleStringCutIxVarlen]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2VisibleStringCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2VisibleStringPcutOpen(P2VisibleStringPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2VisibleStringPcutClose(P2VisibleStringPcut *m)
{
	dve(P2VisibleStringPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2VisibleStringPcutValid(const P2VisibleStringPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
/*
void P2VisibleStringPcutConfigDatatypeExist(P2VisibleStringPcut *m, bool exist)
{
	dve(P2VisibleStringPcutValid(m));
	m->datatype_exist = exist;
	return;
}
*/

cp_t P2VisibleStringPcutOpenBase(Pcut *base)
{
	P2VisibleStringPcut *m = (P2VisibleStringPcut*)base;
	return P2VisibleStringPcutOpen(m);
}
cp_t P2VisibleStringPcutCloseBase(Pcut *base)
{
	P2VisibleStringPcut *m = (P2VisibleStringPcut*)base;
	return P2VisibleStringPcutClose(m);
}
//}}}


#if 0
//{{{ pcut-datatype
// 有数据类型的pcut
cp_t P2VisibleStringDatatypePcutOpen(P2VisibleStringPcut *m)
{
	ifer(P2VisibleStringPcutOpen(m));
	P2VisibleStringPcutConfigDatatypeExist(m, true);
	return 0;
}
cp_t P2VisibleStringDatatypePcutClose(P2VisibleStringPcut *m)
{
	ifer(P2VisibleStringPcutClose(m));
	return 0;
}
cp_t P2VisibleStringDatatypePcutValid(const P2VisibleStringPcut *m)
{
	ifer(P2VisibleStringPcutValid(m));
	ifbr(m->datatype_exist);
	return 0;
}
//}}}
#endif

//{{{ fill_by_string
/*
typedef struct 
{
	PfillItem base;
} FillItemDataType;
static cp_t FillItemProcessDataType(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	mem[offset] = kDlt698DataTypeVisibleString;
	*fill_size = 1;
	return 0;
}
#define kFillItemDataTypeDef {			\
	PFILL_ITEM(kP2VisibleStringNameDatatype, PfillItemOffsetFix, FillItemProcessDataType, kP2VisibleStringDatatypeOffset, NULL)			\
}
*/


typedef struct 
{
	PfillItem base;
	int size;
} FillItemVarLen;
static cp_t FillItemProcessVarLen(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemVarLen *derive = (FillItemVarLen*)fi;

	const int byte_num = N2AXdr(mem + offset, derive->size);
	*fill_size = byte_num;
	return 0;
}
#define kFillItemVarLenDef(_size) {			\
	PFILL_ITEM(kP2VisibleStringNameVarlen, PfillItemOffsetFollow, FillItemProcessVarLen, 0, NULL),			\
	(_size)					\
}


typedef struct 
{
	PfillItem base;
	int size;
	char content[1];		// 变长数据缓冲区
} FillItemContent;
static cp_t FillItemProcessContent(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemContent *derive = (FillItemContent*)fi;

	ifbr(derive->size <= (mem_size - offset));
	memcpy(mem + offset, derive->content, derive->size);
	*fill_size = derive->size;
	return 0;
}
#define kFillItemContentDef(_size) {			\
	PFILL_ITEM(kP2VisibleStringNameContent, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_size), {0},				\
}

cp_t P2VisibleStringFillInitByMem(Pfill *m, bool datatype_exist, const char *string, int size)
{
	ifer(P2DatatypeWithContentFillInit(m, datatype_exist, kThisDatatype));
	/*
	if (datatype_exist)
	{
		const FillItemDataType kFiDt = kFillItemDataTypeDef;
		ifbr(NULL != PfillItemFactory(m, sizeof(FillItemDataType), &kFiDt));
	}
	*/

	const FillItemVarLen kFiVl = kFillItemVarLenDef(size);
	ifbr(NULL != PfillItemFactory(m, sizeof(FillItemVarLen), &kFiVl));

	// 柔性数组
	const int fi_content_size = sizeof(FillItemContent) + size * sizeof(char);
	const FillItemContent kFiContent = kFillItemContentDef(size);
	FillItemContent *fi_content = (FillItemContent *)PfillItemFactory(m, fi_content_size, &kFiContent);
	ifbr(NULL != fi_content);
	// 将内容拷贝结构体尾部数组
	memcpy(fi_content->content, string, size);

	return 0;
}
//}}}


//{{{ fill_by_fill
typedef struct 
{
	PfillItem base;
} FillItemVarLenByFill;
static cp_t FillItemProcessVarLenByFill(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	dvb(2 == ix || 1 == ix);
	//const PfillItem *fi = PfillIxItemConst(fill, ix);
	//const FillItemVarLen *derive = (FillItemVarLen*)fi;

	// 得到content的信息，准备将content往后移动
	// 内容所在的索引号，看P2VisibleStringFillInitByFill()中注册FillItem的顺序
	// 因为datatype可能存在，也可能不存在，所以ix_content不是固定的数。但一定是当前var_len的前一个
	const int ix_content = ix - 1;		
	const int content_size = PfillIxLen(fill, ix_content);
	ifbr(0 <= content_size);
	const int content_offset = PfillIxOffset(fill, ix_content, mem, mem_size);
	// 在P2VisibleStringFillInitByFill()中，content和var_len填写的位置一样，都是datatype后的位置
	dvb(offset == content_offset);
	ifbr(0 <= content_offset);
	ifbr(content_offset < mem_size);
	ifbr(content_offset + content_size <= mem_size);

	char var_len_mem[kVariableLenIntContentByteNumMax];
	const int var_len_size = N2AXdr(var_len_mem, content_size);
	dvb(var_len_size <= kVariableLenIntContentByteNumMax);
	ifbr(var_len_size + content_offset + content_size <= mem_size);

	memmove(mem + content_offset + var_len_size, mem + content_offset, content_size);
	memcpy(mem + offset, var_len_mem, var_len_size);
	*fill_size = var_len_size;
	// 注意：不能改写FillItem content中的offset，否则再次填写时，内容就是错误的。
	return 0;
}



cp_t P2VisibleStringFillInitByFill(Pfill *m, bool datatype_exist, Pfill *fill_string)
{
	/*
	if (datatype_exist)
	{
		const FillItemDataType kFiDt = kFillItemDataTypeDef;
		ifbr(NULL != PfillItemFactory(m, sizeof(FillItemDataType), &kFiDt));
	}
	*/
	ifer(P2DatatypeWithContentFillInit(m, datatype_exist, kThisDatatype));

	// 在datatype之后的位置
	const int offset_after_datatype = datatype_exist ? kDlt698DatatypeSize : 0;

	// 下面用了非常特殊的技术：
	// 因为var_len，在fill_string执行之前，并不知道其长度，因此，需要先填写fill_string，再填写var_len。
	// 关键var_len也是变长的。如果不想再申请过一块巨大空间的话，直接在用户传递过来的mem上填写的话，
	// 需要先填写fill_string，然后，计算var_len长度size，
	// 将string内容往后移动size字节，然后再填写var_len内容
	// 所以：下面两个fill_item的offset都是offset_after_datatype
	const PfillItem kFiString = PFILL_ITEM(kP2VisibleStringNameContent, PfillItemOffsetFix, PfillItemProcessBySub, offset_after_datatype, fill_string); 
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiString), &kFiString));

	const FillItemVarLenByFill kFiVl = PFILL_ITEM(kP2VisibleStringNameVarlen, PfillItemOffsetFix, FillItemProcessVarLenByFill, offset_after_datatype, NULL); 
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVl), &kFiVl));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcutDatatypeNotExist(void)
{
	P2VisibleStringPcut pcut = kP2VisibleStringPcutDef;
	Pcut * const m = &pcut.base;
	ifer(P2VisibleStringPcutOpen(&pcut));
	ifer(P2VisibleStringPcutValid(&pcut));


	// 15 05 02 00 04 20 00 02 00 20 01 02 00 20 04 02 00 00 10 02 00 00 
	const char whole[] = "\x15\x05\x02\x00\x04\x20\x00\x02\x00\x20\x01\x02\x00\x20\x04\x02\x00\x00\x10\x02\x00\x00";

	//ifbr(0 == PcutIxLen(m, kP2VisibleStringCutIxDatatype, whole));
	ifbr(1 == PcutIxLen(m, kP2VisibleStringCutIxVarlen, whole));
	ifbr(21 == PcutIxLen(m, kP2VisibleStringCutIxContent, whole));
	ifbr(22 == PcutIxLen(m, kPcutIxAll, whole));

	//ifbr(0 == PcutIxOffset(m, kP2VisibleStringCutIxDatatype, whole));
	//ifbr(0 == PcutIxOffset(m, kP2VisibleStringCutIxVarlen, whole));
	ifbr(1 == PcutIxOffset(m, kP2VisibleStringCutIxContent, whole));
	ifbr(22 == PcutIxOffset(m, kPcutIxAll, whole));

	//ifer(PcutIxValid(m, kP2VisibleStringCutIxDatatype, whole));
	ifer(PcutIxValid(m, kP2VisibleStringCutIxVarlen, whole));
	ifer(PcutIxValid(m, kP2VisibleStringCutIxContent, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2VisibleStringPcutClose(&pcut));
	return 0;
}
static cp_t TestPcutDatatypeExist(void)
{
	P2DatatypeWithContentPcut dwc = kP2DatatypeWithContentPcutDef;
	ifer(P2DatatypeWithContentPcutOpen(&dwc));


	P2VisibleStringPcut pcut = kP2VisibleStringPcutDef;
	//Pcut * const m = &pcut.base;
	ifer(P2VisibleStringPcutOpen(&pcut));
	ifer(P2VisibleStringPcutValid(&pcut));

	ifer(P2DatatypeWithContentPcutSetContent(&dwc, kThisDatatype, &pcut.base, kP2VisibleStringName));
	Pcut *m = &dwc.base;
	//P2VisibleStringPcutConfigDatatypeExist(&pcut, true);

	// 09 06 10 00 00 00 22 21
	const char whole[] = "\x09\x06\x10\x00\x00\x00\x22\x21";

	ifbr(1 == PcutIxLen(m, kP2DatatypeWithContentCutIxDatatype, whole));
	ifbr(7 == PcutIxLen(m, kP2DatatypeWithContentCutIxContent, whole));
	const char *os_mem = PcutIxPtrConst(m, kP2DatatypeWithContentCutIxContent, whole);		// const版本
	ifbr(1 == PcutIxLen(&pcut.base, kP2VisibleStringCutIxVarlen, os_mem));
	ifbr(6 == PcutIxLen(&pcut.base, kP2VisibleStringCutIxContent, os_mem));
	ifbr(8 == PcutIxLen(m, kPcutIxAll, whole));

	ifbr(0 == PcutIxOffset(m, kP2DatatypeWithContentCutIxDatatype, whole));
	ifbr(1 == PcutIxOffset(m, kP2DatatypeWithContentCutIxContent, whole));
	ifbr(8 == PcutIxOffset(m, kPcutIxAll, whole));

	ifer(PcutIxValid(m, kP2DatatypeWithContentCutIxDatatype, whole));
	ifer(PcutIxValid(m, kP2DatatypeWithContentCutIxContent, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2VisibleStringPcutClose(&pcut));
	ifer(P2DatatypeWithContentPcutClose(&dwc));
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcutDatatypeNotExist,
	TestPcutDatatypeExist,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2VisibleStringTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2VisibleStringTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


