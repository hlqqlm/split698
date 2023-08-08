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
#include <regex.h>
#include <vector>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "fileid.h"
#include "qcp.h"
#include "qstr.h"
#include "qstr_ut.xcp.h"
// FILEID_REGISTER();

static const FileidItem kFileidItems[] = {
#include "id_filename_table_uniq.h"
	    //#include "../libctlcomm/id_filename_table_uniq.h"
	    //#include "../libctl698/id_filename_table_uniq.h"
};
static const int kFileidNum = sizeof(kFileidItems) / sizeof(kFileidItems[0]);
static const FileidTable kFileidTable = {kFileidItems, kFileidNum};
const FileidTable *MainFileidTable() { return &kFileidTable; }

namespace {



TEST(QStr, str) {
	EXPECT_EQ(0, StrValid("123", 4));
	EXPECT_EQ(0, StrValid("123", 1000));
	EXPECT_EQ(0, StrValid("", 4));
	EXPECT_EQ(0, StrValid("", 0));

	EXPECT_NE(0, StrValid(NULL, 4));
	EXPECT_NE(0, StrValid("123", 3));

	EXPECT_EQ(0, StrValid("123", 0));
	return;
}


TEST(QStr, StrLen) {
	EXPECT_EQ(-1, StrLen("", 0));
	EXPECT_EQ(0, StrLen("\x00", 1));
	EXPECT_EQ(1, StrLen("\x31\x00", 2));
	EXPECT_EQ(2, StrLen("\x31\x32\x00", 3));
	EXPECT_EQ(-1, StrLen("\x31\x32\x00", 2));
	return;
}



TEST(QStr, HexStr) {
	EXPECT_EQ("12ab", HexStr("\x12\xab", 2));
	EXPECT_EQ("81070200", HexStr(0x81070200));

    EXPECT_EQ("12ab", HexStr(std::string("\x12\xab")));
    EXPECT_EQ("0012ab", HexStr(std::string("\x00\x12\xab", 3)));
}


TEST(QStr, std_string) {
    EXPECT_EQ(std::string("\x12\xab", 2), "\x12\xab");
    EXPECT_NE(std::string("\x12\xab\x00", 3), "\x12\xab");

    EXPECT_EQ(std::string("\x00\x12\xab", 3).size(), 3);
    EXPECT_EQ(std::string("\x00\x12\xab", 3), std::string("\x00\x12\xab", 3));
    EXPECT_EQ(0, memcmp("\x00\x12\xab", std::string("\x00\x12\xab", 3).data(), 3));
}


TEST(QStr, HexStr2Mem) {
    {
    const std::string dst = HexStr2Mem("");
    EXPECT_TRUE(dst.empty());
    EXPECT_EQ(0, dst.size());
    }
    EXPECT_EQ(std::string(), HexStr2Mem(""));
    {
    const std::string dst = HexStr2Mem("12ab");
    EXPECT_TRUE(!dst.empty());
    EXPECT_EQ(2, dst.size());
    EXPECT_EQ(0, memcmp("\x12\xab", dst.data(), 2));
    }
    EXPECT_EQ(std::string("\x12\xab", 2), HexStr2Mem("12ab"));
    // C++ exception with description "basic_string::_M_construct null not valid" thrown in the test body.
    EXPECT_EQ(std::string("\x00\x12\xab", 3), HexStr2Mem("0012ab"));
    EXPECT_EQ(std::string("\x12\xab\x00", 3), HexStr2Mem("12ab00"));
    EXPECT_EQ(std::string(), HexStr2Mem(""));
    EXPECT_EQ(std::string("\xa0", 1), HexStr2Mem("a"));
    EXPECT_EQ(std::string("\x12\x30", 2), HexStr2Mem("123"));
    EXPECT_EQ(std::string("\x00\xf0", 2), HexStr2Mem("00f"));
    EXPECT_EQ(std::string("\x00\xf0", 2), HexStr2Mem("00f0"));
    EXPECT_EQ(std::string("\x00\x0f", 2), HexStr2Mem("000f"));
    EXPECT_EQ(std::string("\xff\xff", 2), HexStr2Mem("ffff"));
}


TEST(QStr, split) {
	const char s[] = "impulse/notify/impulseEvent/control/dataUpdate";
	const std::vector<std::string> v = StrSplit(s, '/'); //可按多个字符来分隔;
	EXPECT_EQ(5, v.size());
	EXPECT_EQ("impulse", v[0]);
	EXPECT_EQ("notify", v[1]);
	EXPECT_EQ("impulseEvent", v[2]);
	EXPECT_EQ("control", v[3]);
	EXPECT_EQ("dataUpdate", v[4]);
	//std::cout << v << std::endl;
	/*
	for(std::vector<std::string>::size_type i = 0; i < v.size(); ++i)
		std::cout << v[i] << " ";
	std::cout << std::endl;
	*/
}


TEST(QStr, StringReplace) {
	const std::string ori("\x01\x02\x00", 3);
	{
	std::string s(ori);
	// offset < s长度, offset + size < s长度
	EXPECT_EQ(std::string("\x03\x04\x00", 3), StringReplace(s, 0, 2, "\x03\x04\x00"));
	}
	{
	std::string s(ori);
	// offset == s长度-1
	EXPECT_EQ(std::string("\x01\x02\x03\x04", 4), StringReplace(s, 2, 2, "\x03\x04"));
	}
	{
	std::string s(ori);
	// offset == s长度
	EXPECT_EQ(std::string("\x01\x02\x00\x03\x04", 5), StringReplace(s, 3, 2, "\x03\x04"));
	}
	{
	std::string s(ori);
	// offset > s长度，会在空白处填写'\0'，相当于结构体中的空白处pad
	EXPECT_EQ(std::string("\x01\x02\x00\x00\x03\x04", 6), StringReplace(s, 4, 2, "\x03\x04"));
	}

	return;
}

}  // namespace








