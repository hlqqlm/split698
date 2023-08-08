#ifndef __P2_PROCESS_LINK_APDU_H_
#define __P2_PROCESS_LINK_APDU_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pfill_repository.h"



// do
cp_t P2ProcessLinkApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size);

// test
cp_t P2ProcessLinkApduTest(QTEST_ARG);
#endif

