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

20230811-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的FactoryVersion
*/
#include <string.h>

#include "p2_factory_version.h"
#include "p2_factory_version.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2FactoryVersionCutNum)


// {{{ factory_id
#define LenFactoryId		PcutItemLenBySub
static int OffsetFactoryId(Pcut *cut, int ix, const char *whole) { return kP2FactoryVersionFactoryIdOffset; }
#define ValidFactoryId	PcutItemValidBySub
//}}}


// {{{ software_version
#define LenSoftwareVersion		PcutItemLenBySub
#define OffsetSoftwareVersion	PcutItemOffsetDef
#define ValidSoftwareVersion	PcutItemValidBySub
//}}}


// {{{ software_date
#define LenSoftwareDate		PcutItemLenBySub
#define OffsetSoftwareDate	PcutItemOffsetDef
#define ValidSoftwareDate	PcutItemValidBySub
//}}}


// {{{ hardware_version
#define LenHardwareVersion		PcutItemLenBySub
#define OffsetHardwareVersion	PcutItemOffsetDef
#define ValidHardwareVersion	PcutItemValidBySub
//}}}


// {{{ hardware_date
#define LenHardwareDate		PcutItemLenBySub
#define OffsetHardwareDate	PcutItemOffsetDef
#define ValidHardwareDate	PcutItemValidBySub
//}}}


// {{{ factory_extend
#define LenFactoryExtend		PcutItemLenBySub
#define OffsetFactoryExtend		PcutItemOffsetDef
#define ValidFactoryExtend		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "factory_id", LenFactoryId, OffsetFactoryId, ValidFactoryId, NULL },
	{ "software_version", LenSoftwareVersion, OffsetSoftwareVersion, ValidSoftwareVersion, NULL },
	{ "software_date", LenSoftwareDate, OffsetSoftwareDate, ValidSoftwareDate, NULL },
	{ "hardware_version", LenHardwareVersion, OffsetHardwareVersion, ValidHardwareVersion, NULL },
	{ "hardware_date", LenHardwareDate, OffsetHardwareDate, ValidHardwareDate, NULL },
	{ "factory_extend", LenFactoryExtend, OffsetFactoryExtend, ValidFactoryExtend, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2FactoryVersionCutIxFactoryId]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2FactoryVersionCutIxSoftwareVersion]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2FactoryVersionCutIxSoftwareDate]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2FactoryVersionCutIxHardwareVersion]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2FactoryVersionCutIxHardwareDate]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2FactoryVersionCutIxFactoryExtend]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2FactoryVersionPcutOpen(P2FactoryVersionPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2VisibleStringPcutOpen(&m->factory_id));
	PcutSubSet(&m->base, kP2FactoryVersionCutIxFactoryId, &m->factory_id.base, NULL);

	ifer(P2VisibleStringPcutOpen(&m->software_version));
	PcutSubSet(&m->base, kP2FactoryVersionCutIxSoftwareVersion, &m->software_version.base, NULL);

	ifer(P2VisibleStringPcutOpen(&m->software_date));
	PcutSubSet(&m->base, kP2FactoryVersionCutIxSoftwareDate, &m->software_date.base, NULL);

	ifer(P2VisibleStringPcutOpen(&m->hardware_version));
	PcutSubSet(&m->base, kP2FactoryVersionCutIxHardwareVersion, &m->hardware_version.base, NULL);

	ifer(P2VisibleStringPcutOpen(&m->hardware_date));
	PcutSubSet(&m->base, kP2FactoryVersionCutIxHardwareDate, &m->hardware_date.base, NULL);

	ifer(P2VisibleStringPcutOpen(&m->factory_extend));
	PcutSubSet(&m->base, kP2FactoryVersionCutIxFactoryExtend, &m->factory_extend.base, NULL);
	return 0;
}
cp_t P2FactoryVersionPcutClose(P2FactoryVersionPcut *m)
{
	dve(P2FactoryVersionPcutValid(m));

	PcutSubSet(&m->base, kP2FactoryVersionCutIxFactoryId, NULL, NULL);
	ifer(P2VisibleStringPcutClose(&m->factory_id));

	PcutSubSet(&m->base, kP2FactoryVersionCutIxSoftwareVersion, NULL, NULL);
	ifer(P2VisibleStringPcutClose(&m->software_version));

	PcutSubSet(&m->base, kP2FactoryVersionCutIxSoftwareDate, NULL, NULL);
	ifer(P2VisibleStringPcutClose(&m->software_date));

	PcutSubSet(&m->base, kP2FactoryVersionCutIxHardwareVersion, NULL, NULL);
	ifer(P2VisibleStringPcutClose(&m->hardware_version));

	PcutSubSet(&m->base, kP2FactoryVersionCutIxHardwareDate, NULL, NULL);
	ifer(P2VisibleStringPcutClose(&m->hardware_date));

	PcutSubSet(&m->base, kP2FactoryVersionCutIxFactoryExtend, NULL, NULL);
	ifer(P2VisibleStringPcutClose(&m->factory_extend));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2FactoryVersionPcutValid(const P2FactoryVersionPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2VisibleStringPcutValid(&m->factory_id));
	ifer(P2VisibleStringPcutValid(&m->software_version));
	ifer(P2VisibleStringPcutValid(&m->software_date));
	ifer(P2VisibleStringPcutValid(&m->hardware_version));
	ifer(P2VisibleStringPcutValid(&m->hardware_date));
	ifer(P2VisibleStringPcutValid(&m->factory_extend));
	return 0;
}


cp_t P2FactoryVersionPcutOpenBase(Pcut *base)
{
	P2FactoryVersionPcut *m = (P2FactoryVersionPcut*)base;
	return P2FactoryVersionPcutOpen(m);
}
cp_t P2FactoryVersionPcutCloseBase(Pcut *base)
{
	P2FactoryVersionPcut *m = (P2FactoryVersionPcut*)base;
	return P2FactoryVersionPcutClose(m);
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
cp_t P2FactoryVersionTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2FactoryVersionTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


