#ifndef __P2_PROCESS_H_
#define __P2_PROCESS_H_

#include "qos/qcp.h"
#include "dlt698/p2_link.h"


//SECURITY-APDU

typedef struct 
{
	P2Link link;
} P2Process;
#define kP2ProcessDef { kP2LinkDef }

// class
cp_t P2ProcessOpen(P2Process *m);
cp_t P2ProcessClose(P2Process *m);
cp_t P2ProcessValid(P2Process *m);

// do
cp_t P2ProcessInput(P2Process *m, const char *rx_buf, int rx_len, char *tx_buf, int tx_buf_size);

// test
cp_t P2ProcessTest(QTEST_ARG);
#endif

