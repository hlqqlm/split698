#ifndef __QBIT_H__
#define __QBIT_H__

//#include <typeinfo>
#include "qcp.h"

// bit
unsigned int Bit1Num(unsigned int v);

#if 0
/* Set only the required bits */
#define BIT_SET(var, shift, mask, val)			\
	do {						\
		var = ((var) & ~((mask) << (shift)))	\
			| ((val) << (shift));		\
	} while (0)
#endif


// a=target variable, b=bit number to act upon 0-n 
// NOTE!!!: 
// 1    just caculate the value, NOT set the var. If you want to set the var,
// use a = BIT_SET(a, b);
// 2    BIT_CHECK() return NOT 0 or 1, if BIT_CHECK(0x41, 6), return 0x40.
// so, use if(BIT_CHECK(a, bit)) {...} instead of if(1 == BIT_CHECK(a, bit)){...}
#define BIT_SET_VALUE(a, b, v)  ((v) ? ((a) | (1 << (b))) : ((a) & (~(1 << (b)))))
#define BIT_SET(a, b)           ((a) | (1 << (b)))
#define BIT_CLEAR(a, b)         ((a) & ~(1 << (b)))
#define BIT_TOGGLE(a, b)        ((a) ^ (1 << (b)))
#define BIT_CHECK(a, b)         ((a) & (1 << (b)))      // read NOTE!!!

// x=target variable, y=mask 
#define BITMASK_SET(x, y)       ((x) | (y))
#define BITMASK_CLEAR(x, y)     ((x) & (~(y)))
#define BITMASK_TOGGLE(x, y)    ((x) ^ (y))
#define BITMASK_CHECK(x, y)     ((x) & (y))


#define BIT_TO_BYTE_NUM(bitn)   (((bitn) / 8) + (((bitn) % 8) == 0 ? 0 : 1))

// bitmap, lowest byte at index 0
void bitmap_set_value(char *bitmap, int bit, bool v);
void bitmap_set(char *bitmap, int bit);
void bitmap_clear(char *bitmap, int bit);
void bitmap_toggle(char *bitmap, int bit);
bool bitmap_check(const char *bitmap, int bit);
void bitmap_init(char *dst, const char *src, int size);
unsigned int bitmap_bit1_sum(const char *bitmap, int size);
int snprintf_bitmap_style(char *buf, int buf_size, const char *bitmap, int size);



// reverse bitmap, highest byte at index 0
void bitmapr_set_value(char *bitmap, int byte, int bit, bool v);
void bitmapr_set(char *bitmap, int byte, int bit);
void bitmapr_clear(char *bitmap, int byte, int bit);
void bitmapr_toggle(char *bitmap, int byte, int bit);
bool bitmapr_check(const char *bitmap, int byte, int bit);
int bitmapr_bit1_sum(const char *bitmap, int byte, int bit_num);
int bitmapr_next_set_bit_ix(const char *bitmap, int byte, int start_check_ix);


// bitn个bit，所产生的掩码，例如：uint8_t mask = NbitMask(5); -> 0001 1111B
template <typename T>
inline T NBitMask(int bitn)
{
	//assert(bitn <= (sizeof(T) * 8))
	T v = 0;
	for (int i = 0; i < bitn; i++)
		v |= (((T)1) << i);

	return v;
}

// reverse
uint8_t BitReverseUint8(uint8_t v);
uint32_t BitReverseUint32(uint32_t v);

// other
cp_t EqualBool(bool lhs, bool rhs);
#endif

