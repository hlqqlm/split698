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

20230511-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的security request
*/
#include "string.h"

#include "qdlt698.h"
#include "p2_security_request.h"
#include "p2_security_request.xcp.h"
//#define this_file_id	0x5ef30e65 // echo -n dlt698_45_security_request.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kCutNum					(kP2SecurityRequestCutNum)



// {{{ data
#define LenData	PcutItemLenBySub	
static int OffsetData(Pcut *part, int ix, const char *whole) { return kP2SecurityRequestDataOffset; }
#define ValidData	PcutItemValidBySub
//}}}


// {{{ verify
#define LenVerify		PcutItemLenBySub
#define OffsetVerify	PcutItemOffsetDef
#define ValidVerify	PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kCutNum] = {
	// name len offset valid explain
	{ kP2SecurityRequestNameData, LenData, OffsetData, ValidData, NULL },
	{ kP2SecurityRequestNameVerify, LenVerify, OffsetVerify, ValidVerify, NULL },
};
	

static const PcutItem kCutItemsPattern[kCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SecurityRequestCutIxData]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SecurityRequestCutIxVerify]),
};
static void PcutItemsInit(PcutItem items[kCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SecurityRequestPcutOpen(P2SecurityRequestPcut *m)
{
	ifer(P2SecurityRequestDataChoicePcutOpen(&m->data));
	ifer(P2SecurityRequestVerifyChoicePcutOpen(&m->verify));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kCutNum));

	PcutSubSet(&m->base, kP2SecurityRequestCutIxData, &m->data.choice.base, NULL);
	PcutSubSet(&m->base, kP2SecurityRequestCutIxVerify, &m->verify.choice.base, NULL);
	return 0;
}
cp_t P2SecurityRequestPcutClose(P2SecurityRequestPcut *m)
{
	dve(P2SecurityRequestPcutValid(m));

	PcutSubSet(&m->base, kP2SecurityRequestCutIxData, NULL, NULL);
	PcutSubSet(&m->base, kP2SecurityRequestCutIxVerify, NULL, NULL);

	ifer(PcutClose(&m->base));

	ifer(P2SecurityRequestVerifyChoicePcutClose(&m->verify));
	ifer(P2SecurityRequestDataChoicePcutClose(&m->data));
	return 0;
}
cp_t P2SecurityRequestPcutValid(const P2SecurityRequestPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SecurityRequestDataChoicePcutValid(&m->data));
	ifer(P2SecurityRequestVerifyChoicePcutValid(&m->verify));
	return 0;
}


cp_t P2SecurityRequestPcutOpenBase(Pcut *base)
{
	P2SecurityRequestPcut *m = (P2SecurityRequestPcut*)base;
	return P2SecurityRequestPcutOpen(m);
}
cp_t P2SecurityRequestPcutCloseBase(Pcut *base)
{
	P2SecurityRequestPcut *m = (P2SecurityRequestPcut*)base;
	return P2SecurityRequestPcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}
static cp_t TestQpack(void)
{
	P2SecurityRequestQpack grn = kP2SecurityRequestQpackDef;
	ifer(P2SecurityRequestQpackOpen(&grn));
	Qpack * const pack = &grn.base;

	const P2SecurityRequestValue value = {0, 0x40010200};
	// 下面两个方法等效，都是设置value的值
	ifer(QpackValueCopy(pack, &value, sizeof(value)));
	grn.value = value;

	if (PRINT_PACK_IN_TEST_EN)
		QpackSetPrintFill(pack, QpackPrintFillNormal);

	char mem[P2_GRN_WHOLE_SIZE];
	ifer(QpackDo(pack, mem, sizeof(mem)));

	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char result[] = "\x00\x40\x01\x02\x00";
	const int result_size = sizeof(result) - 1;

	ifbr(result_size == QpackIxLen(pack, kQpackIxAll));
	ifbr(0 == memcmp(result, mem, result_size));

	ifer(P2SecurityRequestQpackClose(&grn));
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2SecurityRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SecurityRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
