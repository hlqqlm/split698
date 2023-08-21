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

用来描述dlt698_45中的ActionResponseNormalList中sequence of单元
*/

// for test

#include "p2_omd_dar_data_optional.h"
#include "p2_omd_dar_data_optional.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2OmdDarDataOptionalCutNum)


// {{{ oad
static int LenOmd(Pcut *cut, int ix, const char *whole) { return kP2OmdDarDataOptionalOmdSize; }
static int OffsetOmd(Pcut *cut, int ix, const char *whole) { return kP2OmdDarDataOptionalOmdOffset; }
static cp_t ValidOmd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ dar
#define LenDar		PcutItemLenBySub
static int OffsetDar(Pcut *cut, int ix, const char *whole) { return kP2OmdDarDataOptionalDarOffset; }
#define ValidDar	PcutItemValidBySub
//}}}
 

// {{{ data_optional
#define LenDataOptional		PcutItemLenBySub
static int OffsetDataOptional(Pcut *cut, int ix, const char *whole) { return kP2OmdDarDataOptionalDataOptionalOffset; }
//#define OffsetOptionalFollowReport	PcutItemOffsetDef
#define ValidDataOptional	PcutItemValidBySub
// 用sub来填写data
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2OmdDarDataOptionalNameOmd, LenOmd, OffsetOmd, ValidOmd, NULL },
	{ kP2OmdDarDataOptionalNameDar, LenDar, OffsetDar, ValidDar, NULL },
	{ kP2OmdDarDataOptionalNameDataOptional, LenDataOptional, OffsetDataOptional, ValidDataOptional, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDarDataOptionalCutIxOmd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDarDataOptionalCutIxDar]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDarDataOptionalCutIxDataOptional]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2OmdDarDataOptionalPcutOpen(P2OmdDarDataOptionalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DarPcutOpen(&m->dar_cut));
	PcutSubSet(&m->base, kP2OmdDarDataOptionalCutIxDar, &m->dar_cut.base, kP2OmdDarDataOptionalNameDar);

	ifer(P2DataChoicePcutOpen(&m->data_cut));
	ifer(P2OptionalPcutOpen(&m->data_optional_cut, &m->data_cut.choice.base, kP2OmdDarDataOptionalNameDataOptional));
	PcutSubSet(&m->base, kP2OmdDarDataOptionalCutIxDataOptional, &m->data_optional_cut.base, NULL);
	return 0;
}
cp_t P2OmdDarDataOptionalPcutClose(P2OmdDarDataOptionalPcut *m)
{
	dve(P2OmdDarDataOptionalPcutValid(m));

	PcutSubSet(&m->base, kP2OmdDarDataOptionalCutIxDar, NULL, NULL);
	ifer(P2DarPcutClose(&m->dar_cut));

	PcutSubSet(&m->base, kP2OmdDarDataOptionalCutIxDataOptional, NULL, NULL);
	ifer(P2OptionalPcutClose(&m->data_optional_cut));
	ifer(P2DataChoicePcutClose(&m->data_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2OmdDarDataOptionalPcutValid(const P2OmdDarDataOptionalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DarPcutValid(&m->dar_cut));
	ifer(P2OptionalPcutValid(&m->data_optional_cut));
	ifer(P2DataChoicePcutValid(&m->data_cut));
	return 0;
}


cp_t P2OmdDarDataOptionalPcutOpenBase(Pcut *base)
{
	P2OmdDarDataOptionalPcut *m = (P2OmdDarDataOptionalPcut*)base;
	return P2OmdDarDataOptionalPcutOpen(m);
}
cp_t P2OmdDarDataOptionalPcutCloseBase(Pcut *base)
{
	P2OmdDarDataOptionalPcut *m = (P2OmdDarDataOptionalPcut*)base;
	return P2OmdDarDataOptionalPcutClose(m);
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
cp_t P2OmdDarDataOptionalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2OmdDarDataOptionalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

