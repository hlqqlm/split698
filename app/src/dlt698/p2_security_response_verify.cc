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

20230607-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 security response verify部分报文解析
*/

// var
#include "p2_octet_string.h"
#include "p2_choice.h"

#include "p2_security_response_verify.h"
#include "p2_security_response_verify.xcp.h"
//#define this_file_id	0x41321f41	// echo -n dlt698_45_security_response_verify.c | rhash --simple -


#define TEST_EN				(0)


// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2SecurityResponseVerifyChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2SecurityResponseVerifyChoiceMac, "mac" },	// MAC
};
int P2SecurityResponseVerifyChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2SecurityResponseVerifyChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2SecurityResponseVerifyChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2SecurityResponseVerifyChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ cut
#define LenOptionalMac		PcutItemLenBySub
#define OffsetOptionalMac	PcutItemOffsetDef
#define ValidOptionalMac	PcutItemValidBySub

// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2SecurityResponseVerifyPartNum] = {
	// name len offset valid explain
	{ "optional_mac", LenOptionalMac, OffsetOptionalMac, ValidOptionalMac, NULL },
};
	

static const PcutItem kPartItemsPattern[kP2SecurityResponseVerifyPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2SecurityResponseVerifyPartIxOptionalMac]),
};
static void PcutItemsInit(PcutItem items[kP2SecurityResponseVerifyPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}



cp_t P2SecurityResponseVerifyPcutOpen(P2SecurityResponseVerifyPcut *m)
{
	ifer(P2OctetStringPcutOpen(&m->mac_cut));
	ifer(P2OptionalPcutOpen(&m->optional_cut, &m->mac_cut.base, kP2SecurityResponseVerifyNameMac));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2SecurityResponseVerifyPartNum));

	PcutSubSet(&m->base, kP2SecurityResponseVerifyPartIxOptionalMac, &m->optional_cut.base, NULL);
	return 0;
}
cp_t P2SecurityResponseVerifyPcutClose(P2SecurityResponseVerifyPcut *m)
{
	dve(P2SecurityResponseVerifyPcutValid(m));

	PcutSubSet(&m->base, kP2SecurityResponseVerifyPartIxOptionalMac, NULL, NULL);

	ifer(PcutClose(&m->base));
	ifer(P2OptionalPcutClose(&m->optional_cut));
	ifer(P2OctetStringPcutClose(&m->mac_cut));
	return 0;
}
cp_t P2SecurityResponseVerifyPcutValid(const P2SecurityResponseVerifyPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2OptionalPcutValid(&m->optional_cut));
	ifer(P2OctetStringPcutValid(&m->mac_cut));
	return 0;
}
//}}}


//{{{ fill
// 参考P2ServerApduFillInit()
cp_t P2SecurityResponseVerifyFillInit(Pfill *m, Pfill *fill_mac)
{
	const uint8_t optional_mac = ((NULL == fill_mac) ? 0 : 1);
	const P2OptionalFillItemOptional kFiOptionalMac = kP2OptionalFillItemOptional(kP2SecurityResponseVerifyNameOptional, PfillItemOffsetFix, kP2SecurityResponseVerifyOptionalOffset, optional_mac);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiOptionalMac), &kFiOptionalMac));

	if (NULL != fill_mac)
	{
		const uint8_t choice = kP2SecurityResponseVerifyChoiceMac;
		const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2SecurityResponseVerifyNameChoice, PfillItemOffsetFix, kP2SecurityResponseVerifyChoiceOffset, choice);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

		// 注：fill_mac是fill_octet_string类型，type_exist为false
		const PfillItem kFiMac = PFILL_ITEM(kP2SecurityResponseVerifyNameMac, PfillItemOffsetFix, PfillItemProcessBySub, kP2SecurityResponseVerifyMacOffset, fill_mac);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFiMac), &kFiMac));
	}

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
cp_t P2SecurityResponseVerifyTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SecurityResponseVerifyTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

