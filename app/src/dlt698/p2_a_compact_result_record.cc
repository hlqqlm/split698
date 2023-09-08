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
DLT698_45  ACompactResultRecord 变化部分报文解析
*/

// var
#include "p2_dar.h"
#include "p2_sequence_of_a_record_row.h"


#include "p2_a_compact_result_record.h"
#include "p2_a_compact_result_record.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(1)
#define kChoiceNum				(kP2ACompactResultRecordChoiceNum)


// {{{ choice
// 必须按大小顺序排
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2ACompactResultRecordChoiceDar, "dar" },		// 错误信息 [0] DAR，
	{ kP2ACompactResultRecordChoiceSequenceOfARecordRow, "sequence_of_a_record_row" },	// M 条记录 [1] SEQUENCE OF A-RecordRow
};
int P2ACompactResultRecordChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2ACompactResultRecordChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2ACompactResultRecordChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2ACompactResultRecordChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2ACompactResultRecordChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2DarPcut kP2DarPcutDefVar = kP2DarPcutDef;
static const P2SequenceOfARecordRowPcut kP2SequenceOfARecordRowPcutDefVar = kP2SequenceOfARecordRowPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2DarName
		, sizeof(P2DarPcut), &kP2DarPcutDefVar
		, P2DarPcutOpenBase, P2DarPcutCloseBase },	
	// 错误信息 [0] DAR，

	{ kP2SequenceOfARecordRowName
		, sizeof(P2SequenceOfARecordRowPcut), &kP2SequenceOfARecordRowPcutDefVar
			, P2SequenceOfARecordRowPcutOpenBase, P2SequenceOfARecordRowPcutCloseBase },	
	// M 条记录 [1] SEQUENCE OF A-RecordRo
};
//}}}


//{{{ cut
cp_t P2ACompactResultRecordPcutOpen(P2ACompactResultRecordPcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ACompactResultRecordNameChoice, kChoiceList, kChoiceNum, kVarFactoryInfoList);
}
cp_t P2ACompactResultRecordPcutClose(P2ACompactResultRecordPcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2ACompactResultRecordPcutValid(const P2ACompactResultRecordPcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}


// 定义用子类base来open/close父类的函数
//PCUT_DEFINE_OPEN_BASE(P2ACompactResultRecordPcut);
//PCUT_DEFINE_CLOSE_BASE(P2ACompactResultRecordPcut);

cp_t P2ACompactResultRecordPcutOpenBase(Pcut *base)
{
	P2ACompactResultRecordPcut *m = (P2ACompactResultRecordPcut*)base;
	return P2ACompactResultRecordPcutOpen(m);
}
cp_t P2ACompactResultRecordPcutCloseBase(Pcut *base)
{
	P2ACompactResultRecordPcut *m = (P2ACompactResultRecordPcut*)base;
	return P2ACompactResultRecordPcutClose(m);
}
//}}}



//
cp_t P2ACompactResultRecordChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ACompactResultRecordNameChoice, PfillItemOffsetFix, kP2ACompactResultRecordChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2ACompactResultRecordNameVar, PfillItemOffsetFix, PfillItemProcessBySub, kP2ACompactResultRecordVarOffset, sub_var);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVar), &kFiVar));

	return 0;
}


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
cp_t P2ACompactResultRecordChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ACompactResultRecordChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

