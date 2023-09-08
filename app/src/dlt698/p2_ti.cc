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

20230908-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的TI
*/

#include "p2_long_unsigned.h"

#include "p2_ti.h"
#include "p2_ti.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2TiCutNum)


//{{{ unit
// 必须按大小顺序排
static const P2Enum kTiUnitEnumList[kP2TiUnitNum] = {
	// enumv	name
	{ kP2TiUnitSecond, "second" },	// 秒 （0），
	{ kP2TiUnitMinute, "minute" },	// 分 （1），
	{ kP2TiUnitHour, "hour" },		// 时 （2），
	{ kP2TiUnitDay, "day" },		// 日 （3），
	{ kP2TiUnitMonth, "month" },	// 月 （4），
	{ kP2TiUnitYear, "year" },		// 年 （5）
};
int P2TiUnitEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kTiUnitEnumList, kP2TiUnitNum, enumv);
}
uint8_t P2TiUnitEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kTiUnitEnumList, kP2TiUnitNum, enumv_ix);
}
cp_t P2TiUnitEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kP2TiUnitNum, enumv_ix);
}
cp_t P2TiUnitEnumValid(uint8_t enumv)
{
	return P2EnumValid(kTiUnitEnumList, kP2TiUnitNum, enumv);
}
const char *P2TiUnitEnumStr(uint8_t enumv)
{
	return P2EnumStr(kTiUnitEnumList, kP2TiUnitNum, enumv);
}



uint8_t P2TiUnitValue(const char *whole)
{
	return whole[kP2TiUnitOffset];
}
const char *P2TiUnitMem(const char *whole)
{
	return whole + kP2TiUnitOffset;
}

#define LenUnit		PcutItemLenBySub
//#define OffsetTiUnit	PcutItemOffsetDef
static int OffsetUnit(Pcut *cut, int ix, const char *whole) { return kP2TiUnitOffset; }
#define ValidUnit	PcutItemValidBySub
//}}}




// {{{ interval
static int LenInterval(Pcut *cut, int ix, const char *whole) { return kP2TiIntervalSize; }
static int OffsetInterval(Pcut *cut, int ix, const char *whole) { return kP2TiIntervalOffset; }
static cp_t ValidInterval(Pcut *cut, int ix, const char *whole) { return 0; }
static cp_t ExplainInterval(Pcut *cut, int ix, const char *whole) 
{
	P2LongUnsignedExplain(whole);
	return 0;
}
//}}}



//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2TiNameUnit, LenUnit, OffsetUnit, ValidUnit, NULL },
	{ kP2TiNameInterval, LenInterval, OffsetInterval, ValidInterval, ExplainInterval },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2TiCutIxUnit]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2TiCutIxInterval]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2TiPcutOpen(P2TiPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2EnumPcutOpen(&m->enum_unit, kP2TiNameUnit, kTiUnitEnumList, kP2TiUnitNum));
	PcutSubSet(&m->base, kP2TiCutIxUnit, &m->enum_unit.base, kP2TiNameUnit);
	return 0;
}
cp_t P2TiPcutClose(P2TiPcut *m)
{
	dve(P2TiPcutValid(m));

	PcutSubSet(&m->base, kP2TiCutIxUnit, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_unit));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2TiPcutValid(const P2TiPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2EnumPcutValid(&m->enum_unit));
	return 0;
}


cp_t P2TiPcutOpenBase(Pcut *base)
{
	P2TiPcut *m = (P2TiPcut*)base;
	return P2TiPcutOpen(m);
}
cp_t P2TiPcutCloseBase(Pcut *base)
{
	P2TiPcut *m = (P2TiPcut*)base;
	return P2TiPcutClose(m);
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
cp_t P2TiTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2TiTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

