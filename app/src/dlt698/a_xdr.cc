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
 

20191216-----------------------------------------------------------------------
创建.
DL/T 790.6-2010  采用配电线载波系统的配电自动化 第6部分 A-XDR编码规则	
*/

#include "qos/qcp.h"
#include "qos/qbit.h"
#include "a_xdr.h"
#include "a_xdr.xcp.h"
//#define this_file_id	0xb85a6ca7 // echo -n a_xdr.c | rhash --simple -

#define TEST_EN			(0)

// 长度区在一个字节里编码，第8为设置为1，表示长度区是存在的。其它7为编码为固定长度的无符号整数，其值表示内容区的字节数。
// 所以：长度区字节数，应该是VariableLenIntContentByteNum(start) + 1
// 这里：content，是长度区内容字节数
static int VariableLenIntContentByteNum(char start)
{
	if (0 == BIT_CHECK(start, 7))
		return 0;		// 0表示长度区没有
	const int len = (unsigned char )start & 0x7f;
	return len;
}


//{{{ VariableLenInt可变长度的整数值编码
int VariableLenIntByteNum(char start)
{
	return VariableLenIntContentByteNum(start) + 1;
}

// 值
// 如果出现错误，返回0xffffffff
int VariableLenIntValue(const char buf[])
{
	const int content_byte_num = VariableLenIntContentByteNum(*buf);
	if (0 == content_byte_num)
	{
		const unsigned char v = (unsigned char)(*buf);
		return v;
	}
	// 只处理内容域长度<=4的情况。
	dvb(content_byte_num <= kVariableLenIntContentByteNumMax);
	if (kVariableLenIntContentByteNumMax < content_byte_num)
		return kVariableLenInvalidLen;	// return 0xffffffff;

	const unsigned char *p = (unsigned char *)buf + 1;
	int32_t n = 0;
	for (int i = 0; i < content_byte_num; i++, p++)
	{
		const int shift_bit = (content_byte_num - i - 1) * 8;
		const uint32_t t = ((uint32_t)*p) << shift_bit;
		n += t;
	}
	return n;
}

#if 0
std::string N2AXdr(int n)
{
	DVB(0 <= n);
	if (0 <= n && n < (int)k2Pow7)	// 128 = 2^7
	{
		const uint8_t v = (uint8_t)n;
		std::string ret((char*)&v, 1);
		return ret;
	}
	if ((int)k2Pow7 <= n && n < (int)k2Pow15) // 32768 = 2^15
	{
		const uint8_t content_byte_num = 2;
		// 长度区
		const uint8_t len_field = BIT_SET_VALUE(content_byte_num, 7, 1);
		// b1 b0
		const uint8_t b0 = n % 0x100;
		const uint8_t b1 = n / 0x100;

		std::string ret((char*)&len_field, 1);
		ret.append((char*)&b1, 1);
		ret.append((char*)&b0, 1);
		return ret;
	}
	if ((int)k2Pow15 <= n && n < (int)k2Pow23) // 8388608 = 2^23
	{
		const uint8_t content_byte_num = 3;
		// 长度区
		const uint8_t len_field = BIT_SET_VALUE(content_byte_num, 7, 1);
		// b2 b1 b0
		const uint8_t b0 = n % 0x100;
		const uint16_t high = n / 0x100;
		const uint8_t b1 = high % 0x100;
		const uint8_t b2 = high / 0x100;

		std::string ret((char*)&len_field, 1);
		ret.append((char*)&b2, 1);
		ret.append((char*)&b1, 1);
		ret.append((char*)&b0, 1);
		return ret;
	}

	if ((int)k2Pow23 <= n && n < (int)k2Pow31) 
	{
		const uint8_t content_byte_num = 4;
		// 长度区
		const uint8_t len_field = BIT_SET_VALUE(content_byte_num, 7, 1);
		// b3 b2 b1 b0
		const uint8_t b0 = n % 0x100;
		uint32_t t = n / 0x100;
		const uint8_t b1 = t % 0x100;
		t = t / 0x100;
		const uint8_t b2 = t % 0x100;
		const uint8_t b3 = t / 0x100;

		std::string ret((char*)&len_field, 1);
		ret.append((char*)&b3, 1);
		ret.append((char*)&b2, 1);
		ret.append((char*)&b1, 1);
		ret.append((char*)&b0, 1);
		return ret;
	}

	// 处理不了4字节以上的情况
	DVB(false);
	return "";
}
#endif
int N2AXdr(char *dst, int n)
{
	dvb(0 <= n);
	if (0 <= n && n < (int)k2Pow7)	// 128 = 2^7
	{
		const uint8_t v = (uint8_t)n;
		dst[0] = v;
		// std::string ret((char*)&v, 1);
		return 1;
	}
	if ((int)k2Pow7 <= n && n < (int)k2Pow15) // 32768 = 2^15
	{
		const uint8_t content_byte_num = 2;
		// 长度区
		const uint8_t len_field = BIT_SET_VALUE(content_byte_num, 7, 1);
		// b1 b0
		const uint8_t b0 = n % 0x100;
		const uint8_t b1 = n / 0x100;

		//std::string ret((char*)&len_field, 1);
		//ret.append((char*)&b1, 1);
		//ret.append((char*)&b0, 1);
		dst[0] = len_field;
		dst[1] = b1;
		dst[2] = b0;
		return 3;
	}
	if ((int)k2Pow15 <= n && n < (int)k2Pow23) // 8388608 = 2^23
	{
		const uint8_t content_byte_num = 3;
		// 长度区
		const uint8_t len_field = BIT_SET_VALUE(content_byte_num, 7, 1);
		// b2 b1 b0
		const uint8_t b0 = n % 0x100;
		const uint16_t high = n / 0x100;
		const uint8_t b1 = high % 0x100;
		const uint8_t b2 = high / 0x100;

		//std::string ret((char*)&len_field, 1);
		//ret.append((char*)&b2, 1);
		//ret.append((char*)&b1, 1);
		//ret.append((char*)&b0, 1);
		dst[0] = len_field;
		dst[1] = b2;
		dst[2] = b1;
		dst[3] = b0;
		return 4;
	}

	if (k2Pow23 <= (unsigned int)n && (unsigned int)n < k2Pow31) 
	{
		const uint8_t content_byte_num = 4;
		// 长度区
		const uint8_t len_field = BIT_SET_VALUE(content_byte_num, 7, 1);
		// b3 b2 b1 b0
		const uint8_t b0 = n % 0x100;
		uint32_t t = n / 0x100;
		const uint8_t b1 = t % 0x100;
		t = t / 0x100;
		const uint8_t b2 = t % 0x100;
		const uint8_t b3 = t / 0x100;

		//std::string ret((char*)&len_field, 1);
		//ret.append((char*)&b3, 1);
		//ret.append((char*)&b2, 1);
		//ret.append((char*)&b1, 1);
		//ret.append((char*)&b0, 1);
		dst[0] = len_field;
		dst[1] = b3;
		dst[2] = b2;
		dst[3] = b1;
		dst[4] = b0;
		return 5;
	}

	// 处理不了4字节以上的情况
	dvb(false);
	return 0;
}



//}}}






//{{{ test
#if TEST_EN > 0
#include <string.h>
static cp_t TestVariableLenIntContentByteNum(void)
{
    ifbr(0 == VariableLenIntContentByteNum(0x00));
    ifbr(0 == VariableLenIntContentByteNum(0x7b));
    ifbr(1 == VariableLenIntContentByteNum(0x81));
    ifbr(2 == VariableLenIntContentByteNum(0x82));
    return 0;
}
static cp_t TestVariableLenIntValue(void)
{
    ifbr(123 == VariableLenIntValue("\x7b"));
    ifbr(0 == VariableLenIntValue("\x00"));
    ifbr(128 == VariableLenIntValue("\x82\x00\x80"));
    return 0;
}
static cp_t TestN2AXdr(void)
{
	char mem[10] = {0};
	const int mem_size = sizeof(mem);

	memset(mem, 0, mem_size);
	ifbr(1 == N2AXdr(mem, 123));
	ifbr(0 == memcmp("\x7b", mem, 1));

	memset(mem, 0, mem_size);
	ifbr(1 == N2AXdr(mem, 0));
	ifbr(0 == memcmp("\x00", mem, 1));

	memset(mem, 0, mem_size);
	ifbr(3 == N2AXdr(mem, 128));
	ifbr(0 == memcmp("\x82\x00\x80", mem, 3));

	memset(mem, 0, mem_size);
	ifbr(4 == N2AXdr(mem, k2Pow15));
	ifbr(0 == memcmp("\x83\x00\x80\x00", mem, 4));

	memset(mem, 0, mem_size);
	// qos_printf("N2AXdr(mem, k2Pow23) = %d\n", N2AXdr(mem, k2Pow23));
	ifbr(5 == N2AXdr(mem, k2Pow23));
    ifbr(0 == memcmp("\x84\x00\x80\x00\x00", mem, 5));
	return 0;
}
#if 0
static cp_t TestN2AXdr(void)
{
	EXPECT_EQ(std::string("\x7b", 1), N2AXdr(123));
    EXPECT_EQ(std::string("\x00", 1), N2AXdr(0));
    EXPECT_EQ(std::string("\x82\x00\x80", 3), N2AXdr(128));
    EXPECT_EQ(std::string("\x83\x00\x80\x00", 4), N2AXdr(k2Pow15));
    EXPECT_EQ(std::string("\x84\x00\x80\x00\x00", 5), N2AXdr(k2Pow23));
    return;
}
#endif

static const QtestItem kTestItem[] = {
	TestVariableLenIntContentByteNum,
	TestVariableLenIntValue,
	TestN2AXdr,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t AXdrTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t AXdrTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
