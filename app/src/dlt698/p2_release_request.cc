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

20230823-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的ReleaseRequest
*/
#include <string.h>

#include "p2_release_request.h"
#include "p2_release_request.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ReleaseRequestCutNum)

 
// {{{ piid
static int LenPiid(Pcut *part, int ix, const char *whole) { return kP2ReleaseRequestPiidSize; }
static int OffsetPiid(Pcut *part, int ix, const char *whole) { return kP2ReleaseRequestPiidOffset; }
static cp_t ValidPiid(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}



//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kThisCutNum] = {
	// name len offset valid explain
	{ "piid", LenPiid, OffsetPiid, ValidPiid }, // kP2ReleaseRequestCutIxPiid,
};
	

static const PcutItem kPartItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ReleaseRequestCutIxPiid]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}



cp_t P2ReleaseRequestPcutOpen(P2ReleaseRequestPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2ReleaseRequestPcutClose(P2ReleaseRequestPcut *m)
{
	dve(P2ReleaseRequestPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ReleaseRequestPcutValid(const P2ReleaseRequestPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2ReleaseRequestPcutOpenBase(Pcut *base)
{
	P2ReleaseRequestPcut *m = (P2ReleaseRequestPcut*)base;
	return P2ReleaseRequestPcutOpen(m);
}
cp_t P2ReleaseRequestPcutCloseBase(Pcut *base)
{
	P2ReleaseRequestPcut *m = (P2ReleaseRequestPcut*)base;
	return P2ReleaseRequestPcutClose(m);
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
cp_t P2ReleaseRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ReleaseRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


