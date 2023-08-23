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
DLT698_45 ConnectMechanismInfo choice 部分报文解析
*/

// var
#include "p2_null.h"
#include "p2_visible_string.h"
#include "p2_symmetry_security.h"
#include "p2_signature_security.h"

#include "p2_connect_mechanism_info_choice.h"
#include "p2_connect_mechanism_info_choice.xcp.h"


#define TEST_EN				(0)


// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2ConnectMechanismInfoChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2ConnectMechanismInfoChoiceNull, kP2ConnectMechanismInfoNameNull },	// 公共连接 [0] NullSecurity，
	{ kP2ConnectMechanismInfoChoicePassword, kP2ConnectMechanismInfoNamePassword },	// 一般密码 [1] PasswordSecurity，
	{ kP2ConnectMechanismInfoChoiceSymmetry, kP2ConnectMechanismInfoNameSymmetry },	// 对称加密 [2] SymmetrySecurity，
	{ kP2ConnectMechanismInfoChoiceSignature, kP2ConnectMechanismInfoNameSignature },	// 数字签名 [3] SignatureSecurity
};
int P2ConnectMechanismInfoChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2ConnectMechanismInfoChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2ConnectMechanismInfoChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2ConnectMechanismInfoChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2NullPcut kP2NullPcutDefVar = kP2NullPcutDef;
static const P2VisibleStringPcut kP2VisibleStringPcutDefVar = kP2VisibleStringPcutDef;	// for password
static const P2SymmetrySecurityPcut kP2SymmetrySecurityPcutDefVar = kP2SymmetrySecurityPcutDef;
static const P2SignatureSecurityPcut kP2SignatureSecurityPcutDefVar = kP2SignatureSecurityPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2ConnectMechanismInfoChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2ConnectMechanismInfoNameNull, sizeof(P2NullPcut), &kP2NullPcutDefVar, P2NullPcutOpenBase, P2NullPcutCloseBase },	// 公共连接 [0] NullSecurity，
	{ kP2ConnectMechanismInfoNamePassword, sizeof(P2VisibleStringPcut), &kP2VisibleStringPcutDefVar, P2VisibleStringPcutOpenBase, P2VisibleStringPcutCloseBase },	// 一般密码 [1] PasswordSecurity，
	{ kP2ConnectMechanismInfoNameSymmetry, sizeof(P2SymmetrySecurityPcut), &kP2SymmetrySecurityPcutDefVar, P2SymmetrySecurityPcutOpenBase, P2SymmetrySecurityPcutCloseBase },	// 对称加密 [2] SymmetrySecurity，
	{ kP2ConnectMechanismInfoNameSignature, sizeof(P2SignatureSecurityPcut), &kP2SignatureSecurityPcutDefVar, P2SignatureSecurityPcutOpenBase, P2SignatureSecurityPcutCloseBase },	// 数字签名 [3] SignatureSecurity
};
//}}}


//{{{ cut
cp_t P2ConnectMechanismInfoChoicePcutOpen(P2ConnectMechanismInfoChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ConnectMechanismInfoChoiceName, 
			kChoiceList, kP2ConnectMechanismInfoChoiceNum, kVarFactoryInfoList);
}
cp_t P2ConnectMechanismInfoChoicePcutClose(P2ConnectMechanismInfoChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2ConnectMechanismInfoChoicePcutValid(const P2ConnectMechanismInfoChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
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
cp_t P2ConnectMechanismInfoChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ConnectMechanismInfoChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

