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

20130514------------------------------------------------------------------------
dodo:
        display a string, print '.' if char is NOT printable.

20120712------------------------------------------------------------------------
first version
*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>      // std::setw
#include <sys/time.h>	// gettimeofday()
#include <time.h>

//#include "xlog.h"
//#include "qcp.h"
#include "qstr.h"
#include "qstr.xcp.h"
//FILEID_REGISTER();
//#define this_file_id    0xa35a57e3   // echo -n qstr.c | rhash --simple -


//{{{ str
bool ZeroExistInStr(const char *str, int buf_size)
{
    dvb(NULL != str);
    dvb(0 <= buf_size);

    const char *p = str;
    for (int i = 0; i < buf_size; i++, p++)
    {
        if (*p == '\0')
            return true;
    }

    return false;
}
int StrLen(const char *str, int buf_size)
{
	dvb(NULL != str);
	dvb(0 <= buf_size);

	const char *p = str;
	int len = 0;
    for (int i = 0; i < buf_size; i++, p++)
    {
        if (*p == '\0')
            return i;
    }
	
	return -1;
}
// buf_size 不是字符串的长度，而是存放字符串缓冲区的字节数
cp_t StrValid(const char *str, int buf_size)
{
    if (0 == buf_size)
        return 0;

    ifbr(NULL != str);
    ifbr(0 < buf_size);
    ifbr(ZeroExistInStr(str, buf_size));
    return 0;
}
//}}}



//{{{ mem->hex_str
cp_t HexStrValid(const std::string &hex)
{
	ifbr(hex.find_first_not_of("0123456789abcdefABCDEF", 0) == std::string::npos);
	return 0;
}
cp_t HexStrValid(const char *hex, int size)
{
	const std::string str(hex, size);
	ifer(HexStrValid(str));
	return 0;
}
static const char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
void HexChar(char out[2], unsigned char ch)
{
    out[0] = hexmap[(ch & 0xf0) >> 4];
    out[1] = hexmap[ch & 0x0f];
}
static void HexCharToString(std::string &s, int ix, unsigned char ch)
{
    s[ix] = hexmap[(ch & 0xf0) >> 4];
    s[ix + 1] = hexmap[ch & 0x0f];
}
std::string HexStr(const void *data, int size)
{
    if (NULL == data)
        return "";

    std::string s(size * 2, ' ');
    for (int i = 0; i < size; ++i)
    {
        const unsigned char ch = ((const unsigned char *)data)[i];
        HexCharToString(s, 2 * i, ch);
        //s[2 * i]     = hexmap[(ch & 0xf0) >> 4];
        //s[2 * i + 1] = hexmap[ch & 0x0f];
    }

    return s;
}
std::string HexStr(uint32_t value)
{
    const int size = sizeof(uint32_t);
    const int loop = size * 2;
    std::string s(loop, ' ');
    for (int i = 0; i < size; i++)
    {
        const int shift_bit = 24 - 8 * (i);
        const unsigned char ch = (value >> shift_bit) & 0xff;
        HexCharToString(s, 2 * i, ch);
    }

    return s;
}
std::string HexStr(const std::string &src)
{
	return HexStr(src.data(), src.size());
}
std::string MemAndSizeToHexStr(const void *mem, int size)
{
    std::ostringstream ostr;
	ostr << "[" << size << "|" << (NULL ? "" : HexStr(mem, size)) << "]";
    return ostr.str();
}
//}}}



//{{{ hex_str -> mem
cp_t HexChValid(char ch)
{
	if ('0' <= ch && ch <= '9')
	{
		return 0;
	}
	if ('a' <= ch && ch <= 'f')
	{
		return 0;
	}
	if ('A' <= ch && ch <= 'F')
	{
		return 0;
	}

	return cph;
}
// '1' -> 0x1
// 'a' -> 0xa
// 'f' -> 0xf
// 错误： -1
char HexChToN(char ch)
{
	if ('0' <= ch && ch <= '9')
	{
		return ch - '0';
	}
	if ('a' <= ch && ch <= 'f')
	{
		return 0x0a + (ch - 'a');
	}
	if ('A' <= ch && ch <= 'F')
	{
		return 0x0a + (ch - 'A');
	}

	return 0xff;
}
// "1234ab" -> \x12\x34\xab
// "1234a" -> \x12\x34\xa0		<-- hex_str长度不是2的倍数，最后一个数，放在结果的高4位，低4位为0，这样处理，没有失败的情况。
// ref: /home/hl/cm/app/lib/qos/qstr.c
// 如果长度不是2的倍数，则最后一个hex_str char，放在最后一个数的高4位，低4位为0
std::string HexStr2Mem(const std::string &hex_str)
{
	const int src_size = hex_str.size();
	std::string dst;
	if (0 == src_size)
		return dst;

	// 先处理第1个字符，避免程序中需要记忆上一次的高4位内容
	const char n0_high = HexChToN(hex_str[0]);
	dst += (n0_high << 4);

	//ifbr(0 == src_size % 2);
	for (int i = 1; i < src_size; i++) 
	{
		const char ch = hex_str[i];
		const char n = HexChToN(ch);
		if (0 == i % 2)
		{
			dst += (n << 4);
		}
		else
		{
			// In C++03, std::string::back is not available due to an oversight, but you can get around this by dereferencing the reverse_iterator you get back from rbegin: char ch = *myStr.rbegin();
			//const char last_high = dst.back();
			const char last_high = *dst.rbegin();
			const char composite = (char)(last_high + n);
			//dst.back() = composite;
			*dst.rbegin() = composite;
		}
	}

	return dst;
}
//}}}



//{{{ string vector
std::ostream & operator<<(std::ostream &os, const std::vector<std::string> &string_vector)
{
	int i = 0;
    for (std::vector<std::string>::const_iterator it = string_vector.begin(); it != string_vector.end(); it++, i++)
    {
        const std::string &s = *it;
		os << std::left <<std::setfill(' ') << std::setw(4) << i << s.size() << "," << HexStr(s) << std::endl;
    }

	return os;
}
//}}}



//{{{ split
// ref: How to split a string in C++
std::vector<std::string> StrSplit(const std::string &s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}
//}}}


//{{{ string extend
std::string &StringReplace(std::string &str, int offset, int size, const void *mem)
{
	const int expect_s = offset + size;		// 期望的长度
	const int s = str.size();
	// 如果期望的长度 < 实际的长度，调整
	if (s < expect_s)
	{
		const int append_s = expect_s - s;	// 需要在尾部追加的长度
		dvb(0 < append_s);
		str.append(append_s, '\0');
	}

	dvb(expect_s <= str.size());
	// 从offset开始，替换掉长度为size的内容为mem
	const std::string replace((const char *)mem, size);
	str.replace(offset, size, replace);	
	return str;
}
std::string &StringReplace(std::string &str, int offset, const std::string &mem)
{
	return StringReplace(str, offset, mem.size(), mem.data());
}
//}}}


//{{{ c_style
#define MIN(X,Y)        (((X) < (Y)) ? (X) : (Y))



// 计算，如果用sprintf()来格式化字符串，需要多少字节，包括结尾'\0'
// 字符串打印出来后，字节个数，包括结尾处的'\0'
int sprintf_size(const char *fmt, ...)
{
	dvb(NULL != fmt);
	va_list argptr;
	va_start(argptr, fmt);
	const int len = vsnprintf(NULL, 0, fmt, argptr);
	// snprintf()帮助上说：return 0, 表示错误，
	// 实际情况：snprintf("%s", ""), 就返回0
	va_end(argptr);
	dvb(0 <= len);
	return (len + 1);
}
int vsprintf_size(const char *fmt, va_list ap)
{
	dvb(NULL != fmt);
	const int len = vsnprintf(NULL, 0, fmt, ap);
	dvb(0 <= len);
	return (len + 1);
}




// 如果要打印一个字符串居中，前面补的空格数
// str          要打印的字符串
// line_len     a line char num that can print
// ____xxxxx____
int str_middle_insert_blank(const char *str, int line_len)
{
	dvb(NULL != str);
	dvb(0 < line_len);
	const int len = strlen(str);
	if (len >= line_len)
	{
		return 0;
	}
	const int total_blank_len = line_len - len;
	const int half_blank_len = total_blank_len / 2;
	return half_blank_len;
}


// 0x09 -> '9'
// 0x0a -> 'a'
static inline char n_to_ascii(int n)
{
	dvb(0x0f >= n);
	dvb(0x00 <= n);
	// return n + '0' + (7 * (n / 10));     // big CASE
	// return n + '0' + (39 * (n / 10));       // small case
	return (n > 0x09) ? (n - 10 + 'a') : (n + '0');
}
// return: byte num write in buf
static inline int hexify_ascii(char *buf, char c)
{
	dvb(NULL != buf);
	unsigned char uc = c;
	int high = uc >> 4;
	int low = uc & 0xF;
	*buf       = n_to_ascii(high);
	*(buf + 1) = n_to_ascii(low);
	return 2;
}
int to_hex_mem_len(int n, const char *separator)
{
	dvb(0 <= n);
	dvb(NULL != separator);
	int len = strlen(separator);
	if (0 == n)
	{
		return 0;
	}
	if (1 == n)
	{
		return 2;
	}
	return 2 * n + len * (n - 1);
}
// It is more efficent than sprintf("%02x") method.
// to_hex_mem(hex, "\x12\xab", 2, ""): "\x12\xab" -> "12ab"
// to_hex_mem(hex, "\x12\xab", 2, ","): "\x12\xab" -> "12,ab"
// to_hex_mem(hex, "\x12\xab", 2, ", "): "\x12\xab" -> "12, ab"
// return: byte num write in hex
// Note: There is no '\0' at the end.
int to_hex_mem(char *hex, const char *src, int src_len, const char *separator)
{
	dvb(NULL != hex);
	dvb(NULL != src);
	dvb(0 <= src_len);
	dvb(NULL != separator);
	if (0 >= src_len)
	{
		return 0;
	}
	int i;
	char *p = hex;
	const int s_len = strlen(separator);
	for (i = 0; i < src_len - 1; ++i, ++src)
	{
		p += hexify_ascii(p, *src);
		memcpy(p, separator, s_len);
		p += s_len;
	}
	// the last char, NOT append the separator
	p += hexify_ascii(p, *src);
	const int hex_len = p - hex;
	dvb(hex_len == to_hex_mem_len(src_len, separator));
	return hex_len;
}




int str_to_bin_str_size(const char *bytes, int byte_len, const char *separator)
{
	dvb(NULL != bytes);
	dvb(0 <= byte_len);
	dvb(NULL != separator);
	if (0 == byte_len)
	{
		return 1;
	}
	const int sep_len = strlen(separator);
	const int need_bin_str_len = byte_len * 8 + (byte_len - 1) * sep_len;
	return need_bin_str_len + 1;
}
static void ch_to_bin_str_byte(char *bin_str, char ch)
{
	dvb(NULL != bin_str);
	*bin_str = '\0';
	sprintf(bin_str, "%c%c%c%c%c%c%c%c",
			(ch & 0x80) ? '1' : '0',
			(ch & 0x40) ? '1' : '0',
			(ch & 0x20) ? '1' : '0',
			(ch & 0x10) ? '1' : '0',
			(ch & 0x08) ? '1' : '0',
			(ch & 0x04) ? '1' : '0',
			(ch & 0x02) ? '1' : '0',
			(ch & 0x01) ? '1' : '0');
	return;
}
// 需要调用者保证bin_str size >= str_to_bin_str_size()
cp_t str_to_bin_str(char *bin_str, const char *bytes, int byte_len, const char *separator)
{
	dvb(NULL != bin_str);
	dvb(NULL != bytes);
	dvb(0 <= byte_len);
	dvb(NULL != separator);
	*bin_str = '\0';
	if (byte_len <= 0)
	{
		return 0;
	}
	char *dst = bin_str;
	const char *src = bytes;
	ch_to_bin_str_byte(dst, *src);
	src++;
	char buffer[9] = "";
	for (int i = 1; i < byte_len; i++, src++)
	{
		strcat(dst, separator);
		ch_to_bin_str_byte(buffer, *src);
		strncat(dst, buffer, 8);
	}
	return 0;
}



// return: output byte num
static int printf_a_hex(char n)
{
	qos_printf("%02x", (unsigned char)n);
	return 2;
}
// return: output byte num
int printf_hex(const char *src, int nbyte, const char *separator)
{
	if (0 == nbyte)
	{
		return 0;
	}
	if (1 == nbyte)
	{
		return printf_a_hex(*src);
	}
	int i;
	int cnt = 0;
	for (i = 0; i < nbyte - 1; i++, src++)
	{
		cnt += printf_a_hex(*src);
		//cnt += qos_printf("%s", separator);
		qos_printf("%s", separator);
		cnt += strlen(separator);
	}
	cnt += printf_a_hex(*src);
	return cnt;
}
void printf_hex_ex(const char *title, const char *tail, const char *src, int nbyte, const char *separator)
{
	qos_printf("%s(%d)[", title, nbyte);
	printf_hex(src, nbyte, separator);
	qos_printf("]%s", tail);
	return;
}
// return: output byte num
int printf_section_hex(const char *prompt,
					   const char *section, int section_len,
					   const char *separator
					  )
{
	int cnt = 0;
	// cnt += qos_printf("%s", prompt);
	qos_printf("%s", prompt);
	cnt += strlen(prompt);
	cnt += printf_hex(section, section_len, separator);
	//cnt += qos_printf("(len=%u)", section_len);
	return cnt;
}






int calc_buf_room(int buf_size, int cnt)
{
	return (buf_size > cnt) ? (buf_size - cnt) : 0;
}
char *calc_buf_p(char *buf, int buf_size, int cnt)
{
	int buf_room = calc_buf_room(buf_size, cnt);
	int offset = buf_size - buf_room;
	char *p = (buf != NULL) ? (buf + offset) : NULL;
	return p;
}
static int bytes_added(int result_of_sprintf)
{
	return (result_of_sprintf > 0) ? result_of_sprintf : 0;
}



// unknow char printf as '.'
int snprintf_dot_style(char *buf, int buf_size, const char *mem, int mem_size)
{
	dvb((NULL == buf && 0 == buf_size) || (NULL != buf && 0 <= buf_size));
	dvb(buf_size >= 0);
	dvb(NULL != mem);
	dvb(mem_size >= 0);
	if (0 == mem_size)
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
	int i;
	for (i = 0; i < mem_size; i++, mem++)
	{
		buf_room = calc_buf_room(buf_size, cnt);
		buf_p = calc_buf_p(buf, buf_size, cnt);
		if (printable_char(*mem))
		{
			cnt += snprintf(buf_p, buf_room, "%c", *mem);
		}
		else
		{
			cnt += snprintf(buf_p, buf_room, "%s", ".");
		}
	}
	return cnt;
}
// return: output byte num
static int snprintf_a_hex(char *buf, int buf_size, char n)
{
	int outn = snprintf(buf, buf_size, "%02x", (unsigned char)n);
	dvb(2 == outn);
	return outn;
}
int snprintf_hex_style(char *buf, int buf_size, const char *mem, int mem_size, const char *separator)
{
	dvb((NULL == buf && 0 == buf_size) || (NULL != buf && 0 <= buf_size));
	dvb(0 <= buf_size);
	dvb(NULL != mem);
	dvb(0 <= mem_size);
	dvb(NULL != separator);
	if (0 == mem_size)
	{
		if (NULL != buf && buf_size > 0)
		{
			buf[0] = '\0';
		}
		return 0;
	}
	if (1 == mem_size)
	{
		return snprintf_a_hex(buf, buf_size, *mem);
	}
	int cnt = 0;
	int buf_room = 0;
	char *buf_p = buf;
	int i;
	for (i = 0; i < mem_size - 1; i++, mem++)
	{
		buf_room = calc_buf_room(buf_size, cnt);
		buf_p = calc_buf_p(buf, buf_size, cnt);
		cnt += snprintf_a_hex(buf_p, buf_room, *mem);
		buf_room = calc_buf_room(buf_size, cnt);
		buf_p = calc_buf_p(buf, buf_size, cnt);
		cnt += snprintf(buf_p, buf_room, "%s", separator);
	}
	buf_room = calc_buf_room(buf_size, cnt);
	buf_p = calc_buf_p(buf, buf_size, cnt);
	cnt += snprintf_a_hex(buf_p, buf_room, *mem);
	return cnt;
}
int snprintf_hex_style_blank_separator(char *buf, int buf_size, const char *mem, int mem_size)
{
	return snprintf_hex_style(buf, buf_size, mem, mem_size, " ");
}
int snprintf_hex_style_no_separator(char *buf, int buf_size, const char *mem, int mem_size)
{
	return snprintf_hex_style(buf, buf_size, mem, mem_size, "");
}
// sprintf c style to buf, write '\0' to end.
// return: write to buf byte num. NOT include end of '\0'
// note: buf size should be return value +1.
int snprintf_c_style(char *buf, int buf_size, const char *mem, int mem_size)
{
	dvb((NULL == buf && 0 == buf_size) || (NULL != buf && 0 <= buf_size));
	dvb(0 <= buf_size);
	dvb(NULL != mem);
	dvb(0 <= mem_size);
	if (0 == mem_size)
	{
		if (NULL != buf && buf_size > 0)
		{
			buf[0] = '\0';
		}
		return 0;
	}
	int cnt = 0;
	char *buf_p = NULL;
	int buf_room = 0;
	int i;
	for (i = 0; i < mem_size; i++, mem++)
	{
		buf_p = calc_buf_p(buf, buf_size, cnt);
		buf_room = calc_buf_room(buf_size, cnt);
		unsigned char uch = *mem;
		if (printable_char(uch))
		{
			const char *fmt = "%c";
			cnt += snprintf(buf_p, buf_room, fmt, uch);
		}
		else
		{
			const char *fmt = "\\x%02x";
			cnt += snprintf(buf_p, buf_room, fmt, uch);
		}
	}
	return cnt;
}


// such as: "ab\x01\x02cd"
int printf_c_style(const char *str, int len)
{
	int cnt = 0;
	int i;
	for (i = 0; i < len; i++, str++)
	{
		if (printable_char(*str))
		{
			// cnt += qos_printf("%c", *str);
			qos_printf("%c", *str);
			cnt += 1;
		}
		else
		{
			//cnt += qos_printf("\\x%02x", *str);
			cnt += 4;
		}
	}
	return cnt;
}



int snprintf_style(char *buf, int buf_size, const char *mem, int mem_size,
				   const char *style_descript_left[],
				   const char *style_descript_right[],
				   const snprintf_style_t style_fun_list_null_end[]
				  )
{
	dvb(NULL != mem);
	dvb(0 <= mem_size);
	dvb(NULL != style_descript_left);
	dvb(NULL != style_descript_right);
	dvb(NULL != style_fun_list_null_end);
	int cnt = 0;
	int buf_room = 0;
	char *buf_p = NULL;
	const char **sdl = style_descript_left;
	const char **sdr = style_descript_right;
	const snprintf_style_t *p = style_fun_list_null_end;
	for (; *p; p++, sdl++, sdr++)
	{
		dvb(NULL != *sdl);
		dvb(NULL != *sdr);
		buf_p = calc_buf_p(buf, buf_size, cnt);
		buf_room = calc_buf_room(buf_size, cnt);
		cnt += snprintf(buf_p, buf_room, "%s", *sdl);
		buf_p = calc_buf_p(buf, buf_size, cnt);
		buf_room = calc_buf_room(buf_size, cnt);
		cnt += (*p)(buf_p, buf_room, mem, mem_size);
		buf_p = calc_buf_p(buf, buf_size, cnt);
		buf_room = calc_buf_room(buf_size, cnt);
		cnt += snprintf(buf_p, buf_room, "%s", *sdr);
	}
	dvb(NULL == *p);
	dvb(NULL == *sdl);
	dvb(NULL == *sdr);
	return cnt;
}
int snprintf_style3(char *buf, int buf_size, const char *mem, int mem_size)
{
	dvb(NULL != mem);
	dvb(0 <= mem_size);
	const char *style_descript_left[] =
	{
		"\t[dot_style:",
		"\t[  c_style:",
		"\t[hex_style:",
		NULL
	};
	const char *style_descript_right[] =
	{
		"]\n",
		"]\n",
		"]",
		NULL
	};
	const snprintf_style_t style_fun_list_null_end[] =
	{
		snprintf_dot_style,
		snprintf_c_style,
		snprintf_hex_style_no_separator,
		NULL
	};
	return snprintf_style(buf, buf_size, mem, mem_size,
						  style_descript_left,
						  style_descript_right,
						  style_fun_list_null_end
						 );
}
int snprintf_style_default(char *buf, int buf_size, const char *mem, int mem_size)
{
	dvb(NULL != mem);
	dvb(0 <= mem_size);
	const char *style_descript_left[] =
	{
		"[hex_style:",
		NULL
	};
	const char *style_descript_right[] =
	{
		"]",
		NULL
	};
	const snprintf_style_t style_fun_list_null_end[] =
	{
		snprintf_hex_style_no_separator,
		NULL
	};
	return snprintf_style(buf, buf_size, mem, mem_size,
						  style_descript_left,
						  style_descript_right,
						  style_fun_list_null_end
						 );
}




// return: output byte num, not include end of '\0'
int snprintf_section_hex(char *buf, int buf_size,
						 const char *prompt,
						 const char *section, int section_len, const char *separator)
{
	int cnt = 0;
	int buf_room = 0;
	char *buf_p = NULL;
	buf_room = calc_buf_room(buf_size, cnt);
	buf_p = calc_buf_p(buf, buf_size, cnt);
	cnt += bytes_added(snprintf(buf_p, buf_room, "%s", prompt));
	buf_room = calc_buf_room(buf_size, cnt);
	buf_p = calc_buf_p(buf, buf_size, cnt);
	cnt += bytes_added(snprintf_hex_style(buf_p, buf_room, section, section_len, separator));
	buf_room = calc_buf_room(buf_size, cnt);
	buf_p = calc_buf_p(buf, buf_size, cnt);
	cnt += bytes_added(snprintf(buf_p, buf_room, "(len=%u)", section_len));
	return cnt;
}






// prompt: xxxxx(len=5)
// return: output byte num
int printf_section(const char *prompt, const char *section, int section_len)
{
	int cnt = 0;
	// cnt += qos_printf("%s", prompt);
	qos_printf("%s", prompt);
	cnt += strlen(prompt);
	int i;
	for (i = 0; i < section_len; i++)
	{
		//cnt += qos_printf("%c", section[i]);
		qos_printf("%c", section[i]);
		cnt += 1;
	}
	// cnt += qos_printf("(len=%u)\n", section_len);
	char buf[30];
	cnt += snprintf(buf, sizeof(buf) - 1, "(len=%u)\n", section_len);
	buf[sizeof(buf) - 1] = '\0';
	qos_printf("%s", buf);
	return cnt;
}
// return: output byte num, not include end of '\0'
int snprintf_section(char *buf, int buf_size, const char *prompt, const char *section, int section_len)
{
	int cnt = 0;
	int buf_room = 0;
	char *p = buf;
	buf_room = (buf_size >= cnt) ? (buf_size - cnt) : 0;
	p = (buf != NULL) ? (buf + buf_room) : NULL;
	cnt += bytes_added(snprintf(p, buf_room, "%s", prompt));
	int i;
	for (i = 0; i < section_len; i++)
	{
		buf_room = (buf_size >= cnt) ? (buf_size - cnt) : 0;
		p = (buf != NULL) ? (buf + buf_room) : NULL;
		cnt += bytes_added(snprintf(p, buf_room, "%c", section[i]));
	}
	buf_room = (buf_size >= cnt) ? (buf_size - cnt) : 0;
	p = (buf != NULL) ? (buf + buf_room) : NULL;
	cnt += bytes_added(snprintf(p, buf_room, "(len=%u)\n", section_len));
	return cnt;
}





int strsize(const char *str)
{
	dvb(NULL != str);
	return strlen(str) + 1;
}



// this function will modify the str, and return pointer is the first char which
// is NOT blank.
// return: trimed string head pointer. NEVER return NULL.
char *strtrim(char *str)
{
	dvb(NULL != str);
	char *head = str;
	while (isspace(*head))
	{
		head++;
	}
	strtrim_right(head);
	return head;
}
char *strtrim_right(char *str)
{
	dvb(NULL != str);
	int len = strlen(str);
	if (0 >= len)
	{
		return str;
	}
	char *p = str + len - 1;
	while (isspace(*p) && (p > str))
	{
		p--;
	}
	dvb((p >= str) && (p <= str + len - 1));
	if (isspace(*p))
	{
		dvb(p == str);
		*p = '\0';
	}
	else
	{
		p++;
		*p = '\0';
	}
	return str;
}




// dst中原为字符串，本函数尽量将src append在dst尾部，并判断append是否成功。
// 保证操作不会超过dst_size，而且，一定会在dst后添加'\0'
// return: 是否src完全append进dst中
bool append_str(char *dst, int dst_size, const char* src)
{
	const int dst_len = strlen(dst);
	const int src_len = strlen(src);
	char* const p = dst + dst_len;            // 指向要拷贝的起始地址
	const int buff_len_for_copy = dst_size - dst_len;
	dvb(NULL != dst);
	dvb(NULL != src);
	dvb(0 != dst_size);
	if ((dst_len + 1) > dst_size)
	{
		// 如果dst字符串超过了缓冲区长度，认为系统发生错误，
		// 按照缓冲区长度值进行截断。
		dst[dst_size - 1] = '\0';
		return false;
	}
	str_to_str(p, buff_len_for_copy, src);
	return (size_t)(dst_len + src_len) == strlen(dst);
}




// 将一段内存，变成字符串，dst结尾处，一定是'\0'结尾
// return: 写到str中的字节数，不包括结尾'\0'
int mem_to_str(char *str, int str_size, const void *mem, int n)
{
	dvb(NULL != str);
	dvb(NULL != mem);
	// 如果小于1，无法容纳'\0'，函数返回时，str不是一个合法字符串
	if (1 > str_size)
	{
		dvb(false);
		return 0;
	}
	const int str_byte_num = str_size - 1;
	const int copy_byte_num = MIN(str_byte_num, n);
	memcpy(str, mem, copy_byte_num);
	str[copy_byte_num] = '\0';
	return copy_byte_num;
}






// 将字符串，拷贝到一段内容中，不拷贝'\0'
// return: 写到mem中的字节数
int str_to_mem(void *mem, int mem_size, const char *str)
{
	dvb(NULL != mem);
	dvb(0 <= mem_size);
	dvb(NULL != str);
	const int str_len = strlen(str);
	const int copy_byte_num = MIN(mem_size, str_len);
	memcpy(mem, str, copy_byte_num);
	return copy_byte_num;
}



// len not include end_char
// if end_char is NOT exist in mem, the length is size
// such as: memlen("abc   ", ' ') == 3
int memlen(const char *mem, int size, char end_char)
{
	dvb(NULL != mem);
	dvb(0 <= size);
	int i = 0;
	for (; i < size; i++, mem++)
	{
		if (*mem == end_char)
		{
			return i;
		}
	}
	return size;
}



// copy n char from src to dst, and add '\0'
// dst_size         目标缓冲区长度
// 如果想完全拷贝n个字节的字符串，要求dst_size >= n+1
// return: all src byte copy into dst
// 无论如何，dst string都会在末尾加'\0'，构建成完整的字符串；而且，写入dst的字节数，不超过dst_size
bool str_to_str(char *dst, int dst_size, const char *src)
{
	dvb(NULL != dst);
	dvb(1 <= dst_size);
	dvb(NULL != src);
	if (1 > dst_size)
	{
		return false;
	}
	const int src_len = strlen(src);
	const int dst_len = dst_size - 1;
	const int copy_byte_num = MIN(src_len, dst_len);
	memcpy(dst, src, copy_byte_num);
	dst[copy_byte_num] = '\0';
	return (copy_byte_num == src_len);
}




// 尽量拷贝，如果目标空间不够，返回false
// return: dst_size >= src_size
bool mem_to_mem(void *dst, int dst_size, const void *src, int src_size)
{
	dvb(NULL != dst);
	dvb(0 <= dst_size);
	dvb(NULL != src);
	if (0 > src_size)
	{
		return false;
	}
	dvb(0 <= src_size);
	const int copy_byte_num = MIN(dst_size, src_size);
	memcpy(dst, src, copy_byte_num);
	if (copy_byte_num < src_size)
	{
		return false;
	}
	return true;
}



// 主要用于解析字符串
// return: src + byte_num
const void *memcpy_src_offset(void *dst, const void *src, int byte_num)
{
	memcpy(dst, (char*)src, byte_num);
	return ((char*)src + byte_num);
}
// 主要用于拷贝分离的字符串到缓冲区时用
// 拷贝src到dst，拷贝byte_num字节，返回dst+byte_num指针
// return: dst+byte_num
void *memcpy_dst_offset(void *dst, const void *src, int byte_num)
{
	memcpy(dst, src, byte_num);
	return ((char*)dst + byte_num);
}




// mem store float is: high byte first.
void *float_to_mem_dst_offset(void *dst, const float *src)
{
	dvb(NULL != dst);
	dvb(NULL != src);
	const int copyn = sizeof(float);
	dvb(4 == sizeof(float));
	const char *s = (char*)src;
	s += (copyn - 1);
	char *d = (char*)dst;
	int i;
	for (i = 0; i < copyn; i++, s--, d++)
	{
		*d = *s;
	}
	return d;
}
void *mem_to_float_src_offset(float *dst, const void *src)
{
	dvb(NULL != dst);
	dvb(NULL != src);
	const int copyn = sizeof(float);
	dvb(4 == sizeof(float));
	char *d = (char*)dst;
	d += (copyn - 1);
	const char *s = (char*)src;
	int i;
	for (i = 0; i < copyn; i++, d--, s++)
	{
		*d = *s;
	}
	return (void*)s;
}



// mem store int is: high byte first.
void *int_to_mem_dst_offset(void *dst, const int *src)
{
	dvb(NULL != dst);
	dvb(NULL != src);
	const int copyn = sizeof(int);
	dvb(4 == sizeof(int));
	const char *s = (char*)src;
	s += (copyn - 1);
	char *d = (char*)dst;
	int i;
	for (i = 0; i < copyn; i++, s--, d++)
	{
		*d = *s;
	}
	return d;
}
void *mem_to_int_src_offset(int *dst, const void *src)
{
	dvb(NULL != dst);
	dvb(NULL != src);
	const int copyn = sizeof(int);
	dvb(4 == sizeof(int));
	char *d = (char*)dst;
	d += (copyn - 1);
	const char *s = (char*)src;
	int i;
	for (i = 0; i < copyn; i++, d--, s++)
	{
		*d = *s;
	}
	return (void*)s;
}






// 将src字符串，最多拷贝dst_len个字节到dst中，如果dst多余字节，尾部填充fill字符。
// dst      目标字符串，并非以'\0'结尾
// src      源字符串
// fill     dst尾部填充的字节数
// return: dst_len是否足够容纳src所有字节，足够，返回true
// note:
// 1    如果dst_len，长度小于src字符串长度，只拷贝前dst_len字节。
// 2    dst & src can be same pointer
bool memcpy_fill(char *dst, int dst_len, const char *src, int src_len, char fill)
{
	dvb(NULL != dst);
	dvb(NULL != src);
	const int copy_len = MIN(dst_len, src_len);
	memcpy(dst, src, copy_len);
	// 长度不够
	if (copy_len < src_len)
	{
		return false;
	}
	// 长度刚好，无需填充尾部
	dvb(copy_len >= src_len);
	dvb(copy_len - src_len <= dst_len);
	dvb(copy_len <= dst_len);
	const int fill_len = dst_len - copy_len;
	memset(dst + copy_len, fill, fill_len);
	return true;
}





void switch_2_byte(char byte[2])
{
	const char b0 = byte[0];
	byte[0] = byte[1];
	byte[1] = b0;
}




int memcmp_nocase(const void *s1, const void *s2, size_t n)
{
	size_t i;
	const char *p1 = (char*)s1;
	const char *p2 = (char*)s2;
	for (i = 0; i < n; i++, p1++, p2++)
	{
		if (printable_char(*p1) && printable_char(*p2))
		{
			if (tolower(*p1) != tolower(*p2))
			{
				dvb(*p1 != *p2);
				return *p1 - *p2;
			}
		}
		else
		{
			if (*p1 != *p2)
			{
				return *p1 - *p2;
			}
		}
	}
	return 0;
}


// memcmp, give the diff_ix， other is compitate with standar memcmp()
// if diff_ix NULL, ignore
int memcmp_ex(const void *s1, const void *s2, int len, int *diff_ix)
{
	dvb(NULL != s1);
	dvb(NULL != s2);
	dvb(0 <= len);
	if (NULL == diff_ix)
	{
		return memcmp(s1, s2, len);
	}
	*diff_ix = -1;
	const unsigned char *p1 = (unsigned char *)s1;
	const unsigned char *p2 = (unsigned char *)s2;
	int r = 0;
	int i;
	for (i = 0; i < len; i++, p1++, p2++)
	{
		r = *p1 - *p2;
		if (0 != r)
		{
			dvb(NULL != diff_ix);
			*diff_ix = i;
			return r;
		}
	}
	return 0;
}



// "\x12\x34\x56\x78\x90"       ->      "\x90\x78\x56\x34\x12"
void mem_reverse(void *mem, int size)
{
	int i = 0;
	const int half = size / 2;
	char *left = (char*)mem;
	char *right = (char*)mem + size - 1;
	for (i = 0; i < half; i++, left++, right--)
	{
		const char c = *left;
		*left = *right;
		*right = c;
	}
	return;
}



// 查找字符
// return: index if find. -1 if NOT find.
int mem_find_ch(char ch, const char *mem, int size)
{
	dvb(NULL != mem);
	dvb(0 <= size);
	for (int i = 0; i < size; ++i, ++mem)
	{
		if (*mem == ch)
			return i;
	}
	return -1;
}


// return: true if all mem char is ch.
bool mem_is_same(char ch, const void *mem, int size)
{
	dvb(NULL != mem);
	dvb(0 < size);
	const char *p = (char*)mem;
	int i;
	for (i = 0; i < size; i++, p++)
	{
		if (*p != ch)
		{
			return false;
		}
	}
	return true;
}



// find needle in haystack
char *mem_in_mem(const char *haystack, int haystack_size, const char *needle, int needle_size)
{
	dvb(NULL != haystack);
	dvb(0 < haystack_size);
	dvb(NULL != needle);
	dvb(0 < needle_size);
	if (haystack_size < needle_size)
	{
		return NULL;
	}
	// left is already find space.
	// right is NOT find space.
	int right_size = haystack_size;
	const char *p = haystack;
	while (right_size >= needle_size)
	{
		if (0 == memcmp(p, needle, needle_size))
		{
			return (char*)p;
		}
		p++;
		right_size--;
	}
	return NULL;
}



// because isprint() cause Segmentation fault with stack address, so write mine.
bool printable_char(int c)
{
	// ' ' - '~'
	return (c >= ' ' && c <= '~');
	/*
	        if (c >= ' ' && c <= '/')
	                return true;
	        if (c >= '0' && c <= '9')
	                return true;
	        if (c >= ':' && c <= '@')
	                return true;
	        if (c >= 'A' && c <= 'Z')
	                return true;
	        if (c >= '[' && c <= '`')
	                return true;
	        if (c >= 'a' && c <= 'z')
	                return true;
	        if (c >= '{' && c <= '~')
	                return true;
	        return false;
	*/
}
bool printable_mem(const char *mem, int n)
{
	dvb(NULL != mem);
	dvb(0 < n);
	int i;
	for (i = 0; i < n; i++, mem++)
	{
		if (!printable_char(*mem))
		{
			return false;
		}
	}
	return true;
}
// return: str
char *str_to_lower(char *str)
{
	dvb(NULL != str);
	char *p;
	for (p = str; *p; p++)
	{
		dvb(printable_char(*p));
		*p = tolower(*p);
	}
	return str;
}
char *str_to_upper(char *str)
{
	dvb(NULL != str);
	char *p;
	for (p = str; *p; p++)
	{
		dvb(printable_char(*p));
		*p = toupper(*p);
	}
	return str;
}




bool digit_char(char ch)
{
	return !(0 == isdigit(ch));
}
bool digit_mem(const char *mem, int n)
{
	dvb(NULL != mem);
	dvb(0 <= n);
	int i;
	for (i = 0; i < n; i++, mem++)
	{
		if (!digit_char(*mem))
		{
			return false;
		}
	}
	return true;
}
// example:
// check month mem valid
// mem_digit_2_valid(mem, "01", "12");
#if 0
const char *mem_digit_2_valid(const char *mem, const char *low, const char *high)
{
	dvb(NULL != mem);
	dvb(NULL != low);
	dvb(NULL != high);
	// example: "01"(mem) < "02"(low)
	iff_rs(*mem >= *low, "*mem >= *low fail");
	if (*mem == *low)
	{
		iff_rs(*(mem + 1) >= *(low + 1), "*(mem + 1) >= *(low + 1) fail");
	}
	iff_rs(*mem <= *high, "*mem <= *high fail");
	if (*mem == *high)
	{
		iff_rs(*(mem + 1) <= *(high + 1), "*(mem + 1) <= *(high + 1) fail");
	}
	return NULL;
}
#endif
cp_t mem_digit_2_valid(const char *mem, const char *low, const char *high)
{
	dvb(NULL != mem);
	dvb(NULL != low);
	dvb(NULL != high);
	// example: "01"(mem) < "02"(low)
	// "*mem >= *low fail"
	ifbr(*mem >= *low);
	if (*mem == *low)
	{
		//  "*(mem + 1) >= *(low + 1) fail"
		ifbr(*(mem + 1) >= *(low + 1));
	}
	// "*mem <= *high fail"
	ifbr(*mem <= *high);
	if (*mem == *high)
	{
		// "*(mem + 1) <= *(high + 1) fail"
		ifbr(*(mem + 1) <= *(high + 1)); 
	}

	return 0;
}


cp_t hex_ch_valid(char ch)
{
	if ('0' <= ch && ch <= '9')
		return 0;
	if ('a' <= ch && ch <= 'f')
		return 0;
	if ('A' <= ch && ch <= 'F')
		return 0;
	return cph;
}
// '1' -> 0x1
// 'a' -> 0xa
// 'f' -> 0xf
// 错误： -1
int hex_ch_to_n(char ch)
{
	if ('0' <= ch && ch <= '9')
		return ch - '0';
	if ('a' <= ch && ch <= 'f')
		return 0x0a + (ch - 'a');
	if ('A' <= ch && ch <= 'F')
		return 0x0a + (ch - 'A');
	return -1;
}
// "1234ab" -> \x12\x34\xab
// return: cp
cp_t hex_mem_to_bin(void *bin, int bin_size, const char *hex, int hex_size)
{
	dvb(NULL != bin);
	dvb(0 < bin_size);
	dvb(NULL != hex);
	dvb(0 <= hex_size);
	ifbr(0 == hex_size % 2);
	ifbr(bin_size == hex_size / 2);
	const char *src = hex;
	char *dst = (char*)bin;
	int i = 0;
	for (; i < bin_size; i++, dst++, src += 2)
	{
		const char ch_high = src[0];
		const int high = hex_ch_to_n(ch_high);
		ifbr(0 <= high);
		dvb(high <= 0xf);
		const char ch_low = src[1];
		const int low = hex_ch_to_n(ch_low);
		ifbr(0 <= low);
		dvb(low <= 0xf);
		*dst = (char)((high << 4) + low);
	}
	return 0;
}





// below ok:
// "123"
// below fail:
// "123  "
// " 12"
// "1 23"
// "1""\x00""23"
const char *name_valid_with_error_descript(const char *name, int size,
		const char *should_not_be_empty,
		const char *must_be_printable_char,
		const char *blank_should_not_be_exist)
{
	dvb(NULL != name);
	dvb(0 <= size);
	dvb(NULL != should_not_be_empty);
	dvb(NULL != must_be_printable_char);
	dvb(NULL != blank_should_not_be_exist);
	// iff_rv(size > 0, should_not_be_empty);
	if (!(size > 0))
		return should_not_be_empty;

	//iff_rs(len <= max_size, "name length out of limit");
	const char *p = name;
	int i = 0;
	for (; i < size; i++, p++)
	{
		// iff_rv(printable_char(*p), must_be_printable_char);
		if (!printable_char(*p)) 
			return must_be_printable_char;
		if (' ' == *p)
			return blank_should_not_be_exist;
	}
	return NULL;
}
const char *name_valid(const char *name, int size)
{
	dvb(NULL != name);
	dvb(0 <= size);
	static const char *m_should_not_be_empty = "empty string can NOT be a name";
	static const char *m_must_be_printable_char = "only printable char is exist in name";
	static const char *m_blank_should_not_be_exist = "blank can NOT be exist in name";
	return name_valid_with_error_descript(name, size,
										  m_should_not_be_empty,
										  m_must_be_printable_char,
										  m_blank_should_not_be_exist
										 );
}
// below ok:
// "123"
// "123  "
// below fail:
// " 12"
// "1 23"
// "1""\x00""23"
// error_descript must be type of "static const char *".
const char *name_follow_blank_valid_with_error_descript(const char *name, int size,
		const char *should_not_be_empty,
		const char *must_be_printable_char,
		const char *blank_should_not_be_exist,

		const char *first_char_should_not_be_blank,
		const char *blank_should_not_be_at_middle
													   )
{
	dvb(NULL != name);
	dvb(0 <= size);
	dvb(NULL != should_not_be_empty);
	dvb(NULL != must_be_printable_char);
	dvb(NULL != blank_should_not_be_exist);
	dvb(NULL != first_char_should_not_be_blank);
	dvb(NULL != blank_should_not_be_at_middle);
	// iff_rv(size > 0, should_not_be_empty);
	if (!(size > 0))
		return should_not_be_empty;

	int ix = mem_find_ch(' ', name, size);
	// iff_rv(0 != ix, first_char_should_not_be_blank);
	if (0 == ix) 
		return first_char_should_not_be_blank;
	int name_size = (0 > ix) ? size : ix;
	dvb(name_size <= size);
	// r_if_not_null(name_valid_with_error_descript(name, name_size,
	// 			  should_not_be_empty,
	// 			  must_be_printable_char,
	// 			  blank_should_not_be_exist));
	const char *r = name_valid_with_error_descript(name, name_size,
	 			  should_not_be_empty,
	 			  must_be_printable_char,
	 			  blank_should_not_be_exist);
	if (NULL != r)
		return r;
	// check name follow with all are blanks
	dvb(0 != ix);
	if (0 < ix)
	{
		const char *blank_start = name + ix;
		int blank_size = size - ix;
		// iff_rv(mem_is_same(' ', blank_start, blank_size), blank_should_not_be_at_middle);
		if (!mem_is_same(' ', blank_start, blank_size)) 
			return blank_should_not_be_at_middle;
	}
	return NULL;
}
const char *name_follow_blank_valid(const char *name, int size)
{
	dvb(NULL != name);
	dvb(0 <= size);
	static const char *m_should_not_be_empty = "empty string can NOT be a name_follow_blank";
	static const char *m_must_be_printable_char = "only printable char is exist in name_follow_blank";
	static const char *m_blank_should_not_be_exist = "blank name can NOT be exist in name_follow_blank";
	static const char *m_first_char_should_not_be_blank = "first char should NOT be blank in name_follow_blank";
	static const char *m_blank_should_not_be_at_middle = "blank should not be at middle of name_follow_blank";
	return name_follow_blank_valid_with_error_descript(name, size,
			m_should_not_be_empty,
			m_must_be_printable_char,
			m_blank_should_not_be_exist,
			m_first_char_should_not_be_blank,
			m_blank_should_not_be_at_middle
													  );
}





#define CRYPT_TABLE_SIZE        0x500
static unsigned long m_crypt_table[CRYPT_TABLE_SIZE];
static bool m_crypt_table_init = false;
bool hash_crypt_table_init(void)
{
	if (m_crypt_table_init)
	{
		return true;
	}
	m_crypt_table_init = true;
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;
	for (index1 = 0; index1 < 0x100; index1++)
	{
		for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{
			unsigned long temp1, temp2;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);
			m_crypt_table[index2] = (temp1 | temp2);
		}
	}
	return true;
}
unsigned int hash_string(const char *string, unsigned int hash_type)
{
	dvb(m_crypt_table_init);
	dvb(NULL != string);
	const unsigned char *key = (const unsigned char *)string;
	unsigned int seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch;
	while (*key != 0)
	{
		ch = toupper(*key++);
		seed1 = m_crypt_table[(hash_type << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}


// 例如：char command_line[] = "./ut -t 3 -s 10 -g \"1 3\" -v";
// 其中" "中间的部分，当作一个整体
// 这个函数会改变str，返回的argv[]中指针，会指向str中拆分后的各个字符串
cp_t str_to_argc_argv(int *argcp, char *argv[], int max_args, char *str)
{
	dvb(NULL != argcp);
	dvb(NULL != argv);
	dvb(0 < max_args);
	dvb(NULL != str);
	int argc = 0;
	char *p2 = strtok(str, " ");
	while (p2)
	{
		if (argc >= max_args - 1)
		{
			// full
			dvb(false);
			return cph;
		}
		//qos_printf("p2=%s ", p2);
		char *next_str = p2 + strlen(p2) + 1;
		// 处理" "中间的部分，当作一个整体
		if (*next_str == '"')
		{
			argv[argc++] = p2;
			p2 = strtok(0, "\"");
			//qos_printf("p2_1=%s ", p2);
			//qos_printf("argc:%d argv:%s p2=%s ", argc-1, argv[argc-1], p2);
		}
		else
		{
			argv[argc++] = p2;
			p2 = strtok(0, " ");
		}
		//qos_printf("argc:%d argv:%s p2=%s\n", argc-1, argv[argc-1], p2);
	}
	argv[argc] = 0;
	*argcp = argc;
	return 0;
}
//}}}
