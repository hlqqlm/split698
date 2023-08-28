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

20230828-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的CompactProxySetThenGetRequest
*/
#include "p2_compact_proxy_set_then_get_request.h"
#include "p2_compact_proxy_set_then_get_request.xcp.h"


#define TEST_EN					(0)
#define kThisCutNum				(kP2CompactProxySetThenGetRequestCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetThenGetRequestPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetThenGetRequestPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ timeout
static int LenTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetThenGetRequestTimeoutSize; }
static int OffsetTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetThenGetRequestTimeoutOffset; }
static cp_t ValidTimeout(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ tsa
#define LenTsa		PcutItemLenBySub
static int OffsetTsa(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetThenGetRequestTsaOffset; }
#define ValidTsa		PcutItemValidBySub
//}}}


// {{{ sequence_of_oad_data_oad_unsigned
#define LenSequenceOf		PcutItemLenBySub
#define OffsetSequenceOf	PcutItemOffsetDef
#define ValidSequenceOf		PcutItemValidBySub
//}}}



//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2CompactProxySetThenGetRequestNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2CompactProxySetThenGetRequestNameTimeout, LenTimeout, OffsetTimeout, ValidTimeout, NULL },
	{ kP2CompactProxySetThenGetRequestNameTsa, LenTsa, OffsetTsa, ValidTsa, NULL },
	{ kP2CompactProxySetThenGetRequestNameSequenceOf, LenSequenceOf, OffsetSequenceOf, ValidSequenceOf, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetThenGetRequestCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetThenGetRequestCutIxTimeout]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetThenGetRequestCutIxTsa]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetThenGetRequestCutIxSequenceOf]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxySetThenGetRequestPcutOpen(P2CompactProxySetThenGetRequestPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2TsaPcutOpen(&m->tsa));
	PcutSubSet(&m->base, kP2CompactProxySetThenGetRequestCutIxTsa, &m->tsa.base, NULL);

	ifer(P2OadDataOadUnsignedPcutOpen(&m->oad_data_oad_unsigned_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_cut, &m->oad_data_oad_unsigned_cut.base, kP2CompactProxySetThenGetRequestNameSequenceOf));
	PcutSubSet(&m->base, kP2CompactProxySetThenGetRequestCutIxSequenceOf, &m->sequence_of_cut.base, NULL);
	return 0;
}
cp_t P2CompactProxySetThenGetRequestPcutClose(P2CompactProxySetThenGetRequestPcut *m)
{
	dve(P2CompactProxySetThenGetRequestPcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxySetThenGetRequestCutIxTsa, NULL, NULL);
	ifer(P2TsaPcutClose(&m->tsa));

	PcutSubSet(&m->base, kP2CompactProxySetThenGetRequestCutIxSequenceOf, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_cut));
	ifer(P2OadDataOadUnsignedPcutClose(&m->oad_data_oad_unsigned_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxySetThenGetRequestPcutValid(const P2CompactProxySetThenGetRequestPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2TsaPcutValid(&m->tsa));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_cut));
	ifer(P2OadDataOadUnsignedPcutValid(&m->oad_data_oad_unsigned_cut));
	return 0;
}


cp_t P2CompactProxySetThenGetRequestPcutOpenBase(Pcut *base)
{
	P2CompactProxySetThenGetRequestPcut *m = (P2CompactProxySetThenGetRequestPcut*)base;
	return P2CompactProxySetThenGetRequestPcutOpen(m);
}
cp_t P2CompactProxySetThenGetRequestPcutCloseBase(Pcut *base)
{
	P2CompactProxySetThenGetRequestPcut *m = (P2CompactProxySetThenGetRequestPcut*)base;
	return P2CompactProxySetThenGetRequestPcutClose(m);
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
cp_t P2CompactProxySetThenGetRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxySetThenGetRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
