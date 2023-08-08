#ifndef __A_XDR_H__
#define __A_XDR_H__

#include "qos/qtest.h"

#define kVariableLenIntContentByteNumMax (4)
#define k2Pow7  (128)
#define k2Pow15 (32768)
#define k2Pow23 (8388608)
#define k2Pow31 (2147483648)

// 可变长度的整数值编码
// 可变长度整数值字节数 = content_byte_num + 1;
int VariableLenIntByteNum(char start);

// 值
// 如果出现错误，返回kVariableLenInvalidLen=0xffffffff
#define kVariableLenInvalidLen (0xffffffff)
int VariableLenIntValue(const char buf[]);

// 数值转换成A-Xdr格式
//std::string N2AXdr(int n);
//return: 写到dst中字节数
int N2AXdr(char *dst, int n);

// test
cp_t AXdrTest(QTEST_ARG);
#endif
