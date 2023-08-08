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


bit operation: set, clear, toggle

20130516------------------------------------------------------------------------
bitmap highest byte in ix 0, reason is below:
1       Dsp - arm protcol translate high byte first. If bitmap low byte first,
        I need swap byte sequence.
2       Print the bitmap array, the sequence is match human's habit.
        Such as bitmap "0001 0010 0011 0100" -> hex "0x12 0x34", we always think
        highest bit is at left, lowest bit is at right. If we use lowest byte
        at index 0, the array content is "0x34 0x12", if we print the array,
        the sequence do NOT match human reading habit.

20130509------------------------------------------------------------------------
support bitmap which is a unsigned char array bit opperation.
*/
#include "qcp.h"
#include "qstr.h"
#include "qbit.h"
#include "qbit.xcp.h"
//#include "qbit.xcp.h"
//FILEID_REGISTER();
//#define this_file_id    0x4a88eb2b // echo -n qbit.cc | rhash --simple -



//{{{ bit
#if 0
int Bit1Num(unsigned long x)
{
    int n = 0;
    if (x) do ++n;
        while (x &= x-1);
    return n;
}
#endif
// http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive
// Counting bits set (naive way)
// unsigned int v; // count the number of bits set in v
unsigned int Bit1Num(unsigned int v)
{
    unsigned int c; // c accumulates the total bits set in v
    for (c = 0; v; v >>= 1)
        c += v & 1;

    return c;
}
//}}}



//{{{bitmap
// bitmap, lowest byte at index 0
// Index to byte.
#define IX(bit)         ((bit) / 8)
// Specific bit in byte.
#define POS(bit)        ((bit) % 8)
//
#define THE_BYTE(bitmap, bit)   (bitmap[IX(bit)])
void bitmap_set_value(char *bitmap, int bit, bool v)
{
    dvb(NULL != bitmap);
    dvb(0 <= bit);

    unsigned char *ubm = (unsigned char*)bitmap;
    THE_BYTE(ubm, bit) = BIT_SET_VALUE(THE_BYTE(ubm, bit), POS(bit), v);
}
void bitmap_set(char *bitmap, int bit)
{
    dvb(NULL != bitmap);
    dvb(0 <= bit);

    unsigned char *ubm = (unsigned char*)bitmap;
    THE_BYTE(ubm, bit) = BIT_SET(THE_BYTE(ubm, bit), POS(bit));
}
void bitmap_clear(char *bitmap, int bit)
{
    dvb(NULL != bitmap);
    dvb(0 <= bit);

    unsigned char *ubm = (unsigned char*)bitmap;
    THE_BYTE(ubm, bit) = BIT_CLEAR(THE_BYTE(ubm, bit), POS(bit));
}
void bitmap_toggle(char *bitmap, int bit)
{
    dvb(NULL != bitmap);
    dvb(0 <= bit);

    unsigned char *ubm = (unsigned char*)bitmap;
    THE_BYTE(ubm, bit) = BIT_TOGGLE(THE_BYTE(ubm, bit), POS(bit));
}
bool bitmap_check(const char *bitmap, int bit)
{
    dvb(NULL != bitmap);
    dvb(0 <= bit);

    unsigned char *ubm = (unsigned char*)bitmap;
    return BIT_CHECK(THE_BYTE(ubm, bit), POS(bit));
}
// because in literatur, low byte is on right,
// but for array, low byte is on left, this is NOT converation for reading.
// for example:
// \x12\x34 = 0001 0010 0011 0100
// we think \x34 is low bit, if you use this initial a bitmap, such as
// memcpy(bitmap, "\x12\x34", 2)
// the bitmap think bit is
//      0011 0100 0001 0010
// 0x12 became low byte.
// below function is adjust this conversation.
void bitmap_init(char *dst, const char *src, int size)
{
    dvb(NULL != dst);
    dvb(NULL != src);
    dvb(0 <= size);

    int i = 0;
    const char *s = src + size - 1;
    for (; i < size; i++, dst++, s--)
        *dst = *s;
    return;
}
unsigned int bitmap_bit1_sum(const char *bitmap, int size)
{
    dvb(NULL != bitmap);
    dvb(0 <= size);

    unsigned int cnt = 0;
    const char *b = bitmap;
    int i = 0;
    for (; i < size; i++, b++) {
        if (*b)
            cnt += Bit1Num((unsigned char)*b);
    }

    return cnt;
}



// 先打印高bit。例如，一个16bit
// bit15 bit14 ... bit1 bit0
int snprintf_bitmap_style(char *buf, int buf_size, const char *bitmap, int size)
{
	dvb((NULL == buf && 0 == buf_size) || (NULL != buf && 0 <= buf_size));
	dvb(0 <= buf_size);
	dvb(NULL != bitmap);
	dvb(0 <= size);
	if (0 == size)
	{
		if (NULL != buf && buf_size > 0)
		{
			buf[0] = '\0';
		}
		return 0;
	}
	int cnt = 0;
	int buf_room = 0;
	char *buf_p = NULL;
	int i = 0;
	int bit_num = size * 8;
	for (i = 0; i < bit_num; i++)
	{
		buf_room = calc_buf_room(buf_size, cnt);
		buf_p = calc_buf_p(buf, buf_size, cnt);
		const char *value = bitmap_check(bitmap, bit_num - i - 1) ? "1" : "0";
		const char *fmt = (0 == i % 4) ? " %s" : "%s";
		cnt += snprintf(buf_p, buf_room, fmt, value);
	}
	return cnt;
}

//}}}



//{{{bitmapr
// reverse bitmap, highest byte at index 0
// Index to byte.
#define IX_R(byte, bit)         (byte - ((bit) / 8) - 1)
// Specific bit in byte.
#define POS_R(bit)              ((bit) % 8)
//
#define THE_BYTE_R(bitmap, byte, bit)   (bitmap[IX_R(byte, bit)])
void bitmapr_set_value(char *bitmap, int byte, int bit, bool v)
{
    dvb(NULL != bitmap);
    dvb(0 < byte);
    dvb(0 <= bit);
    dvb(bit < byte * 8);

    unsigned char *ubm = (unsigned char*)bitmap;
    THE_BYTE_R(ubm, byte, bit) = BIT_SET_VALUE(THE_BYTE_R(ubm, byte, bit), POS_R(bit), v);
}
void bitmapr_set(char *bitmap, int byte, int bit)
{
    dvb(NULL != bitmap);
    dvb(0 < byte);
    dvb(0 <= bit);
    dvb(bit < byte * 8);

    unsigned char *ubm = (unsigned char*)bitmap;
    THE_BYTE_R(ubm, byte, bit) = BIT_SET(THE_BYTE_R(ubm, byte, bit), POS_R(bit));
}
void bitmapr_clear(char *bitmap, int byte, int bit)
{
    dvb(NULL != bitmap);
    dvb(0 < byte);
    dvb(0 <= bit);
    dvb(bit < byte * 8);

    unsigned char *ubm = (unsigned char*)bitmap;
    THE_BYTE_R(ubm, byte, bit) = BIT_CLEAR(THE_BYTE_R(ubm, byte, bit), POS_R(bit));
}
void bitmapr_toggle(char *bitmap, int byte, int bit)
{
    dvb(NULL != bitmap);
    dvb(0 < byte);
    dvb(0 <= bit);
    dvb(bit < byte * 8);

    unsigned char *ubm = (unsigned char*)bitmap;
    THE_BYTE_R(ubm, byte, bit) = BIT_TOGGLE(THE_BYTE_R(ubm, byte, bit), POS_R(bit));
}
bool bitmapr_check(const char *bitmap, int byte, int bit)
{
    dvb(NULL != bitmap);
    dvb(0 < byte);
    dvb(0 <= bit);
    dvb(bit < byte * 8);

    unsigned char *ubm = (unsigned char*)bitmap;
    return BIT_CHECK(THE_BYTE_R(ubm, byte, bit), POS_R(bit));
}
int bitmapr_bit1_sum(const char *bitmap, int byte, int bit_num)
{
    dvb(NULL != bitmap);
    dvb(0 < byte);
    dvb(0 <= bit_num);

    int cnt = 0;
    int i = 0;
    for (; i < bit_num; i++)
        if (bitmapr_check(bitmap, byte, i))
            cnt++;

    return cnt;
}
// 数组src中，找到bit置位的ix，从start_check_ix(include)开始检查
// if no more bit is set, return -1.
int bitmapr_next_set_bit_ix(const char *bitmap, int byte, int start_check_ix)
{
    dvb(NULL != bitmap);
    const int total_bit = byte * 8;
    dvb(0 <= start_check_ix);

    if (total_bit <= start_check_ix)
        return -1;

    int i;
    for (i=start_check_ix; i<total_bit; i++)
        if (bitmapr_check(bitmap, byte, i))
            return i;

    return -1;
}
//}}}



//{{{ reverse
static const unsigned char kBitReverseTable256[] = 
{
	0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
	0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
	0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
	0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
	0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
	0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
	0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
	0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
	0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
	0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
	0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
	0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
	0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
	0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
	0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
	0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};
uint8_t BitReverseUint8(uint8_t v)
{
	return kBitReverseTable256[v];
}
uint32_t BitReverseUint32(uint32_t v)
{
	const uint32_t c = (kBitReverseTable256[v & 0xff] << 24) | 
		(kBitReverseTable256[(v >> 8) & 0xff] << 16) | 
		(kBitReverseTable256[(v >> 16) & 0xff] << 8) |
		(kBitReverseTable256[(v >> 24) & 0xff]);
	return c;
#if 0
	// Option 2:
	unsigned char * p = (unsigned char *) &v;
	unsigned char * q = (unsigned char *) &c;
	q[3] = BitReverseTable256[p[0]]; 
	q[2] = BitReverseTable256[p[1]]; 
	q[1] = BitReverseTable256[p[2]]; 
	q[0] = BitReverseTable256[p[3]];
#endif
}
//}}}



//{{{ other
cp_t EqualBool(bool lhs, bool rhs)
{
    if (lhs && rhs)
        return 0;
    if (!(lhs) && !(rhs))
        return 0;
    return cph;
}
//}}}




