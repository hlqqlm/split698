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

20230407-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 security response data choice 变化部分报文解析
*/

// var
#include "p2_octet_string.h"
#include "p2_dar.h"

#include "p2_security_response_data_choice.h"
#include "p2_security_response_data_choice.xcp.h"
//#define this_file_id	0x5a988a48	// echo -n dlt698_45_security_response_data_choice.c | rhash --simple -


#define TEST_EN				(0)


// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2SecurityResponseDataChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2SecurityResponseDataChoicePlain, kP2SecurityResponseDataNamePlain },	// 明文
	{ kP2SecurityResponseDataChoiceCipher, kP2SecurityResponseDataNameCipher },	// 密文
	{ kP2SecurityResponseDataChoiceDar, kP2SecurityResponseDataNameDar },	// 密文
};
int P2SecurityResponseDataChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2SecurityResponseDataChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2SecurityResponseDataChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2SecurityResponseDataChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2OctetStringPcut kP2OctetStringPcutDefVar = kP2OctetStringPcutDef;
static const uint8_t kDar = kP2DarOtherReason;
static const PcutFactoryInfo kVarFactoryInfoList[kP2SecurityResponseDataChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2SecurityResponseDataNamePlain, sizeof(P2OctetStringPcut), &kP2OctetStringPcutDefVar, P2OctetStringPcutOpenBase, P2OctetStringPcutCloseBase },	// 明文
	{ kP2SecurityResponseDataNameCipher, sizeof(P2OctetStringPcut), &kP2OctetStringPcutDefVar, P2OctetStringPcutOpenBase, P2OctetStringPcutCloseBase },	// 密文
	{ kP2SecurityResponseDataNameDar, kP2DarSize, &kDar, P2DarPcutOpenBase, P2DarPcutCloseBase },	// Dar
};
//}}}


//{{{ cut
cp_t P2SecurityResponseDataChoicePcutOpen(P2SecurityResponseDataChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2SecurityResponseDataChoiceName, 
			kChoiceList, kP2SecurityResponseDataChoiceNum, kVarFactoryInfoList);
}
cp_t P2SecurityResponseDataChoicePcutClose(P2SecurityResponseDataChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2SecurityResponseDataChoicePcutValid(const P2SecurityResponseDataChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}
//}}}


//{{{ fill
typedef struct 
{
	PfillItem base;
	uint8_t dar;
} FillItemDar;
static cp_t FillItemProcessDar(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemDar *derive = (FillItemDar*)fi;

	mem[offset] = derive->dar;
	*fill_size = kP2DarSize;
	return 0;
}
#define kFillItemDarDef(_dar) { PFILL_ITEM(kP2SecurityResponseDataNameDar, PfillItemOffsetFix, FillItemProcessDar, kP2SecurityResponseDataVarOffset, NULL), (_dar) }




// 模仿P2OctetStringFillInit()
cp_t P2SecurityResponseDataChoiceFillInit(Pfill *m, uint8_t choice, P2DarT dar, Pfill *fill_octet_string)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2SecurityResponseDataChoiceName, PfillItemOffsetFix, kP2SecurityResponseDataChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	if (kP2SecurityResponseDataChoiceDar == choice)
	{
		const FillItemDar kFiDar = kFillItemDarDef(dar);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFiDar), &kFiDar));
	}
	else
	{
		// 当choice=0/1
		ifbr(NULL != fill_octet_string);
		const char *kName = (kP2SecurityResponseDataChoicePlain == choice) ? kP2SecurityResponseDataNamePlain : kP2SecurityResponseDataNameCipher;
		const PfillItem kFiSub = PFILL_ITEM(kName, PfillItemOffsetFix, PfillItemProcessBySub, kP2SecurityResponseDataVarOffset, fill_octet_string);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFiSub), &kFiSub));
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
cp_t P2SecurityResponseDataChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SecurityResponseDataChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

