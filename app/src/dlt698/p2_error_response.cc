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

用来描述dlt698_45中的ErrorResponse
*/
#include <string.h>

#include "p2_error_response.h"
#include "p2_error_response.xcp.h"
//#define this_file_id	0x08d887d6	// echo -n dlt698_45_get_request.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ErrorResponseCutNum)

// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2ErrorResponsePiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2ErrorResponsePiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ type
// 必须按大小顺序排
#define kEnumNum kP2ErrorResponseTypeEnumNum
static const P2Enum kEnumList[kEnumNum] = {
	// enumv	name
	{ kP2ErrorResponseTypeEnumApdu, "apdu" },		// APDU 无法解析 （1），
	{ kP2ErrorResponseTypeEnumUnsupported, "unsupported" },	// 服务不支持 （2），
	{ kP2ErrorResponseTypeEnumOther, "other" },	// 其他 （255）
};
int P2ErrorResponseTypeEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kEnumList, kEnumNum, enumv);
}
uint8_t P2ErrorResponseTypeEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kEnumList, kEnumNum, enumv_ix);
}
cp_t P2ErrorResponseTypeEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kEnumNum, enumv_ix);
}
cp_t P2ErrorResponseTypeEnumValid(uint8_t enumv)
{
	return P2EnumValid(kEnumList, kEnumNum, enumv);
}
const char *P2ErrorResponseTypeEnumStr(uint8_t enumv)
{
	return P2EnumStr(kEnumList, kEnumNum, enumv);
}



uint8_t P2ErrorResponseTypeValue(const char *whole)
{
	return whole[kP2ErrorResponseTypeOffset];
}
const char *P2ErrorResponseTypeMem(const char *whole)
{
	return whole + kP2ErrorResponseTypeOffset;
}

/*
static int LenType(Pcut *cut, int ix, const char *whole) { return kP2ErrorResponseTypeSize; }
static int OffsetType(Pcut *cut, int ix, const char *whole) { return kP2ErrorResponseTypeOffset; }
static cp_t ValidType(Pcut *cut, int ix, const char *whole) 
{ 
	const uint8_t type = P2ErrorResponseTypeValue(whole);
	return P2ErrorResponseTypeEnumValid(type); 
}
static cp_t ExplainType(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t type = P2ErrorResponseTypeValue(whole);
	qos_printf("%s", P2ErrorResponseTypeEnumStr(type));
	return 0;
}
*/

#define LenType		PcutItemLenBySub
static int OffsetType(Pcut *cut, int ix, const char *whole) { return kP2ErrorResponseTypeOffset; }
#define ValidType	PcutItemValidBySub
//}}}
 

//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ErrorResponseNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2ErrorResponseNameType, LenType, OffsetType, ValidType, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ErrorResponseCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ErrorResponseCutIxType]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ErrorResponsePcutOpen(P2ErrorResponsePcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	// 因为要执行enum do table，所以必须要用sub来解析enum
	ifer(P2EnumPcutOpen(&m->enum_type, kP2EnumName, kEnumList, kP2ErrorResponseTypeEnumNum));
	PcutSubSet(&m->base, kP2ErrorResponseCutIxType, &m->enum_type.base, kP2ErrorResponseNameType);
	return 0;
}
cp_t P2ErrorResponsePcutClose(P2ErrorResponsePcut *m)
{
	dve(P2ErrorResponsePcutValid(m));

	PcutSubSet(&m->base, kP2ErrorResponseCutIxType, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_type));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ErrorResponsePcutValid(const P2ErrorResponsePcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2EnumPcutValid(&m->enum_type));
	return 0;
}


cp_t P2ErrorResponsePcutOpenBase(Pcut *base)
{
	P2ErrorResponsePcut *m = (P2ErrorResponsePcut*)base;
	return P2ErrorResponsePcutOpen(m);
}
cp_t P2ErrorResponsePcutCloseBase(Pcut *base)
{
	P2ErrorResponsePcut *m = (P2ErrorResponsePcut*)base;
	return P2ErrorResponsePcutClose(m);
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
cp_t P2ErrorResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ErrorResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


