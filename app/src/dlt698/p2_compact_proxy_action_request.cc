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

20230829-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的CompactProxyActionRequest
*/
#include "p2_compact_proxy_action_request.h"
#include "p2_compact_proxy_action_request.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2CompactProxyActionRequestCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionRequestPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionRequestPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ timeout
static int LenTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionRequestTimeoutSize; }
static int OffsetTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionRequestTimeoutOffset; }
static cp_t ValidTimeout(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ tsa
#define LenTsa		PcutItemLenBySub
static int OffsetTsa(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionRequestTsaOffset; }
#define ValidTsa		PcutItemValidBySub
//}}}


// {{{ sequence_of_omd_data
#define LenSequenceOfOmdData		PcutItemLenBySub
// static int OffsetSequenceOfOmdData(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyActionRequestSequenceOfOmdDataOffset; }
#define OffsetSequenceOfOmdData		PcutItemOffsetDef
#define ValidSequenceOfOmdData		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2CompactProxyActionRequestNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2CompactProxyActionRequestNameTimeout, LenTimeout, OffsetTimeout, ValidTimeout, NULL },
	{ kP2CompactProxyActionRequestNameTsa, LenTsa, OffsetTsa, ValidTsa, NULL },
	{ kP2CompactProxyActionRequestNameSequenceOfOmdData, LenSequenceOfOmdData, OffsetSequenceOfOmdData, ValidSequenceOfOmdData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyActionRequestCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyActionRequestCutIxTimeout]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyActionRequestCutIxTsa]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyActionRequestCutIxSequenceOfOmdData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxyActionRequestPcutOpen(P2CompactProxyActionRequestPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2TsaPcutOpen(&m->tsa));
	PcutSubSet(&m->base, kP2CompactProxyActionRequestCutIxTsa, &m->tsa.base, NULL);

	ifer(P2OmdDataPcutOpen(&m->omd_data_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_omd_data_cut, &m->omd_data_cut.base, kP2CompactProxyActionRequestNameSequenceOfOmdData));
	PcutSubSet(&m->base, kP2CompactProxyActionRequestCutIxSequenceOfOmdData, &m->sequence_of_omd_data_cut.base, NULL);
	return 0;
}
cp_t P2CompactProxyActionRequestPcutClose(P2CompactProxyActionRequestPcut *m)
{
	dve(P2CompactProxyActionRequestPcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxyActionRequestCutIxTsa, NULL, NULL);
	ifer(P2TsaPcutClose(&m->tsa));

	PcutSubSet(&m->base, kP2CompactProxyActionRequestCutIxSequenceOfOmdData, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_omd_data_cut));
	ifer(P2OmdDataPcutClose(&m->omd_data_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxyActionRequestPcutValid(const P2CompactProxyActionRequestPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2TsaPcutValid(&m->tsa));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_omd_data_cut));
	ifer(P2OmdDataPcutValid(&m->omd_data_cut));
	return 0;
}


cp_t P2CompactProxyActionRequestPcutOpenBase(Pcut *base)
{
	P2CompactProxyActionRequestPcut *m = (P2CompactProxyActionRequestPcut*)base;
	return P2CompactProxyActionRequestPcutOpen(m);
}
cp_t P2CompactProxyActionRequestPcutCloseBase(Pcut *base)
{
	P2CompactProxyActionRequestPcut *m = (P2CompactProxyActionRequestPcut*)base;
	return P2CompactProxyActionRequestPcutClose(m);
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
cp_t P2CompactProxyActionRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxyActionRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
