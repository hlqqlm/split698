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

20230824-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的connect response info
*/

// for test

#include "p2_connect_response_info.h"
#include "p2_connect_response_info.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2ConnectResponseInfoCutNum)


//{{{ connect_result
// 必须按大小顺序排
static const P2Enum kConnectResultEnumList[kP2ConnectResultNum] = {
	// enumv	name
	{ kP2ConnectResultAllowConnection, "allow_connection" },		// 允许建立应用连接 （0），
	{ kP2ConnectResultPasswordWrong, "password_wrong" },		// 密码错误 （1），
	{ kP2ConnectResultSymmetricDecryptionError, "symmetric_decryption_error" },		// 对称解密错误 （2），
	{ kP2ConnectResultAsymmetricDecryptionError, "asymmetric_decryption_error" },		// 非对称解密错误 （3），
	{ kP2ConnectResultSignWrong, "sign_wrong" },		// 签名错误 （4），
	{ kP2ConnectResultProtocolVersionWrong, "protocol_version_wrong" },		// 协议版本不匹配 （5），
	{ kP2ConnectResultEsamError, "esam_error" },		// ESAM 通信故障 （6），
	{ kP2ConnectResultOtherError, "other_error" },		// 其他错误 （255）
};
int P2ConnectResultEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kConnectResultEnumList, kP2ConnectResultNum, enumv);
}
uint8_t P2ConnectResultEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kConnectResultEnumList, kP2ConnectResultNum, enumv_ix);
}
cp_t P2ConnectResultEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kP2ConnectResultNum, enumv_ix);
}
cp_t P2ConnectResultEnumValid(uint8_t enumv)
{
	return P2EnumValid(kConnectResultEnumList, kP2ConnectResultNum, enumv);
}
const char *P2ConnectResultEnumStr(uint8_t enumv)
{
	return P2EnumStr(kConnectResultEnumList, kP2ConnectResultNum, enumv);
}



uint8_t P2ConnectResultValue(const char *whole)
{
	return whole[kP2ConnectResponseInfoConnectResultOffset];
}
const char *P2ConnectResultMem(const char *whole)
{
	return whole + kP2ConnectResponseInfoConnectResultOffset;
}

#define LenConnectResult		PcutItemLenBySub
//#define OffsetConnectResult	PcutItemOffsetDef
static int OffsetConnectResult(Pcut *cut, int ix, const char *whole) { return kP2ConnectResponseInfoConnectResultOffset; }
#define ValidConnectResult	PcutItemValidBySub
//}}}




// {{{ security_data_optional
#define LenSecurityDataOptional		PcutItemLenBySub
static int OffsetSecurityDataOptional(Pcut *cut, int ix, const char *whole) { return kP2ConnectResponseInfoSecurityDataOptionalOffset; }
#define ValidSecurityDataOptional		PcutItemValidBySub
//}}}



//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ConnectResponseInfoNameConnectResult, LenConnectResult, OffsetConnectResult, ValidConnectResult, NULL },
	{ kP2ConnectResponseInfoNameSecurityDataOptional, LenSecurityDataOptional, OffsetSecurityDataOptional, ValidSecurityDataOptional, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ConnectResponseInfoCutIxConnectResult]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ConnectResponseInfoCutIxSecurityDataOptional]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ConnectResponseInfoPcutOpen(P2ConnectResponseInfoPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	//ifer(P2ConnectResultPcutOpen(&m->dar_cut));
	//PcutSubSet(&m->base, kP2ConnectResponseInfoCutIxConnectResult, &m->connect_result.base, kP2ConnectResponseInfoNameConnectResult);
	ifer(P2EnumPcutOpen(&m->enum_connect_result, kP2ConnectResponseInfoNameConnectResult, kConnectResultEnumList, kP2ConnectResultNum));
	PcutSubSet(&m->base, kP2ConnectResponseInfoCutIxConnectResult, &m->enum_connect_result.base, kP2ConnectResponseInfoNameConnectResult);

	ifer(P2SecurityDataPcutOpen(&m->security_data));
	ifer(P2OptionalPcutOpen(&m->security_data_optional, &m->security_data.base, kP2ConnectResponseInfoNameSecurityDataOptional));
	PcutSubSet(&m->base, kP2ConnectResponseInfoCutIxSecurityDataOptional, &m->security_data_optional.base, NULL);
	return 0;
}
cp_t P2ConnectResponseInfoPcutClose(P2ConnectResponseInfoPcut *m)
{
	dve(P2ConnectResponseInfoPcutValid(m));

	PcutSubSet(&m->base, kP2ConnectResponseInfoCutIxConnectResult, NULL, NULL);
	//ifer(P2ConnectResultPcutClose(&m->connect_result));
	//PcutSubSet(&m->base, kP2ComdcbCutIxBps, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_connect_result));

	PcutSubSet(&m->base, kP2ConnectResponseInfoCutIxSecurityDataOptional, NULL, NULL);
	ifer(P2OptionalPcutClose(&m->security_data_optional));
	ifer(P2SecurityDataPcutClose(&m->security_data));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ConnectResponseInfoPcutValid(const P2ConnectResponseInfoPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2EnumPcutValid(&m->enum_connect_result));
	//ifer(P2ConnectResultPcutValid(&m->connect_result));
	ifer(P2OptionalPcutValid(&m->security_data_optional));
	ifer(P2SecurityDataPcutValid(&m->security_data));
	return 0;
}


cp_t P2ConnectResponseInfoPcutOpenBase(Pcut *base)
{
	P2ConnectResponseInfoPcut *m = (P2ConnectResponseInfoPcut*)base;
	return P2ConnectResponseInfoPcutOpen(m);
}
cp_t P2ConnectResponseInfoPcutCloseBase(Pcut *base)
{
	P2ConnectResponseInfoPcut *m = (P2ConnectResponseInfoPcut*)base;
	return P2ConnectResponseInfoPcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0

static cp_t TestPfill(void)
{
	return 0;
}

static const QtestItem kTestItem[] = {
	TestPfill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ConnectResponseInfoTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ConnectResponseInfoTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

