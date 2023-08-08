#ifndef __QCP_H_
#define __QCP_H_

#include <iostream>
#include <fstream>
//#include <vector>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "qlibc.h"

// pattern: 
// #define this_file_id    0x      // echo -n xxx.cc | rhash --simple -
// echo -n xxx.c | rhash --simple -
// 改  id_filename_table_uniq.h

// code position
typedef uint64_t cp_t;

// CpStr() 传入字符串固定长度
#define CPSTR_SIZE          (256)

// 某个函数、功能待实现
//#define TODO(str)
#define TODO(...)

#define UNUSED(x) ((void)(x))

#define wait_any_key()


// #define vv_cp(_v, _cp)	do { std::cout << #_v << "=" << _v << ", at " << CodePosStr(_cp) << std::endl; } while(0)
#define vv_cp(_v, _cp)	do { char buf[CPSTR_SIZE]; qos_printf("%s=%d, at %s\r\n", #_v, (int)(_v), CpStr(buf, _cp)); } while(0)
#define vv(_v)		vv_cp(_v, (cph))

// code_pos
//#define code_pos(_file_ida, _linea)      ((cp_t)((((cp_t)(_file_ida)) << 32) | ((cp_t)(_linea))))
//#define code_pos_file_id(_ep)           ((uint32_t)(((cp_t)(_ep)) >> 32))
//#define code_pos_line(_ep)              ((uint32_t)(((cp_t)(_ep)) & 0xffffffff))
//#define code_pos_file_name(_ep)         fileid_name(code_pos_file_id(_ep))
//#define CP_FORMAT                       "#%d@%s"
//#define CP_VALUE(_ep)                   code_pos_line(_ep), code_pos_file_name(_ep)

#define CODE_POS(fileid_, line_)	((cp_t)((((cp_t)(fileid_)) << 32) | (line_)))
//cp_t CodePos(uint32_t fileid, uint32_t line);
#define cph                             (CODE_POS(this_file_id, __LINE__))
uint32_t CpFileid(cp_t cp);
uint32_t CpLine(cp_t cp);

// table
const char *CpFilenameInTable(cp_t cp);
#ifdef __cplusplus
//std::string CpFilenameInTable(cp_t cp);
std::ostream &CodePosOsStr(std::ostream &ostr, cp_t cp);
//std::string CodePosStr(cp_t cp);
std::string CpStr(cp_t cp);
// std::ostream & operator<<(std::ostream &os, cp_t &cp);
// 打印cp列表，当一个函数返回多个错误点时，可以用cp列表，然后用本函数打印
//std::ostream & operator<<(std::ostream &os, const std::vector<cp_t> &cp_vector);
#else
#endif
char *CodePosStr(char *buf, int buf_size, cp_t cp);
char *CpStr(char buf[CPSTR_SIZE], cp_t cp);

cp_t CpPrintEx(cp_t call_position, cp_t function_error_position);
#define CpPrint(fep)	CpPrintEx(cph, (fep))

//#define de_default(_ep, _msg)   do { std::cout << _msg  << CodePosition(_ep) << std::endl; } while(0)
//#define de_null(_ep, _msg)      do { } while(0)

void DeDefaultDo(cp_t cp, const char *msg);
// 忽略掉call_cp
inline void DeDefault(cp_t cp, cp_t call_cp, const char *msg) { DeDefaultDo(cp, msg); }
void DeCall(cp_t ret_cp, cp_t call_cp, const char *msg);
void DeNull(cp_t cp, cp_t call_cp, const char *msg);

#if 1
#define re_return_ep(_ep)   do { return (_ep); } while(0)
#define re_null(_ep)      do { } while(0)
#define re_exit(_ep)      do { exit(1); } while(0)

// if error, display error, return error, with express string
// 注意：宏中必须保证，_exp只运行一次。
#define ife_de_re_exp(_exp, _msg, _call_cp, _de_macro, _re_macro)           \
        do {                                            \
                const cp_t _ep = _exp;                  \
                if (0 != _ep) {                         \
                        _de_macro(_ep, _call_cp, _msg); \
                        _re_macro(_ep);                 \
                }                                       \
        } while(0)


#if 0
#if DEBUG > 0
// if error, display error, return error
#define ife_de_re(_a)           ife_de_re_exp(#_a, _a)
#else
// 如果是release版本，去掉_exp_str，减小代码大小
#define ife_de_re(_a)           ife_de_re_exp("", _a)
#endif
#endif


// 如果是release版本，去掉_exp_str，减小代码大小
// if error display message return
#define ifedmr(_a, _msg)        ife_de_re_exp(_a, _msg, 0, DeDefault, re_return_ep)
// if error display return
#define ifedr(_a)               ife_de_re_exp(_a, "", 0, DeDefault, re_return_ep)
// if error exit
#define ifee(_a)				ife_de_re_exp(_a, "exit here, ", cph, DeCall, re_exit)
// if error return
#define ifer(_a)                ife_de_re_exp(_a, "", 0, DeNull, re_return_ep)
// if error display
//打印调用的位置，和错误的位置
#define ifed(_a)				ife_de_re_exp(_a, "", cph, DeCall, re_null);


// bool express -> error position
#define b2e(_b)         ((_b) ? 0 : (cph))
// if bool express, return error pos
#define ifbdr(_b)       ifedr(b2e(_b))
#define ifbe(_b)		ifee(b2e(_b))
#define ifbr(_b)        ifer(b2e(_b))
#define ifbd(_b)        ifed(b2e(_b))


// debug verify error pos
#if DEBUG > 0
#define dve_exp(_exp_str, _ep)         ife_de_exp(_exp_str, _ep)
#else
#define dve_exp(_exp_str, _ep)         do{ } while(0)
#endif

//#define ifedm(_a, _msg)         ife_de_re_exp(_a, _msg, 0, de_default, re_null)

#if DEBUG > 0
//#define dve(_a)       ifedm(_a, "dve() FAIL for <" #_a ">:")
#define dve(_e)		dve_do((_e), (cph))
#else
#define dve(_a)
#endif

// debug verify bool express
#if DEBUG > 0
//#define dvb(_b)       ifedm(b2e(_b), "dvb() FAIL for <" #_b ">:")
#define dvb(_b)		dvb_do((_b), (cph))
#else
#define dvb(_b)
#endif

#endif



#if 0
#define FILEID_REGISTER_EX()                                                                                    \
__attribute__ ((constructor))                                                                                   \
static void __fileid_constructor(void)                                                                          \
{                                                                                                               \
		static const char *fn = __FILE__;					\
		const int r = g_fileid_register.Insert(this_file_id, fn);					\
                if (0 != r) {                                                                                           \
                        exit(r);                                                                                        \
                }                                                                                                       \
                else {          \
                }                                                                                                       \
                                                                                                                        \
                return;                                                                                                 \
}
#define FILEID_REGISTER()               FILEID_REGISTER_EX()
#endif
#define FILEID_REGISTER()               


#if 0
#define FILEID_REGISTER(_fn) void _fn ## _fileid_register(void) {               \
        static const fileid_t m_fileid = fileid_v;                                                              \
        const int r = g_file_id_list.insert(m_fileid.filename_hash, m_fileid.name);                     \
        if (0 != r) {                                                                                           \
                std::cout << "fileid insert to fileid_hash FAIL. " << std::endl;     \
                exit(1);                                                                                        \
        }                                                                                                       \
        else {          \
                std::cout << "register " <<  m_fileid.name << "ok" << std::endl;        \
        }                                                                                                       \
                                                                                                        \
        return;                                                                                         \
}
#endif



//bool nb(bool v);
void dvb_do(bool b, cp_t call_cp);
//#define dvb(b)	dvb_do((b), cph)

void dve_do(cp_t e, cp_t call_cp);
//#define dve(e)	dve_do((e), cph)






#if CFG_CPU_TYPE == CPU_TYPE_ARM
	
#define varx(v_)			\
		std::cout << #v_ << ": " << (v_) << " 0x" << std::hex << (unsigned int)(v_) << std::dec << std::endl;
#else

#define varx(v_)			\
do {					\
	if ((typeid(v_) == typeid(uint8_t)) || (typeid(v_) == typeid(int8_t))) {		\
		std::cout << #v_ << ": " << (int)(v_) << " 0x" << std::hex << (unsigned int)(v_) << std::dec << std::endl;	\
	}	\
	else if (typeid(v_) == typeid(uint32_t) || typeid(v_) == typeid(int32_t)) {		\
		std::cout << #v_ << ": " << (v_) << " 0x" << std::hex << (unsigned int)(v_) << std::dec << std::endl;	\
	} \
	else if (typeid(v_) == typeid(cp_t)) {						\
		std::cout << #v_ << ": " << CodePosStr(v_) << std::endl;		\
	}	\
	else	{									\
		std::cout << #v_ << ": " << (v_) << std::endl;				\
	} \
} while(0)
#endif

#define vary(v_)			\
do {						\
		std::cout << #v_ << ": " << (v_) << std::endl;				\
} while(0)				

#define varcp(cp_)			\
do {						\
		std::cout << #cp_ << ": " << CodePosStr(cp_) << std::endl;				\
} while(0)				



// 调试相关
// usuage:
// here( 1 )
// here( 2 )
#define here(n)         do { here_do(cph, #n); } while(0)
#if 0
#if DEBUG > 0
#define here(n)         do { here_do(cph, #n); } while(0)
//#define herek(n)        do { here_do(cph, #n, true); } while(0)
#else
#define here(n)
//#define herek(n)
#endif
#endif
void here_do(cp_t cp, const char* message);



// var
#if DEBUG > 0
//#define var(n)          do { var_do(__FILE__, __LINE__, #n, (long)(n), sizeof(typeof(n)), __func__ ); } while(0)
#define var(n)          do { var_do(__FILE__, __LINE__, #n, (long)(n), sizeof(n), __func__ ); } while(0)
#else
#define var(n)
#endif
void var_do(const char *filename, int line, const char* var_name, long n, int size, const char *func);


#if DEBUG > 0
#define dmem(m, len)    do { dmem_do(cph, #m, (char*)m, len); } while(0)
#else
#define dmem(m, len)
#endif
void dmem_do(cp_t cp, const char* mem_name, const char* mem, int len);


#if DEBUG > 0
#define dstr(s)         do { dstr_do(cph, #s, (char*)s); } while(0)
#else
#define dstr(s)
#endif
void dstr_do(cp_t cp, const char *str_name, const char *str);


// #define dmem(mem_, size_)	DisplayMem((cph), #mem_, (mem_), (size_))
// void DisplayMem(cp_t cp, const char *mem_name, const void *mem, int size);
#endif

