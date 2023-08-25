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

20230825-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的compact set reponse normal
*/

// for test

#include "p2_compact_set_response_normal.h"
#include "p2_compact_set_response_normal.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2CompactSetResponseNormalCutNum)


// {{{ piid_acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactSetResponseNormalPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactSetResponseNormalPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}



//{{{ dar
#define LenDar		PcutItemLenBySub
static int OffsetDar(Pcut *cut, int ix, const char *whole) { return kP2CompactSetResponseNormalDarOffset; }
#define ValidDar	PcutItemValidBySub
//}}}
 

//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2CompactSetResponseNormalNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2CompactSetResponseNormalNameDar, LenDar, OffsetDar, ValidDar, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactSetResponseNormalCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactSetResponseNormalCutIxDar]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactSetResponseNormalPcutOpen(P2CompactSetResponseNormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DarPcutOpen(&m->dar_cut));
	PcutSubSet(&m->base, kP2CompactSetResponseNormalCutIxDar, &m->dar_cut.base, kP2CompactSetResponseNormalNameDar);
	return 0;
}
cp_t P2CompactSetResponseNormalPcutClose(P2CompactSetResponseNormalPcut *m)
{
	dve(P2CompactSetResponseNormalPcutValid(m));

	PcutSubSet(&m->base, kP2CompactSetResponseNormalCutIxDar, NULL, NULL);
	ifer(P2DarPcutClose(&m->dar_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactSetResponseNormalPcutValid(const P2CompactSetResponseNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DarPcutValid(&m->dar_cut));
	return 0;
}


cp_t P2CompactSetResponseNormalPcutOpenBase(Pcut *base)
{
	P2CompactSetResponseNormalPcut *m = (P2CompactSetResponseNormalPcut*)base;
	return P2CompactSetResponseNormalPcutOpen(m);
}
cp_t P2CompactSetResponseNormalPcutCloseBase(Pcut *base)
{
	P2CompactSetResponseNormalPcut *m = (P2CompactSetResponseNormalPcut*)base;
	return P2CompactSetResponseNormalPcutClose(m);
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
cp_t P2CompactSetResponseNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactSetResponseNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

