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

用来描述dlt698_45中的LinkRequest
*/
#include <string.h>

#include "p2_datetime.h"

#include "p2_link_request.h"
#include "p2_link_request.xcp.h"
//#define this_file_id	0x08d887d6	// echo -n dlt698_45_get_request.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)


// {{{ piid_acd
//#define LenPiidAcd	PcutItemLenBySub	
//static int OffsetData(Pcut *cut, int ix, const char *whole) { return kP2SecurityRequestDataOffset; }
//#define ValidData	PcutItemValidBySub

static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2LinkRequestPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2LinkRequestPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ type
// 必须按大小顺序排
#define kEnumNum kP2LinkRequestTypeEnumNum
static const P2Enum kEnumList[kEnumNum] = {
	// enumv	name
	{ kP2LinkRequestTypeEnumLogin, "login" },		// 登录 （0），
	{ kP2LinkRequestTypeEnumHeartbeat, "heartbeat" },	// 心跳 （1），
	{ kP2LinkRequestTypeEnumHeartLogout, "logout" },	// 退出登录 （2）
};
int P2LinkRequestTypeEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kEnumList, kEnumNum, enumv);
}
uint8_t P2LinkRequestTypeEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kEnumList, kEnumNum, enumv_ix);
}
cp_t P2LinkRequestTypeEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kEnumNum, enumv_ix);
}
cp_t P2LinkRequestTypeEnumValid(uint8_t enumv)
{
	return P2EnumValid(kEnumList, kEnumNum, enumv);
}
const char *P2LinkRequestTypeEnumStr(uint8_t enumv)
{
	return P2EnumStr(kEnumList, kEnumNum, enumv);
}



uint8_t P2LinkRequestTypeValue(const char *whole)
{
	return whole[kP2LinkRequestTypeOffset];
}
const char *P2LinkRequestTypeMem(const char *whole)
{
	return whole + kP2LinkRequestTypeOffset;
}


static int LenType(Pcut *cut, int ix, const char *whole) { return kP2LinkRequestTypeSize; }
static int OffsetType(Pcut *cut, int ix, const char *whole) { return kP2LinkRequestTypeOffset; }
static cp_t ValidType(Pcut *cut, int ix, const char *whole) 
{ 
	const uint8_t type = P2LinkRequestTypeValue(whole);
	return P2LinkRequestTypeEnumValid(type); 
}
static cp_t ExplainType(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t type = P2LinkRequestTypeValue(whole);
	qos_printf("%s", P2LinkRequestTypeEnumStr(type));
	return 0;
}
//}}}
 

// {{{ heartbeat
uint16_t P2LinkRequestHeartbeatIntervalValue(const char *whole) 
{
	const char *mem = whole + kP2LinkRequestHeartbeatIntervalOffset;
	return Dlt698Uint16(mem);
}
static int LenHeartbeatInterval(Pcut *cut, int ix, const char *whole) { return kP2LinkRequestHeartbeatIntervalSize; }
static int OffsetHeartbeatInterval(Pcut *cut, int ix, const char *whole) { return kP2LinkRequestHeartbeatIntervalOffset; }
static cp_t ValidHeartbeatInterval(Pcut *cut, int ix, const char *whole) { return 0; }
static cp_t ExplainHeartbeatInterval(Pcut *cut, int ix, const char *whole) 
{
	const uint16_t value = P2LinkRequestHeartbeatIntervalValue(whole);
	qos_printf("%uD", value);
	return 0;
}
//}}}


// {{{ datetime
static int LenDatetime(Pcut *cut, int ix, const char *whole) { return kP2LinkRequestDatetimeSize; }
static int OffsetDatetime(Pcut *cut, int ix, const char *whole) { return kP2LinkRequestDatetimeOffset; }
static cp_t ValidDatetime(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = whole + kP2LinkRequestDatetimeOffset;
	ifer(Dlt698DatetimeValid(mem));
	return 0; 
}
static cp_t ExplainDatetime(Pcut *cut, int ix, const char *whole) 
{
	const char *mem = whole + kP2LinkRequestDatetimeOffset;
	return P2DatetimeExplain(mem);
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2LinkRequestPartNum] = {
	// name len offset valid
	{ kP2LinkRequestNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd },
	{ kP2LinkRequestNameType, LenType, OffsetType, ValidType, ExplainType },
	{ kP2LinkRequestNameHeartbeatInterval, LenHeartbeatInterval, OffsetHeartbeatInterval, ValidHeartbeatInterval, ExplainHeartbeatInterval },
	{ kP2LinkRequestNameDatetime, LenDatetime, OffsetDatetime, ValidDatetime, ExplainDatetime },
};
	

static const PcutItem kPartItemsPattern[kP2LinkRequestPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2LinkRequestCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2LinkRequestCutIxType]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2LinkRequestCutIxHeartbeatInterval]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2LinkRequestCutIxDatetime]),
};
static void PcutItemsInit(PcutItem items[kP2LinkRequestPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}

cp_t P2LinkRequestPcutOpen(P2LinkRequestPcut *m)
{
	ifer(P2EnumPcutOpen(&m->enum_type, kP2EnumName, kEnumList, kP2LinkRequestTypeEnumNum));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2LinkRequestPartNum));

	PcutSubSet(&m->base, kP2LinkRequestCutIxType, &m->enum_type.base, kP2LinkRequestNameType);
	return 0;
}
cp_t P2LinkRequestPcutClose(P2LinkRequestPcut *m)
{
	dve(P2LinkRequestPcutValid(m));

	PcutSubSet(&m->base, kP2LinkRequestCutIxType, NULL, NULL);

	ifer(PcutClose(&m->base));

	ifer(P2EnumPcutClose(&m->enum_type));
	return 0;
}
cp_t P2LinkRequestPcutValid(const P2LinkRequestPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2EnumPcutValid(&m->enum_type));
	return 0;
}


cp_t P2LinkRequestPcutOpenBase(Pcut *base)
{
	P2LinkRequestPcut *m = (P2LinkRequestPcut*)base;
	return P2LinkRequestPcutOpen(m);
}
cp_t P2LinkRequestPcutCloseBase(Pcut *base)
{
	P2LinkRequestPcut *m = (P2LinkRequestPcut*)base;
	return P2LinkRequestPcutClose(m);
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
cp_t P2LinkRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2LinkRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


