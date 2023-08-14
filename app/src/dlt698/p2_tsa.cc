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

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

TSA 目标服务器地址TSA（Target Server Address）
*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "qos/qmem.h"

#include "a_xdr.h"
#include "p2_datatype_with_content.h"

#include "p2_tsa.h"
#include "p2_tsa.xcp.h"


#define TEST_EN					(1)
#define kThisDatatype			(kDlt698DataTypeTsa)
#define kThisCutNum				(kP2TsaCutNum)

//{{{ misc
static const P2TsaPcut *ToDerive(const Pcut *part)
{
	return (P2TsaPcut*)(part);
}
//}}}


//{{{ datatype
/*
static int DatatypeSize(const Pcut *part, const char *whole) 
{
	const P2TsaPcut *derive = ToDerive(part);
	if (!derive->datatype_exist)
		return 0;

	dvb(kDlt698DataTypeTsa == *whole);
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
	return kP2TsaDatatypeOffset; 
}
static cp_t ValidDatatype(Pcut *part, int ix, const char *whole) 
{ 
	const P2TsaPcut *derive = ToDerive(part);
	if (derive->datatype_exist)
	{
		ifbr(kDlt698DataTypeTsa == *whole);
		return 0;
	}
	return 0; 
}
*/
//}}}


//{{{ var_len
static int VarlenOffset(const Pcut *part, const char *whole)
{
	return kP2TsaVarlenOffset;
}
static const char *VarlenMem(const char *whole)
{
	const char *mem = whole + kP2TsaVarlenOffset;
	return mem;
}
static int VarlenSize(const Pcut *part, const char *whole)
{
	const char *mem = VarlenMem(whole);
	return VariableLenIntByteNum(*mem);
	// return Qdlt698OctetStringLenSize(mem);
	// return Qdlt698TsaLenSize(mem);
}
static int LenVarlen(Pcut *part, int ix, const char *whole) { return VarlenSize(part, whole); }
static int OffsetVarlen(Pcut *part, int ix, const char *whole) { return VarlenOffset(part, whole); }
static cp_t ValidVarlen(Pcut *part, int ix, const char *whole) 
{ 
	const char *mem = VarlenMem(whole);
	ifbr(kVariableLenInvalidLen != VariableLenIntValue(mem));
	return 0; 
}
static cp_t ExplainVarlen(Pcut *part, int ix, const char *whole) 
{
	const char *mem = VarlenMem(whole);
	const int value = VariableLenIntValue(mem);
	qos_printf("%dD", value);
	return 0;
}

//}}}


//{{{ content
static int LenContent(Pcut *part, int ix, const char *whole) 
{ 
	const char *mem = VarlenMem(whole);
	// const int content_size = Qdlt698TsaContentSize(mem);
	const int content_size = VariableLenIntValue(mem);
var(content_size);
	return content_size;
}
static int OffsetContent(Pcut *part, int ix, const char *whole) 
{ 
	const int varlen_size = VarlenSize(part, whole);
	return kP2TsaContentOffset(varlen_size);
}
cp_t P2TsaContentSizeValid(int content_size)
{
	ifbr(kP2TsaContentSizeMin <= content_size);
	ifbr(content_size <= kP2TsaContentSizeMax);
	return 0;
}
static cp_t ValidContent(Pcut *part, int ix, const char *whole) 
{
	const char *mem = VarlenMem(whole);
	const int content_size = VariableLenIntValue(mem);
	ifer(P2TsaContentSizeValid(content_size));
	return 0;
}
//}}}


//{{{ all
int P2TsaCutSize(const char *whole)
{
	return Qdlt698OctetStringLenContentSize(whole);
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "varlen", LenVarlen, OffsetVarlen, ValidVarlen, ExplainVarlen },
	{ "content", LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2TsaCutIxVarlen]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2TsaCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2TsaPcutOpen(P2TsaPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2AddrPcutOpen(&m->addr));
	PcutSubSet(&m->base, kP2TsaCutIxContent, &m->addr.base, "addr");
	return 0;
}
cp_t P2TsaPcutClose(P2TsaPcut *m)
{
	dve(P2TsaPcutValid(m));

	PcutSubSet(&m->base, kP2TsaCutIxContent, NULL, NULL);
	ifer(P2AddrPcutClose(&m->addr));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2TsaPcutValid(const P2TsaPcut *m)
{
	ifer(P2AddrPcutValid(&m->addr));
	ifer(PcutValid(&m->base));
	return 0;
}


PCUT_DEFINE_OPEN_BASE(P2TsaPcut);
PCUT_DEFINE_CLOSE_BASE(P2TsaPcut);
/*
cp_t P2TsaPcutOpenBase(Pcut *base)
{
	P2TsaPcut *m = (P2TsaPcut*)base;
	return P2TsaPcutOpen(m);
}
cp_t P2TsaPcutCloseBase(Pcut *base)
{
	P2TsaPcut *m = (P2TsaPcut*)base;
	return P2TsaPcutClose(m);
}
*/
//}}}



//{{{ fill_by_string
/*
typedef struct 
{
	PfillItem base;
} FillItemDataType;
static cp_t FillItemProcessDataType(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	mem[offset] = kDlt698DataTypeTsa;
	*fill_size = 1;
	return 0;
}
#define kFillItemDataTypeDef {			\
	PFILL_ITEM(kP2TsaNameDatatype, PfillItemOffsetFix, FillItemProcessDataType, kP2TsaDatatypeOffset, NULL)			\
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
	PFILL_ITEM(kP2TsaNameVarlen, PfillItemOffsetFollow, FillItemProcessVarLen, 0, NULL),			\
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
	PFILL_ITEM(kP2TsaNameContent, PfillItemOffsetFollow, FillItemProcessContent, 0, NULL),			\
	(_size), {0},				\
}

cp_t P2TsaFillInitByMem(Pfill *m, bool datatype_exist, const char *string, int size)
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
	// 内容所在的索引号，看P2TsaFillInitByFill()中注册FillItem的顺序
	// 因为datatype可能存在，也可能不存在，所以ix_content不是固定的数。但一定是当前var_len的前一个
	const int ix_content = ix - 1;		
	const int content_size = PfillIxLen(fill, ix_content);
	ifbr(0 <= content_size);
	const int content_offset = PfillIxOffset(fill, ix_content, mem, mem_size);
	// 在P2TsaFillInitByFill()中，content和var_len填写的位置一样，都是datatype后的位置
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



cp_t P2TsaFillInitByFill(Pfill *m, bool datatype_exist, Pfill *fill_string)
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
	const PfillItem kFiString = PFILL_ITEM(kP2TsaNameContent, PfillItemOffsetFix, PfillItemProcessBySub, offset_after_datatype, fill_string); 
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiString), &kFiString));

	const FillItemVarLenByFill kFiVl = PFILL_ITEM(kP2TsaNameVarlen, PfillItemOffsetFix, FillItemProcessVarLenByFill, offset_after_datatype, NULL); 
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVl), &kFiVl));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcutDatatypeNotExist(void)
{
	return 0;
}
static cp_t TestPcutDatatypeExist(void)
{
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcutDatatypeNotExist,
	TestPcutDatatypeExist,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2TsaTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2TsaTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


