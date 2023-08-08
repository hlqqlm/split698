#ifndef __QSTR_H__
#define __QSTR_H__

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "qcp.h"
//#include "qstr.xcp.h"
#define this_file_id    0xa35a57e3   // echo -n qstr.c | rhash --simple -

// str相关
bool ZeroExistInStr(const char *str, int buf_size);
// 如果str中有0，则返回字符串长度。
// 如果str中没有0，则返回-1
int StrLen(const char *str, int buf_size);
cp_t StrValid(const char *str, int buf_size);

// 16进制转换
cp_t HexStrValid(const std::string &hex);
cp_t HexStrValid(const char *hex, int size);
std::string HexStr(const void *data, int size);
std::string HexStr(uint32_t n);
std::string HexStr(const std::string &src);
// 打印mem和其size的统一格式
// [size|xxxxxxxx]
// 考虑mem=NULL的情况
std::string MemAndSizeToHexStr(const void *mem, int size);
// 逆转换
std::string HexStr2Mem(const std::string &hex_str);
//cp_t HexStr2Mem(const std::string hex_str, OadAutoMem &oam);
// (mem_size != String2buff(hex_string, mem_size, (char*)&dst))
//


// 打印字符串vector
std::ostream & operator<<(std::ostream &os, const std::vector<std::string> &string_vector);

// split
std::vector<std::string> StrSplit(const std::string &s, char delimiter);

// string extend字符串扩展函数
// 将mem的值，拷贝到str偏移量为offset，长度为size。
// 如果str长度不够offset，则添加占位符'0'。
// 如果超过offset，则覆盖一部分。
std::string &StringReplace(std::string &str, int offset, int size, const void *mem);
std::string &StringReplace(std::string &str, int offset, const std::string &mem);

template<typename T>
std::string &Array2Str(std::string &str, const T array[], int array_n) 
{
	for (int i = 0; i < array_n; i++)
	{
		const std::string s((char*)(&array[i]), sizeof(T));
		str.append(s.data(), s.size());
	}
	
	return str;
}
template<typename T>
cp_t StrArrayValid(const std::string &str) 
{
	const int ts = sizeof(T);
	dvb(0 < ts);
	const int s = str.size();
	ifbr(0 == s % ts);
	return 0;
}
template<typename T>
int StrArrayLen(const std::string &str) 
{
	const int ts = sizeof(T);
	const int s = str.size();
	if (0 != s % ts)
		return 0;
	return s / ts;
}
template<typename T>
void Str2Array(T array[], int array_n, const std::string &str) 
{
	dvb(array_n == StrArrayLen<T>(str));

	const char *mem = str.data();
	int offset = 0;
	for (int i = 0; i < array_n; i++, offset += sizeof(T))
	{
		const char * const src = mem + offset;
		memcpy(&array[i], src, sizeof(T));
	}

	return;
}


/*
template<typename T>
cp_t Str2Array(T array[], int array_n, const std::string &str) 
{
	const int s = sizeof(T);
	ifbr(0 == (str.size() % s));
	const int array_n = str.size() / s;

	for (int i = 0; i < array_n; i++)
	{
		const std::string s(&array[i], sizeof(T));
		str += s;
	}
	
	return str;
}
*/



// 字符串_内存拷贝相关
bool str_to_str(char *dst, int dst_size, const char *src);
bool mem_to_mem(void *dst, int dst_size, const void *src, int src_size);

int mem_to_str(char *str, int str_size, const void *mem, int n);
int str_to_mem(void *mem, int mem_size, const char *str);

int memlen(const char *mem, int size, char end_char);

bool append_str(char *dst, int dst_size, const char *src);
int strsize(const char *str);
char *strtrim(char *str);
char *strtrim_right(char *str);
#define mem_dup_trim_alloc(mem_alloc_fun, trim_fun, mem, mem_size)      \
(char*)                                                                 \
({						                        \
        dvb(NULL != mem);                                                \
        dvb(0 <= mem_size);                                              \
                                                                        \
        const int _tmp_size = mem_size + 1;                             \
        char *_tmp = (char*)mem_alloc_fun(_tmp_size);                          \
        dvb(NULL != _tmp);                                               \
                                                                        \
		memcpy(_tmp, mem, mem_size);                                    \
        _tmp[mem_size] = '\0';                                          \
        char *_p = trim_fun(_tmp);                                      \
        dvb(mem_size >= strlen(_tmp));                                   \
                                                                        \
        _p;                                                             \
})


// 内存处理
void const *memcpy_src_offset(void *dst, const void *src, int byte_num);
void *memcpy_dst_offset(void *dst, const void *src, int byte_num);

// mem store float is: high byte first. use in communication of cpu
void *float_to_mem_dst_offset(void *dst, const float *src);
void *mem_to_float_src_offset(float *dst, const void *src);

// mem store int is: high byte first. use in communication of cpu
void *int_to_mem_dst_offset(void *dst, const int *src);
void *mem_to_int_src_offset(int *dst, const void *src);

bool memcpy_fill(char *dst, int dst_size, const char *src, int src_len, char fill);
void switch_2_byte(char byte[2]);
int memcmp_nocase(const void *s1, const void *s2, size_t n);
int memcmp_ex(const void *s1, const void *s2, int len, int *diff_ix);
void mem_reverse(void *mem, int size);
// 查找字符
int mem_find_ch(char ch, const char *mem, int len);
bool mem_is_same(char ch, const void *mem, int len);
char *mem_in_mem(const char *haystack, int haystack_size, const char *needle, int needle_size);


// mem_alloc_fun:
//      alloca()
//      qmalloc()
//      malloc()
//
// example:
//      char *p = mem_to_str_on_mem(alloca, "123", 3);
//      p is on stack, needn't free
//      char *q = mem_to_str_on_mem(qmalloc, "123", 3);
//      qfreez(q);
//
typedef void *(*mem_alloc_t)(int size);
// 很多情况下，需要将一个没有0结尾的字符数组，临时转换为0结尾的字符串，用来显示等。
#define mem_to_str_alloc(mem_alloc_fun, src, src_len)                   \
(char*)                                                                 \
({						                        \
        dvb(NULL != src);                                                \
        dvb(0 <= src_len);                                               \
                                                                        \
        const int _dst_size = src_len + 1;                              \
        char *_dst = mem_alloc_fun(_dst_size);                          \
        dvb(NULL != _dst);                                               \
                                                                        \
	memcpy(_dst, src, src_len);                                    \
        _dst[src_len] = '\0';                                           \
                                                                        \
        _dst;                                                           \
})

//char *str_dup_alloc(mem_alloc_t mem_alloc, const char* src);
#define str_dup_alloc(mem_alloc_fun, src)                               \
(char*)                                                                 \
({					                                \
        dvb(NULL != src);                                                \
                                                                        \
        const int _dst_size = strlen(src) + 1;                          \
        char *_dst = (char*)mem_alloc_fun(_dst_size);                   \
        dvb(NULL != _dst);                                               \
                                                                        \
	memcpy(_dst, src, _dst_size);                                   \
                                                                        \
        _dst;                                                           \
})

#define mem_dup_alloc(mem_alloc_fun, src, len)                          \
(void*)                                                                 \
({									\
        dvb(NULL != src);                                                \
        dvb(0 <= len);                                                   \
                                                                        \
        char *_dst = mem_alloc_fun(len);                                \
        dvb(NULL != _dst);                                               \
                                                                        \
	memcpy(_dst, src, len);                                         \
                                                                        \
        _dst;                                                           \
})



#if 0
// 如果snprintf() str size不够，则打dv()，其他行为不变。
#define snprintf_dv(str, size, format, ...)                             \
(int)                                                                 \
({								        \
        const int _t_size = snprintf(str, size, format, __VA_ARGS__);   \
        dv(_t_size >= 0);                                                \
        char *_t_p = mem_alloc_fun(_t_size);                            \
        dv(NULL != _t_p);                                               \
                                                                        \
	const int _t_result_len = snprintf(_t_p, _t_size, fmt, __VA_ARGS__);  \
        UNUSED(_t_result_len);                                          \
        dv(_t_result_len+1 == _t_size);                                 \
        dv(_t_p[_t_result_len] == '\0');                                \
                                                                        \
        _t_p;                                                                   \
})

int len_add = snprintf(buf, buf_size, fmt,)
			  dv(len_expect < buf_size);
qstr.h
#endif

// snprintf()行为：
// 始终会在输出最后添加‘\0’
// 输出内容，不会超过buf长度。
// return：如果缓冲区长度够的话，格式化后的字符串长度。
//      返回值会为0，例如：sprintf(buf, 5, "%s", "");
//
//
//
// 一个snprintf()安全版本
// 1.	简化工作，会返回一个内存指针，存放格式化后的结果。
// 2.   可以指定内存在堆/堆栈上。
// 3.	如果结果放在堆栈上。你无需为内存分配，也无需释放内存。
// 4.	对snprintf()结果进行检查。如果不对，返回的NULL
// 5.	为了尽量减少二意性，和sprintf()传入、传出的参数，意义尽量相同
//
// 参数含义：
// return		格式化后的字符串首地址
//
// usuage:
//	const char *p = sprintf_alloc(alloca, "%s%d", "abc", 123);
//
// read: g_new()宏定义写法。
#define sprintf_alloc(mem_alloc_fun, fmt, ...)	                        \
(char*)                                                                 \
({								        \
        const int _t_size = sprintf_size(fmt, __VA_ARGS__);             \
        dvb(_t_size > 0);                                                \
        char *_t_p = (char*)mem_alloc_fun(_t_size);                         \
        dvb(NULL != _t_p);                                               \
                                                                        \
		const int _t_result_len = snprintf(_t_p, _t_size, fmt, __VA_ARGS__);  \
        UNUSED(_t_result_len);                                          \
        dvb(_t_result_len+1 == _t_size);                                 \
        dvb(_t_p[_t_result_len] == '\0');                                \
                                                                        \
        _t_p;                                                                   \
})
// 限制alloc memory长度
#define sprintf_alloc_limit(size_limit, mem_alloc_fun, fmt, ...)	                        \
(char*)                                                                \
({								        \
        const int _t_size = sprintf_size(fmt, __VA_ARGS__);             \
        dvb(_t_size > 0);                                                \
        dvb(_t_size <= size_limit);                                      \
        char *_t_p = NULL;                                              \
        if (_t_size <= size_limit) {                                    \
                _t_p = mem_alloc_fun(_t_size);                          \
                dvb(NULL != _t_p);                                       \
                                                                        \
                if (NULL != _t_p) {                                     \
                        const int _t_result_len = snprintf(_t_p, _t_size, fmt, __VA_ARGS__);  \
                        dvb(_t_result_len+1 == _t_size);                 \
                        dvb(_t_p[_t_result_len] == '\0');                \
                }                                                       \
        }                                                               \
                                                                        \
        _t_p;                                                                   \
})

#define sprintf_on_stack(fmt, ...)    sprintf_alloc(alloca, fmt, __VA_ARGS__)


#define sprintf_hex_style_alloc(_mem_alloc_fun, _str, _len, _separator)     \
(char*)                                                                 \
({								        \
        const int _len_expect = snprintf_hex_style(NULL, 0, _str, _len, _separator); \
        const int _size = _len_expect + 1;                                       \
        dvb(_size > 0);							\
        char *_p = (char*)_mem_alloc_fun(_size);                                \
        dvb(NULL != _p);														\
																				\
	const int _result_len = snprintf_hex_style(_p, _size, _str, _len, _separator);  \
        UNUSED(_result_len);													\
        dvb(_result_len+1 == _size);                                       \
        dvb(_p[_result_len] == '\0');                                      \
                                                                        \
        _p;                                                              \
})
// example:
// const char *str =sprintf_style_alloc(alloca, snprintf_c_style, str, len);
// sprintf_style_fun can NOT be snprintf_hex_style, but can be
// snprintf_hex_style_blank_separator
#define sprintf_style_alloc(mem_alloc_fun, snprintf_style_fun, mem, mem_size) \
(char*)                                                                 \
({								        \
        const int len_expect = snprintf_style_fun(NULL, 0, mem, mem_size);   \
        const int size = len_expect + 1;                                \
        dvb(size > 0);                                                   \
        char *_p = (char*)mem_alloc_fun(size);                                  \
        dvb(NULL != _p);                                                  \
                                                                        \
	const int result_len = snprintf_style_fun(_p, size, mem, mem_size);   \
        UNUSED(result_len);                                             \
        dvb(result_len+1 == size);                                       \
        dvb(_p[result_len] == '\0');                                      \
                                                                        \
        _p;                                                              \
})
#define printf_hex_str(_title, _str, _len, _separator, _tail)		\
do {								        \
	const char * const _hex = sprintf_hex_style_alloc(alloca, _str, _len, _separator);	\
	printf("%s%s%s", (_title), (NULL == _hex ? "" : _hex), _tail);			\
} while(0)


// 显示相关
// hex str
// hex str is: "\x12\xab" -> "12ab", can add separator, such as "12,ab"
int to_hex_mem_len(int n, const char *separator);
int to_hex_mem(char *hex, const char *src, int src_len, const char *separator);


// byte -> binary string
// 0x12 -> "00010010"
int str_to_bin_str_size(const char *bytes, int byte_len, const char *separator);
cp_t str_to_bin_str(char *bin_str, const char *bytes, int byte_len, const char *separator);

int printf_hex(const char *src, int nbyte, const char *separator);
// usuage: printf_hex_ex("mem_name: ", "\r\n", mem, size, " ");
void printf_hex_ex(const char *title, const char *tail, const char *src, int nbyte, const char *separator);
int printf_section(const char *prompt, const char *section, int section_len);
int printf_section_hex(const char *prompt,
					   const char *section, int section_len, const char *separator);
int printf_c_style(const char *str, int str_len);


int calc_buf_room(int buf_size, int cnt);
char *calc_buf_p(char *buf, int buf_size, int cnt);
int snprintf_section(char *buf, int buf_size,
					 const char *prompt, const char *section, int section_len);
int snprintf_section_hex(char *buf, int buf_size, const char *prompt, const char *section, int section_len, const char *separator);
int snprintf_dot_style(char *buf, int buf_size, const char *mem, int mem_size);
int snprintf_hex_style(char *buf, int buf_size, const char *mem, int mem_size, const char *separator);
int snprintf_hex_style_blank_separator(char *buf, int buf_size, const char *mem, int mem_size);
int snprintf_hex_style_no_separator(char *buf, int buf_size, const char *mem, int mem_size);
int snprintf_c_style(char *buf, int buf_size, const char *mem, int mem_size);
typedef int (*snprintf_style_t)(char *buf, int buf_size, const char *mem, int mem_size);
int snprintf_style(char *buf, int buf_size, const char *mem, int mem_size,
				   const char *style_descript_left[],
				   const char *style_descript_right[],
				   const snprintf_style_t *style_fun_list_null_end
				  );
int snprintf_style_default(char *buf, int buf_size, const char *mem, int mem_size);
int snprintf_style3(char *buf, int buf_size, const char *mem, int mem_size);


int sprintf_size(const char *fmt, ...);
int vsprintf_size(const char *fmt, va_list ap);
//#define printf          u_printf
//int u_printf( const char* fmt, ... );
int str_middle_insert_blank(const char *str, int line_len);

// 可视化相关
bool printable_char(int c);
bool printable_mem(const char *mem, int n);
char *str_to_lower(char *str);
char *str_to_upper(char *str);

bool digit_char(char ch);
bool digit_mem(const char *mem, int n);
//const char *mem_digit_2_valid(const char *mem, const char *low, const char *high);
cp_t mem_digit_2_valid(const char *mem, const char *low, const char *high);

cp_t hex_ch_valid(char ch);
int hex_ch_to_n(char ch);
cp_t hex_mem_to_bin(void *bin, int bin_size, const char *hex, int hex_size);

// command line
char **split_command_line(char *lpCmdLine, int *pNumArgs);

// name
const char *name_valid(const char *name, int size);
const char *name_follow_blank_valid(const char *name, int size);
const char *name_valid_with_error_descript(const char *name, int size,
		const char *should_not_be_empty,
		const char *must_be_printable_char,
		const char *blank_should_not_be_exist
										  );
const char *name_follow_blank_valid_with_error_descript(const char *name, int size,
		const char *should_not_be_empty,
		const char *must_be_printable_char,
		const char *blank_should_not_be_exist,

		const char *first_char_should_not_be_blank,
		const char *blank_should_not_be_at_middle
													   );


// hash
bool hash_crypt_table_init(void);
unsigned int hash_string(const char *string, unsigned int hash_type);

// argc argv
cp_t str_to_argc_argv(int *argcp, char *argv[], int max_args, char *str);


#undef this_file_id
#endif
