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

用来描述dlt698_45中的ReleaseNotification
*/
#include <string.h>

#include "p2_release_notification.h"
#include "p2_release_notification.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ReleaseNotificationCutNum)

 
// {{{ piid-acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ReleaseNotificationPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ReleaseNotificationPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ link_datetimes
#define LenLinkDatetimes			PcutItemLenBySub	
static int OffsetLinkDatetimes(Pcut *cut, int ix, const char *whole) { return kP2ReleaseNotificationLinkDatetimesOffset; }
#define ValidLinkDatetimes		PcutItemValidBySub
//}}}


// {{{ server_datetimes
#define LenServerDatetimes			PcutItemLenBySub	
static int OffsetServerDatetimes(Pcut *cut, int ix, const char *whole) { return kP2ReleaseNotificationServerDatetimesOffset; }
#define ValidServerDatetimes		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ReleaseNotificationNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd }, 
	{ kP2ReleaseNotificationNameLinkDatetimes, LenLinkDatetimes, OffsetLinkDatetimes, ValidLinkDatetimes }, 
	{ kP2ReleaseNotificationNameServerDatetimes, LenServerDatetimes, OffsetServerDatetimes, ValidServerDatetimes }, 
};
	

static const PcutItem kPartItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ReleaseNotificationCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ReleaseNotificationCutIxLinkDatetimes]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ReleaseNotificationCutIxServerDatetimes]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}



cp_t P2ReleaseNotificationPcutOpen(P2ReleaseNotificationPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DatetimesPcutOpen(&m->link_datetimes));
	PcutSubSet(&m->base, kP2ReleaseNotificationCutIxLinkDatetimes, &m->link_datetimes.base, NULL);

	ifer(P2DatetimesPcutOpen(&m->server_datetimes));
	PcutSubSet(&m->base, kP2ReleaseNotificationCutIxServerDatetimes, &m->server_datetimes.base, NULL);
	return 0;
}
cp_t P2ReleaseNotificationPcutClose(P2ReleaseNotificationPcut *m)
{
	dve(P2ReleaseNotificationPcutValid(m));

	PcutSubSet(&m->base, kP2ReleaseNotificationCutIxLinkDatetimes, NULL, NULL);
	ifer(P2DatetimesPcutClose(&m->link_datetimes));

	PcutSubSet(&m->base, kP2ReleaseNotificationCutIxServerDatetimes, NULL, NULL);
	ifer(P2DatetimesPcutClose(&m->server_datetimes));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ReleaseNotificationPcutValid(const P2ReleaseNotificationPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DatetimesPcutValid(&m->link_datetimes));
	ifer(P2DatetimesPcutValid(&m->server_datetimes));
	return 0;
}


cp_t P2ReleaseNotificationPcutOpenBase(Pcut *base)
{
	P2ReleaseNotificationPcut *m = (P2ReleaseNotificationPcut*)base;
	return P2ReleaseNotificationPcutOpen(m);
}
cp_t P2ReleaseNotificationPcutCloseBase(Pcut *base)
{
	P2ReleaseNotificationPcut *m = (P2ReleaseNotificationPcut*)base;
	return P2ReleaseNotificationPcutClose(m);
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
cp_t P2ReleaseNotificationTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ReleaseNotificationTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


