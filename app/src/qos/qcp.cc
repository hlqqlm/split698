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

20220303-----------------------------------------------------------------------
huanglin 创建.
实现code postion
*/
#include <stdio.h>
#include <math.h>
//#include <unistd.h>
#include <assert.h>
#include <sstream>
#include <string.h>

#include "qlibc.h"
#include "fileid.h"
#include "qcp.h"
#include "qcp.xcp.h"
//#define this_file_id 0x434f3189

//{{{ const
static const uint64_t kLow32bitMask = 0x00000000ffffffff;
//extern const FileidItem kFileidTable[];
//extern const int kFileidNum;
//extern const FileidTable kFileidTable;
extern const FileidTable *MainFileidTable(void);
//extern FileidTableCpp g_fileid_table;
//}}}


//{{{ split & assemble
/*
cp_t CodePos(uint32_t fileid, uint32_t line)
{   
	//const cp_t cp = (((cp_t)fileid) << 32) | line;
	//return cp;
	return CODE_POS(fileid, line);
}
*/
uint32_t CpFileid(cp_t cp)
{   
	const uint32_t fileid = (uint32_t)((cp >> 32) & kLow32bitMask);
	return fileid;
}
uint32_t CpLine(cp_t cp)
{
	const uint32_t line = (uint32_t)(cp & kLow32bitMask);
	return line;
}
//}}}


//{{{ fileid table
const char *CpFilenameInTable(cp_t cp)
{
	const FileidTable *ft = MainFileidTable();
	return FileidTableFilenameById(ft->table, ft->num, CpFileid(cp));
}
/*
#ifdef __cplusplus
static std::string CpFilenameInTable(const FileidTableCpp &ft, cp_t cp)
{
	return ft.FileIdName(CpFileId(cp));
}
std::string CpFilenameInTable(cp_t cp)
{
	return CpFilenameInTable(g_fileid_table, cp);
}
#else
const char *CpFilenameInTable(cp_t cp)
{
}
#endif
*/
//}}} 


//{{{ to_str
#if 1
std::ostream &CodePosOsStr(std::ostream &ostr, cp_t cp)
{
	if (0 == cp)
		ostr << "0";
	else
		ostr << "#" << CpLine(cp) << "@" << CpFilenameInTable(cp);
	return ostr;
}
std::string CodePosStr(cp_t cp)
{
	std::ostringstream ostr;
	CodePosOsStr(ostr, cp);
	return ostr.str();
}
std::string CpStr(cp_t cp)
{
	return CodePosStr(cp);
}
#endif
char *CodePosStr(char *buf, int buf_size, cp_t cp)
{
	if (buf_size <= 1)
	{
		qos_printf("ERROR: buf_size wrongs in CodePosStr(). buf_size=%d\n", buf_size);
		return NULL;
	}

	if (0 == cp)
		snprintf(buf, buf_size, "#0@0");
	else
		snprintf(buf, buf_size, "#%d@%s", CpLine(cp), CpFilenameInTable(cp));
	return buf;
}
char *CpStr(char buf[CPSTR_SIZE], cp_t cp)
{
	return CodePosStr(buf, CPSTR_SIZE, cp);
}
//}}}


//{{{ DeXxx
void DeNull(cp_t cp, cp_t call_cp, const char *msg)
{
	(void)cp;
	(void)call_cp;
	(void)msg;
	return;
}
void DeDefaultDo(cp_t cp, const char *msg)
{   
	// std::cout << msg  << " " << CpStr(cp) << std::endl;
	//std::cout << CpStr(cp) << std::endl;
	char str_cp[CPSTR_SIZE];
	qos_printf("%s %s" LINEND, msg, CpStr(str_cp, cp));
	return;
}
// 打印调用位置
void DeCall(cp_t ret_cp, cp_t call_cp, const char *msg)
{
	// std::cout << msg  << "" << CpStr(ret_cp) << ", call at " << CpStr(call_cp) << "" << std::endl;
	//std::cout << CpStr(cp) << std::endl;
	char buf_ret_cp[CPSTR_SIZE];
	char buf_call_cp[CPSTR_SIZE];
	qos_printf("%s%s, call at %s" LINEND, msg, CpStr(buf_ret_cp, ret_cp), CpStr(buf_call_cp, call_cp));
	return;
}
//}}}


//{{{ dvb/dve
void dvb_do(bool b, cp_t call_cp)
{
	if (b)
		return;
	// std::cout << "dvb() FAIL, call at " << CpStr(call_cp) << std::endl;
	char str_cp[CPSTR_SIZE];
	qos_printf("dvb() FAIL, call at %s\n", CpStr(str_cp, call_cp));
	return;
}
void dve_do(cp_t e, cp_t call_cp)
{
	if (0 == e)
		return;
	//std::cout << "e:" << e << std::endl;
	//std::cout << "cp:" << cp << std::endl;
	// std::cout << "dve() FAIL, call at " << CpStr(call_cp) << ", error at " << CpStr(e) << std::endl;

	char str_e[CPSTR_SIZE];
	char str_call_cp[CPSTR_SIZE];
	qos_printf("dve() FAIL, call at %s, error at %s." LINEND, CpStr(str_call_cp, call_cp), CpStr(str_e, e));
	return;
}
//}}}



//{{{ here
void here_do(cp_t cp, const char *message)
{
	//const std::string cp_str = CpStr(cp);
	char str_cp[CPSTR_SIZE];
	qos_printf("[here_%s][%s]\n", message, CpStr(str_cp, cp));
	fflush(stdout);
	return;
}
// because isprint(cause Segmentation fault with stack address, so write mine.
static bool qisprint(int c)
{
	// ' ' - '~'
	return (c >= ' ' && c <= '~');
}
/*
void var_do(cp_t cp, const char* var_name, long n, int size, const char *func)
{
	//unit_test_output_scroll(1);
	//const char *file_name = file_id_name(file_id);
	//if (NULL == file_name)
	//{
	//	file_name = m_file_name;
	//	line = __LINE__;
	//}
	if (NULL == var_name)
	{
		//file_name = m_file_name;
		//line = __LINE__;
		cp = cph;
	}
	const std::string cp_str = CpStr(cp);
	// 数值放在前面，是因为有些名字很长
	const char ch = qisprint(n) ? (char)n : '.';
	if ((unsigned int)size <= sizeof(int))
		qos_printf("[var: %s = %d(D), 0x%x][char=%c][size=%d][%s!%s()]\n",
				var_name, (int)n, (unsigned int)n, ch, size, cp_str.c_str(), func);
	else if ((unsigned int)size <= sizeof(long))
		qos_printf("[var: %s = %ld(D), 0x%lx][char=%c][size=%d][%s!%s()]\n",
				var_name, n, (unsigned long)n, ch, size, cp_str.c_str(), func);
	else
		qos_printf("[var: %s = %ld(D), 0x%lx][char=%c][size=%d][%s!%s()]\n",
				var_name, n, (unsigned long)n, ch, size, cp_str.c_str(), func);
	//if (wait_key)
	//	wait_any_key();
	return;
}
*/
void var_do(const char *filename, int line, const char* var_name, long n, int size, const char *func)
{
	//unit_test_output_scroll(1);
	if (NULL == var_name)
	{
		filename = __FILE__;
		line = __LINE__;
	}
	// 数值放在前面，是因为有些名字很长
	const char ch = qisprint(n) ? (char)n : '.';
	if ((unsigned int)size <= sizeof(int))
		qos_printf("[var: %s = %d(D), 0x%x][char=%c][size=%d][#%d@%s!%s()]\n",
				var_name, (int)n, (unsigned int)n, ch, size, line, filename, func);
	else if ((unsigned int)size <= sizeof(long))
		qos_printf("[var: %s = %ld(D), 0x%lx][char=%c][size=%d][#%d@%s!%s()]\n",
				var_name, n, (unsigned long)n, ch, size, line, filename, func);
	else
		qos_printf("[var: %s = %ld(D), 0x%lx][char=%c][size=%d][#%d@%s!%s()]\n",
				var_name, n, (unsigned long)n, ch, size, line, filename, func);
	//if (wait_key)
	//	wait_any_key();
	return;
}
void dmem_do(cp_t cp, const char* mem_name, const char* mem, int len)
{
	/*
	unit_test_output_scroll(1);
	const char* file_name = file_id_name(file_id);
	if (NULL == file_name)
	{
		file_name = m_file_name;
		line = __LINE__;
	}
	*/
	if (NULL == mem_name)
		cp = cph;
	if (NULL == mem)
		cp = cph;
	{
	//const std::string cp_str = CpStr(cp);
	//qos_printf("mem:%s[len=%d][%s]\n", mem_name, len, cp_str.c_str());
	char str_cp[CPSTR_SIZE];
	qos_printf("mem:%s[len=%d][%s]\n", mem_name, len, CpStr(str_cp, cp));
	}
	// 字符串格式
	{
		// str可能不是'\0'结束，所以一个个字符打印
		qos_printf("\t[dot_style=");
		const char *p = mem;
		int i = 0;
		for (; i < len; i++, p++)
		{
			if (qisprint(*p))
				qos_printf("%c", *p);
			else
				qos_printf("%s", ".");
		}
		qos_printf("%s", "]\n");
	}
	// c style
	{
		qos_printf("%s", "\t[  c_style=");
		const char *p = mem;
		int i = 0;
		for (; i < len; i++, p++)
		{
			unsigned char uc = *p;
			if (qisprint(*p))
				qos_printf("%c", uc);
			else
				qos_printf("\\x%02x", uc);
		}
		qos_printf("%s", "]\n");
	}
	// 16进制格式
	{
		qos_printf("%s", "\t[hex_style=");
		const char *p = mem;
		int i = 0;
		for (; i < len; i++, p++)
		{
			unsigned char uc = *p;
			qos_printf("%02x", uc);
		}
		qos_printf("%s", "]\n");
	}
	/*
	if (wait_key)
	{
		wait_any_key();
	}
	else
	{
		//sleep_sec(SLOW_SEC);
	}
	*/
	return;
}
void dstr_do(cp_t cp, const char *str_name, const char *str)
{
	// unit_test_output_scroll(1);
	const char *s = "[NULL]";
	if (NULL == str)
		str = s;
	/*
	const char *file_name = file_id_name(file_id);
	if (NULL == file_name)
	{
		file_name = m_file_name;
		line = __LINE__;
	}
	if (NULL == str_name)
	{
		file_name = m_file_name;
		line = __LINE__;
	}
	if (NULL == str)
	{
		file_name = m_file_name;
		line = __LINE__;
	}
	*/
	if (NULL == str_name)
		cp = cph;
	if (NULL == str)
		cp = cph;
	// const std::string cp_str = CpStr(cp);
	// const int len = strlen(str);
	// qos_printf("dstr:%s[len=%d][%s]", str_name, len, cp_str.c_str());
	char str_cp[CPSTR_SIZE];
	const int len = strlen(str);
	qos_printf("dstr:%s[len=%d][%s]", str_name, len, CpStr(str_cp, cp));
	// 字符串格式
	{
		// str可能不是'\0'结束，所以转存到堆栈内存上
		qos_printf("[str=%s]", str);
	}
	// 16进制格式
	{
		qos_printf("[hex=");
		for (int i = 0; i < len - 1; i++)
			qos_printf("%02x", (unsigned char)str[i]);
		qos_printf("%02x]\n", (unsigned char)str[len - 1]);
	}
	/*
	if (wait_key)
	{
		wait_any_key();
	}
	else
	{
		//sleep_sec(SLOW_SEC);
	}
	*/
	return;
}

//}}}

