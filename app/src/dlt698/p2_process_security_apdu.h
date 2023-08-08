#ifndef __P2_PROCESS_SECURITY_APDU_H_
#define __P2_PROCESS_SECURITY_APDU_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pfill_repository.h"


// do
cp_t P2ProcessSecurityApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size);

// test
cp_t P2ProcessSecurityApduTest(QTEST_ARG);
#endif

