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

20230711-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的LinkResponse
*/
#include <string.h>

#include "p2_datetime.h"

#include "p2_link_response.h"
#include "p2_link_response.xcp.h"
//#define this_file_id	0x08d887d6	// echo -n dlt698_45_get_response.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2LinkResponseCutNum)



// {{{ piid_acd
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2LinkResponsePiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2LinkResponsePiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


// {{{ result
const char *P2LinkResponseResultBit7Str(uint8_t bit7)
{
	return (0 == bit7) ? "unbelieve_time" : "believe_time";
}
const char *P2LinkResponseResultBit0To2Str(uint8_t bit0_2)
{
	if (0 == bit0_2)
		return "ok";
	if (1 == bit0_2)
		return "addr_repeat";
	if (2 == bit0_2)
		return "illegal_equipment";
	if (3 == bit0_2)
		return "insufficient_storage";
	return "keep";
}
uint8_t ResultBit7(uint8_t result)
{
	return (result & kP2LinkResponseResultBit7Mask) >> 7;
}
uint8_t ResultBit0To2(uint8_t result)
{
	return result & kP2LinkResponseResultBit0To2Mask;
}
uint8_t P2LinkResponseResult(const char *whole)
{
	return whole[kP2LinkResponseResultOffset];
}


static int LenResult(Pcut *cut, int ix, const char *whole) { return kP2LinkResponseResultSize; }
static int OffsetResult(Pcut *cut, int ix, const char *whole) { return kP2LinkResponseResultOffset; }
static cp_t ValidResult(Pcut *cut, int ix, const char *whole) { return 0; }
static cp_t ExplainResult(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t result = P2LinkResponseResult(whole);
	const uint8_t bit7 = ResultBit7(result);
	const uint8_t bit0_2 = ResultBit0To2(result);

	qos_printf("bit7=%u(%s) bit0_2=%u(%s)", bit7, P2LinkResponseResultBit7Str(bit7), bit0_2, P2LinkResponseResultBit0To2Str(bit0_2));
	return 0;
}
//}}}


// {{{ request_datetime
static int LenRequestDatetime(Pcut *cut, int ix, const char *whole) { return kP2LinkResponseRequestDatetimeSize; }
static int OffsetRequestDatetime(Pcut *cut, int ix, const char *whole) { return kP2LinkResponseRequestDatetimeOffset; }
static cp_t ValidRequestDatetime(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = whole + kP2LinkResponseRequestDatetimeOffset;
	ifer(Dlt698DatetimeValid(mem));
	return 0; 
}
static cp_t ExplainRequestDatetime(Pcut *cut, int ix, const char *whole) 
{
	const char *mem = whole + kP2LinkResponseRequestDatetimeOffset;
	return P2DatetimeExplain(mem);
}
//}}}


// {{{ receive_datetime
static int LenReceiveDatetime(Pcut *cut, int ix, const char *whole) { return kP2LinkResponseReceiveDatetimeSize; }
static int OffsetReceiveDatetime(Pcut *cut, int ix, const char *whole) { return kP2LinkResponseReceiveDatetimeOffset; }
static cp_t ValidReceiveDatetime(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = whole + kP2LinkResponseReceiveDatetimeOffset;
	ifer(Dlt698DatetimeValid(mem));
	return 0; 
}
static cp_t ExplainReceiveDatetime(Pcut *cut, int ix, const char *whole) 
{
	const char *mem = whole + kP2LinkResponseReceiveDatetimeOffset;
	return P2DatetimeExplain(mem);
}
//}}}


// {{{ response_datetime
static int LenResponseDatetime(Pcut *cut, int ix, const char *whole) { return kP2LinkResponseResponseDatetimeSize; }
static int OffsetResponseDatetime(Pcut *cut, int ix, const char *whole) { return kP2LinkResponseResponseDatetimeOffset; }
static cp_t ValidResponseDatetime(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = whole + kP2LinkResponseResponseDatetimeOffset;
	ifer(Dlt698DatetimeValid(mem));
	return 0; 
}
static cp_t ExplainResponseDatetime(Pcut *cut, int ix, const char *whole) 
{
	const char *mem = whole + kP2LinkResponseResponseDatetimeOffset;
	return P2DatetimeExplain(mem);
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2LinkResponseNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2LinkResponseNameResult, LenResult, OffsetResult, ValidResult, ExplainResult },
	{ kP2LinkResponseNameRequestDatetime, LenRequestDatetime, OffsetRequestDatetime, ValidRequestDatetime, ExplainRequestDatetime },
	{ kP2LinkResponseNameReceiveDatetime, LenReceiveDatetime, OffsetReceiveDatetime, ValidReceiveDatetime, ExplainReceiveDatetime },
	{ kP2LinkResponseNameResponseDatetime, LenResponseDatetime, OffsetResponseDatetime, ValidResponseDatetime, ExplainResponseDatetime },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2LinkResponseCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2LinkResponseCutIxResult]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2LinkResponseCutIxRequestDatetime]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2LinkResponseCutIxReceiveDatetime]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2LinkResponseCutIxResponseDatetime]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2LinkResponsePcutOpen(P2LinkResponsePcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2LinkResponsePcutClose(P2LinkResponsePcut *m)
{
	dve(P2LinkResponsePcutValid(m));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2LinkResponsePcutValid(const P2LinkResponsePcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2LinkResponsePcutOpenBase(Pcut *base)
{
	P2LinkResponsePcut *m = (P2LinkResponsePcut*)base;
	return P2LinkResponsePcutOpen(m);
}
cp_t P2LinkResponsePcutCloseBase(Pcut *base)
{
	P2LinkResponsePcut *m = (P2LinkResponsePcut*)base;
	return P2LinkResponsePcutClose(m);
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
cp_t P2LinkResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2LinkResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


