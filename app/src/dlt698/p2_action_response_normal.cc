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

20230817-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的action response normal
*/

// for test

#include "p2_action_response_normal.h"
#include "p2_action_response_normal.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2ActionResponseNormalCutNum)


// {{{ piid_acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ActionResponseNormalPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ActionResponseNormalPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


// {{{ oad
static int LenOmd(Pcut *cut, int ix, const char *whole) { return kP2ActionResponseNormalOmdSize; }
static int OffsetOmd(Pcut *cut, int ix, const char *whole) { return kP2ActionResponseNormalOmdOffset; }
static cp_t ValidOmd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ dar
#define LenDar		PcutItemLenBySub
static int OffsetDar(Pcut *cut, int ix, const char *whole) { return kP2ActionResponseNormalDarOffset; }
#define ValidDar	PcutItemValidBySub
//}}}
 

// {{{ data_optional
#define LenOptionalData		PcutItemLenBySub
static int OffsetOptionalData(Pcut *cut, int ix, const char *whole) { return kP2ActionResponseNormalOptionalDataOffset; }
//#define OffsetOptionalFollowReport	PcutItemOffsetDef
#define ValidOptionalData	PcutItemValidBySub
// 用sub来填写data
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ActionResponseNormalNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2ActionResponseNormalNameOmd, LenOmd, OffsetOmd, ValidOmd, NULL },
	{ kP2ActionResponseNormalNameDar, LenDar, OffsetDar, ValidDar, NULL },
	{ kP2ActionResponseNormalNameOptionalData, LenOptionalData, OffsetOptionalData, ValidOptionalData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionResponseNormalCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionResponseNormalCutIxOmd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionResponseNormalCutIxDar]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionResponseNormalCutIxOptionalData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ActionResponseNormalPcutOpen(P2ActionResponseNormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DarPcutOpen(&m->dar_cut));
	PcutSubSet(&m->base, kP2ActionResponseNormalCutIxDar, &m->dar_cut.base, kP2ActionResponseNormalNameDar);

	ifer(P2DataChoicePcutOpen(&m->data_cut));
	ifer(P2OptionalPcutOpen(&m->optional_data_cut, &m->data_cut.choice.base, kP2ActionResponseNormalNameOptionalData));
	PcutSubSet(&m->base, kP2ActionResponseNormalCutIxOptionalData, &m->optional_data_cut.base, NULL);
	return 0;
}
cp_t P2ActionResponseNormalPcutClose(P2ActionResponseNormalPcut *m)
{
	dve(P2ActionResponseNormalPcutValid(m));

	PcutSubSet(&m->base, kP2ActionResponseNormalCutIxDar, NULL, NULL);
	ifer(P2DarPcutClose(&m->dar_cut));

	PcutSubSet(&m->base, kP2ActionResponseNormalCutIxOptionalData, NULL, NULL);
	ifer(P2OptionalPcutClose(&m->optional_data_cut));
	ifer(P2DataChoicePcutClose(&m->data_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ActionResponseNormalPcutValid(const P2ActionResponseNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DarPcutValid(&m->dar_cut));
	ifer(P2OptionalPcutValid(&m->optional_data_cut));
	ifer(P2DataChoicePcutValid(&m->data_cut));
	return 0;
}


cp_t P2ActionResponseNormalPcutOpenBase(Pcut *base)
{
	P2ActionResponseNormalPcut *m = (P2ActionResponseNormalPcut*)base;
	return P2ActionResponseNormalPcutOpen(m);
}
cp_t P2ActionResponseNormalPcutCloseBase(Pcut *base)
{
	P2ActionResponseNormalPcut *m = (P2ActionResponseNormalPcut*)base;
	return P2ActionResponseNormalPcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0

static cp_t TestPfill(void)
{
	return 0;
}

static const QtestItem kTestItem[] = {
	TestPfill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ActionResponseNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ActionResponseNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

