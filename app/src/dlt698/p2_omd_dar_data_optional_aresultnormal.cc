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

20230821-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的ActionThenGetResponseNormalList中sequence of单元
*/

// for test

#include "p2_omd_dar_data_optional_aresultnormal.h"
#include "p2_omd_dar_data_optional_aresultnormal.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2OmdDarDataOptionalAresultnormalCutNum)


// {{{ oad
static int LenOmd(Pcut *cut, int ix, const char *whole) { return kP2OmdDarDataOptionalAresultnormalOmdSize; }
static int OffsetOmd(Pcut *cut, int ix, const char *whole) { return kP2OmdDarDataOptionalAresultnormalOmdOffset; }
static cp_t ValidOmd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ dar
#define LenDar		PcutItemLenBySub
static int OffsetDar(Pcut *cut, int ix, const char *whole) { return kP2OmdDarDataOptionalAresultnormalDarOffset; }
#define ValidDar	PcutItemValidBySub
//}}}
 

// {{{ data_optional
#define LenDataOptional		PcutItemLenBySub
static int OffsetDataOptional(Pcut *cut, int ix, const char *whole) { return kP2OmdDarDataOptionalAresultnormalDataOptionalOffset; }
//#define OffsetOptionalFollowReport	PcutItemOffsetDef
#define ValidDataOptional	PcutItemValidBySub
// 用sub来填写data
//}}}


// {{{ a_result_normal
#define LenAresultnormal	PcutItemLenBySub
#define OffsetAresultnormal	PcutItemOffsetDef
#define ValidAresultnormal	PcutItemValidBySub
// 用sub来填写data
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2OmdDarDataOptionalAresultnormalNameOmd, LenOmd, OffsetOmd, ValidOmd, NULL },
	{ kP2OmdDarDataOptionalAresultnormalNameDar, LenDar, OffsetDar, ValidDar, NULL },
	{ kP2OmdDarDataOptionalAresultnormalNameDataOptional, LenDataOptional, OffsetDataOptional, ValidDataOptional, NULL },
	{ kP2OmdDarDataOptionalAresultnormalNameAresultnormal, LenAresultnormal, OffsetAresultnormal, ValidAresultnormal, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDarDataOptionalAresultnormalCutIxOmd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDarDataOptionalAresultnormalCutIxDar]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDarDataOptionalAresultnormalCutIxDataOptional]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDarDataOptionalAresultnormalCutIxAresultnormal]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2OmdDarDataOptionalAresultnormalPcutOpen(P2OmdDarDataOptionalAresultnormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DarPcutOpen(&m->dar_cut));
	PcutSubSet(&m->base, kP2OmdDarDataOptionalAresultnormalCutIxDar, &m->dar_cut.base, kP2OmdDarDataOptionalAresultnormalNameDar);

	ifer(P2DataChoicePcutOpen(&m->data_cut));
	ifer(P2OptionalPcutOpen(&m->data_optional_cut, &m->data_cut.choice.base, kP2OmdDarDataOptionalAresultnormalNameDataOptional));
	PcutSubSet(&m->base, kP2OmdDarDataOptionalAresultnormalCutIxDataOptional, &m->data_optional_cut.base, NULL);

	ifer(P2AResultNormalPcutOpen(&m->aresultnormal_cut));
	PcutSubSet(&m->base, kP2OmdDarDataOptionalAresultnormalCutIxAresultnormal, &m->aresultnormal_cut.base, kP2OmdDarDataOptionalAresultnormalNameAresultnormal);
	return 0;
}
cp_t P2OmdDarDataOptionalAresultnormalPcutClose(P2OmdDarDataOptionalAresultnormalPcut *m)
{
	dve(P2OmdDarDataOptionalAresultnormalPcutValid(m));

	PcutSubSet(&m->base, kP2OmdDarDataOptionalAresultnormalCutIxDar, NULL, NULL);
	ifer(P2DarPcutClose(&m->dar_cut));

	PcutSubSet(&m->base, kP2OmdDarDataOptionalAresultnormalCutIxDataOptional, NULL, NULL);
	ifer(P2OptionalPcutClose(&m->data_optional_cut));
	ifer(P2DataChoicePcutClose(&m->data_cut));

	PcutSubSet(&m->base, kP2OmdDarDataOptionalAresultnormalCutIxAresultnormal, NULL, NULL);
	ifer(P2AResultNormalPcutClose(&m->aresultnormal_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2OmdDarDataOptionalAresultnormalPcutValid(const P2OmdDarDataOptionalAresultnormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DarPcutValid(&m->dar_cut));
	ifer(P2OptionalPcutValid(&m->data_optional_cut));
	ifer(P2DataChoicePcutValid(&m->data_cut));
	ifer(P2AResultNormalPcutValid(&m->aresultnormal_cut));
	return 0;
}


cp_t P2OmdDarDataOptionalAresultnormalPcutOpenBase(Pcut *base)
{
	P2OmdDarDataOptionalAresultnormalPcut *m = (P2OmdDarDataOptionalAresultnormalPcut*)base;
	return P2OmdDarDataOptionalAresultnormalPcutOpen(m);
}
cp_t P2OmdDarDataOptionalAresultnormalPcutCloseBase(Pcut *base)
{
	P2OmdDarDataOptionalAresultnormalPcut *m = (P2OmdDarDataOptionalAresultnormalPcut*)base;
	return P2OmdDarDataOptionalAresultnormalPcutClose(m);
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
cp_t P2OmdDarDataOptionalAresultnormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2OmdDarDataOptionalAresultnormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

