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

20230730-----------------------------------------------------------------------
Huanglin创建.
*/
#include "gtest/gtest.h"
#include "fileid.h"
#include "qcp.h"
#include "p2_datetime.h"
#include "p2_datetime_ut.xcp.h"



static const FileidItem kFileidItems[] = {
#include "id_filename_table_uniq.h"
	    //#include "../libctlcomm/id_filename_table_uniq.h"
	    //#include "../libctl698/id_filename_table_uniq.h"
};
static const int kFileidNum = sizeof(kFileidItems) / sizeof(kFileidItems[0]);
static const FileidTable kFileidTable = {kFileidItems, kFileidNum};
const FileidTable *MainFileidTable() { return &kFileidTable; }

namespace {

// ../src/dlt698/p2_datetime.cc
TEST(p2_double_long, all) {
	const cp_t cp = P2DatetimeTest(1, 0);
	if (0 != cp)
		std::cout << CpStr(cp) << std::endl;
	EXPECT_EQ(0, cp);
	return;
}




}  // namespace





