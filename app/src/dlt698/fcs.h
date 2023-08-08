/*
	---------------------------------------------------------------------------
	Copyright (c) 2003, Dominik Reichl <dominik.reichl@t-online.de>, Germany.
	All rights reserved.

	Distributed under the terms of the GNU General Public License v2.

	This software is provided 'as is' with no explicit or implied warranties
	in respect of its properties, including, but not limited to, correctness 
	and/or fitness for purpose.
	---------------------------------------------------------------------------
*/

#ifndef ___FCS_16_H___
#define ___FCS_16_H___

void fcs16_init(unsigned short *uFcs16);
void fcs16_update(unsigned short *uFcs16, unsigned char *pBuffer, unsigned long uBufSize);
void fcs16_final(unsigned short *uFcs16);
unsigned short GetFcs(const char *pBuffer, unsigned long uBufSize);

void fcs32_init(unsigned long *uFcs32);
void fcs32_update(unsigned long *uFcs32, unsigned char *pBuffer, unsigned long uBufSize);
void fcs32_final(unsigned long *uFcs32);

#endif // ___FCS_16_H___
