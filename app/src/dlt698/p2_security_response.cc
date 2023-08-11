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


用来描述dlt698_45中的security response
*/
#include "p2_security_response_data_choice.h"
#include "p2_security_response_verify.h"

#include "p2_security_response.h"
#include "p2_security_response.xcp.h"
//#define this_file_id	0xb6068ae9 // echo -n dlt698_45_security_response.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)



// {{{ data
#define LenData	PcutItemLenBySub	
static int OffsetData(Pcut *part, int ix, const char *whole) { return kP2SecurityResponseDataOffset; }
#define ValidData	PcutItemValidBySub
//}}}


// {{{ verify
#define LenVerify		PcutItemLenBySub
#define OffsetVerify	PcutItemOffsetDef
#define ValidVerify		PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kP2SecurityResponseCutNum] = {
	// name len offset valid
	{ kP2SecurityResponseNameData, LenData, OffsetData, ValidData },
	{ kP2SecurityResponseNameVerify, LenVerify, OffsetVerify, ValidVerify },
};
	

static const PcutItem kCutItemsPattern[kP2SecurityResponseCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SecurityResponseCutIxData]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SecurityResponseCutIxVerify]),
};
static void PcutItemsInit(PcutItem items[kP2SecurityResponseCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SecurityResponsePcutOpen(P2SecurityResponsePcut *m)
{
	ifer(P2SecurityResponseDataChoicePcutOpen(&m->data));
	ifer(P2SecurityResponseVerifyPcutOpen(&m->verify));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2SecurityResponseCutNum));

	PcutSubSet(&m->base, kP2SecurityResponseCutIxData, &m->data.choice.base, NULL);
	PcutSubSet(&m->base, kP2SecurityResponseCutIxVerify, &m->verify.base, NULL);
	return 0;
}
cp_t P2SecurityResponsePcutClose(P2SecurityResponsePcut *m)
{
	dve(P2SecurityResponsePcutValid(m));

	PcutSubSet(&m->base, kP2SecurityResponseCutIxData, NULL, NULL);
	PcutSubSet(&m->base, kP2SecurityResponseCutIxVerify, NULL, NULL);

	ifer(PcutClose(&m->base));

	ifer(P2SecurityResponseVerifyPcutClose(&m->verify));
	ifer(P2SecurityResponseDataChoicePcutClose(&m->data));
	return 0;
}
cp_t P2SecurityResponsePcutValid(const P2SecurityResponsePcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SecurityResponseDataChoicePcutValid(&m->data));
	ifer(P2SecurityResponseVerifyPcutValid(&m->verify));
	return 0;
}


cp_t P2SecurityResponsePcutOpenBase(Pcut *base)
{
	P2SecurityResponsePcut *m = (P2SecurityResponsePcut*)base;
	return P2SecurityResponsePcutOpen(m);
}
cp_t P2SecurityResponsePcutCloseBase(Pcut *base)
{
	P2SecurityResponsePcut *m = (P2SecurityResponsePcut*)base;
	return P2SecurityResponsePcutClose(m);
}
//}}}


//{{{ fill
// 参考P2ServerApduFillInit()
cp_t P2SecurityResponseFillInit(Pfill *m, Pfill *fill_data, Pfill *fill_verify)
{
	const PfillItem kFiData = PFILL_ITEM(kP2SecurityResponseNameData, PfillItemOffsetFix, PfillItemProcessBySub, kP2SecurityResponseDataOffset, fill_data);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiData), &kFiData));

	const PfillItem kFiVerify = PFILL_ITEM(kP2SecurityResponseNameVerify, PfillItemOffsetFollow, PfillItemProcessBySub, 0, fill_verify);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVerify), &kFiVerify));

	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2SecurityResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SecurityResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
