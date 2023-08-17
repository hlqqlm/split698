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
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的SetResponseNormal
*/

// for test

#include "p2_set_response_normal.h"
#include "p2_set_response_normal.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2SetResponseNormalCutNum)


// {{{ piid_acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2SetResponseNormalPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2SetResponseNormalPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2SetResponseNormalOadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2SetResponseNormalOadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ dar
#define LenDar		PcutItemLenBySub
static int OffsetDar(Pcut *cut, int ix, const char *whole) { return kP2SetResponseNormalDarOffset; }
#define ValidDar	PcutItemValidBySub
//}}}
 

//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2SetResponseNormalNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2SetResponseNormalNameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2SetResponseNormalNameDar, LenDar, OffsetDar, ValidDar, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetResponseNormalCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetResponseNormalCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetResponseNormalCutIxDar]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SetResponseNormalPcutOpen(P2SetResponseNormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DarPcutOpen(&m->dar_cut));
	PcutSubSet(&m->base, kP2SetResponseNormalCutIxDar, &m->dar_cut.base, kP2SetResponseNormalNameDar);
	return 0;
}
cp_t P2SetResponseNormalPcutClose(P2SetResponseNormalPcut *m)
{
	dve(P2SetResponseNormalPcutValid(m));

	PcutSubSet(&m->base, kP2SetResponseNormalCutIxDar, NULL, NULL);
	ifer(P2DarPcutClose(&m->dar_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2SetResponseNormalPcutValid(const P2SetResponseNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DarPcutValid(&m->dar_cut));
	return 0;
}


cp_t P2SetResponseNormalPcutOpenBase(Pcut *base)
{
	P2SetResponseNormalPcut *m = (P2SetResponseNormalPcut*)base;
	return P2SetResponseNormalPcutOpen(m);
}
cp_t P2SetResponseNormalPcutCloseBase(Pcut *base)
{
	P2SetResponseNormalPcut *m = (P2SetResponseNormalPcut*)base;
	return P2SetResponseNormalPcutClose(m);
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
cp_t P2SetResponseNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SetResponseNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

