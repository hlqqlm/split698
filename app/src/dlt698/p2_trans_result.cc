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

20230816-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 trans result choice 变化部分报文解析
*/

// var
#include "p2_dar.h"
#include "p2_octet_string.h"


#include "p2_trans_result.h"
#include "p2_trans_result.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(1)
#define kChoiceNum				(kP2TransResultChoiceNum)


// {{{ choice
// 必须按大小顺序排
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2TransResultChoiceDar, "dar" },		// 转发结果 [0] DAR，
	{ kP2TransResultChoiceReturnData, "return_data" },		// 返回数据 [1] octet-string
};
int P2TransResultChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2TransResultChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2TransResultChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2TransResultChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2TransResultChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2DarPcut kP2DarPcutDefVar = kP2DarPcutDef;
static const P2OctetStringPcut kP2OctetStringPcutDefVar = kP2OctetStringPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2TransResultChoiceNameDar, sizeof(P2DarPcut), &kP2DarPcutDefVar, P2DarPcutOpenBase, P2DarPcutCloseBase },	// 转发结果 [0] DAR，
	{ kP2TransResultChoiceNameReturnData, sizeof(P2OctetStringPcut), &kP2OctetStringPcutDefVar, P2OctetStringPcutOpenBase, P2OctetStringPcutCloseBase },	// 返回数据 [1] octet-string
};
//}}}


//{{{ pcut
cp_t P2TransResultPcutOpen(P2TransResultPcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2TransResultNameChoice, kChoiceList, kChoiceNum, kVarFactoryInfoList);
}
cp_t P2TransResultPcutClose(P2TransResultPcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2TransResultPcutValid(const P2TransResultPcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}



// 定义用子类base来open/close父类的函数
cp_t P2TransResultPcutOpenBase(Pcut *base)
{
	P2TransResultPcut *m = (P2TransResultPcut*)base;
	return P2TransResultPcutOpen(m);
}
cp_t P2TransResultPcutCloseBase(Pcut *base)
{
	P2TransResultPcut *m = (P2TransResultPcut*)base;
	return P2TransResultPcutClose(m);
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
cp_t P2TransResultTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2TransResultTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

