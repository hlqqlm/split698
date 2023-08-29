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

用来描述dlt698_45中的
*/
#include "p2_compact_proxy_action_then_get_request.h"
#include "p2_compact_proxy_action_then_get_request.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2CompactProxyActionThenGetRequestCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionThenGetRequestPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionThenGetRequestPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ timeout
static int LenTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionThenGetRequestTimeoutSize; }
static int OffsetTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionThenGetRequestTimeoutOffset; }
static cp_t ValidTimeout(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ tsa
#define LenTsa		PcutItemLenBySub
static int OffsetTsa(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionThenGetRequestTsaOffset; }
#define ValidTsa		PcutItemValidBySub
//}}}


// {{{ sequence_of_omd_data_oad_unsigned
#define LenSequenceOfOmdDataOadUnsigned			PcutItemLenBySub
//static int OffsetSequenceOfOmdDataOadUnsigned(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionThenGetRequestSequenceOfOmdDataOadUnsignedOffset; }
#define OffsetSequenceOfOmdDataOadUnsigned		PcutItemOffsetDef
#define ValidSequenceOfOmdDataOadUnsigned		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2CompactProxyActionThenGetRequestNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2CompactProxyActionThenGetRequestNameTimeout, LenTimeout, OffsetTimeout, ValidTimeout, NULL },
	{ kP2CompactProxyActionThenGetRequestNameTsa, LenTsa, OffsetTsa, ValidTsa, NULL },
	{ kP2CompactProxyActionThenGetRequestNameSequenceOfOmdDataOadUnsigned, LenSequenceOfOmdDataOadUnsigned, OffsetSequenceOfOmdDataOadUnsigned, ValidSequenceOfOmdDataOadUnsigned, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyActionThenGetRequestCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyActionThenGetRequestCutIxTimeout]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyActionThenGetRequestCutIxTsa]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyActionThenGetRequestCutIxSequenceOf]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxyActionThenGetRequestPcutOpen(P2CompactProxyActionThenGetRequestPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2TsaPcutOpen(&m->tsa));
	PcutSubSet(&m->base, kP2CompactProxyActionThenGetRequestCutIxTsa, &m->tsa.base, NULL);

	ifer(P2OmdDataOadUnsignedPcutOpen(&m->omd_data_oad_unsigned_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_omd_data_oad_unsigned_cut, &m->omd_data_oad_unsigned_cut.base, kP2CompactProxyActionThenGetRequestNameSequenceOfOmdDataOadUnsigned));
	PcutSubSet(&m->base, kP2CompactProxyActionThenGetRequestCutIxSequenceOf, &m->sequence_of_omd_data_oad_unsigned_cut.base, NULL);
	return 0;
}
cp_t P2CompactProxyActionThenGetRequestPcutClose(P2CompactProxyActionThenGetRequestPcut *m)
{
	dve(P2CompactProxyActionThenGetRequestPcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxyActionThenGetRequestCutIxTsa, NULL, NULL);
	ifer(P2TsaPcutClose(&m->tsa));

	PcutSubSet(&m->base, kP2CompactProxyActionThenGetRequestCutIxSequenceOf, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_omd_data_oad_unsigned_cut));
	ifer(P2OmdDataOadUnsignedPcutClose(&m->omd_data_oad_unsigned_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxyActionThenGetRequestPcutValid(const P2CompactProxyActionThenGetRequestPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2TsaPcutValid(&m->tsa));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_omd_data_oad_unsigned_cut));
	ifer(P2OmdDataOadUnsignedPcutValid(&m->omd_data_oad_unsigned_cut));
	return 0;
}


cp_t P2CompactProxyActionThenGetRequestPcutOpenBase(Pcut *base)
{
	P2CompactProxyActionThenGetRequestPcut *m = (P2CompactProxyActionThenGetRequestPcut*)base;
	return P2CompactProxyActionThenGetRequestPcutOpen(m);
}
cp_t P2CompactProxyActionThenGetRequestPcutCloseBase(Pcut *base)
{
	P2CompactProxyActionThenGetRequestPcut *m = (P2CompactProxyActionThenGetRequestPcut*)base;
	return P2CompactProxyActionThenGetRequestPcutClose(m);
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
cp_t P2CompactProxyActionThenGetRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxyActionThenGetRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
