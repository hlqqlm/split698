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

用来描述dlt698_45中的ReleaseResponse
*/
#include <string.h>

#include "p2_release_response.h"
#include "p2_release_response.xcp.h"


#define TEST_EN					(0)
#define kThisCutNum				(kP2ReleaseResponseCutNum)

// {{{ piid
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ReleaseResponsePiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ReleaseResponsePiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ type
// 必须按大小顺序排
#define kEnumNum kP2ReleaseResponseResultEnumNum
static const P2Enum kEnumList[kEnumNum] = {
	// enumv	name
	{ kP2ReleaseResponseResultEnumOk, "ok" },		// 成功（0）
};
int P2ReleaseResponseResultEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kEnumList, kEnumNum, enumv);
}
uint8_t P2ReleaseResponseResultEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kEnumList, kEnumNum, enumv_ix);
}
cp_t P2ReleaseResponseResultEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kEnumNum, enumv_ix);
}
cp_t P2ReleaseResponseResultEnumValid(uint8_t enumv)
{
	return P2EnumValid(kEnumList, kEnumNum, enumv);
}
const char *P2ReleaseResponseResultEnumStr(uint8_t enumv)
{
	return P2EnumStr(kEnumList, kEnumNum, enumv);
}



uint8_t P2ReleaseResponseResultValue(const char *whole)
{
	return whole[kP2ReleaseResponseResultOffset];
}
const char *P2ReleaseResponseResultMem(const char *whole)
{
	return whole + kP2ReleaseResponseResultOffset;
}


#define LenResult		PcutItemLenBySub
static int OffsetResult(Pcut *cut, int ix, const char *whole) { return kP2ReleaseResponseResultOffset; }
#define ValidResult	PcutItemValidBySub
//}}}
 

//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ReleaseResponseNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2ReleaseResponseNameResult, LenResult, OffsetResult, ValidResult, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ReleaseResponseCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ReleaseResponseCutIxResult]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ReleaseResponsePcutOpen(P2ReleaseResponsePcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	// 因为要执行enum do table，所以必须要用sub来解析enum
	ifer(P2EnumPcutOpen(&m->enum_result, kP2EnumName, kEnumList, kP2ReleaseResponseResultEnumNum));
	PcutSubSet(&m->base, kP2ReleaseResponseCutIxResult, &m->enum_result.base, kP2ReleaseResponseNameResult);
	return 0;
}
cp_t P2ReleaseResponsePcutClose(P2ReleaseResponsePcut *m)
{
	dve(P2ReleaseResponsePcutValid(m));

	PcutSubSet(&m->base, kP2ReleaseResponseCutIxResult, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_result));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ReleaseResponsePcutValid(const P2ReleaseResponsePcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2EnumPcutValid(&m->enum_result));
	return 0;
}


cp_t P2ReleaseResponsePcutOpenBase(Pcut *base)
{
	P2ReleaseResponsePcut *m = (P2ReleaseResponsePcut*)base;
	return P2ReleaseResponsePcutOpen(m);
}
cp_t P2ReleaseResponsePcutCloseBase(Pcut *base)
{
	P2ReleaseResponsePcut *m = (P2ReleaseResponsePcut*)base;
	return P2ReleaseResponsePcutClose(m);
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
cp_t P2ReleaseResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ReleaseResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


