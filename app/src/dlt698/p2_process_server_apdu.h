#ifndef __P2_PROCESS_SERVER_APDU_H_
#define __P2_PROCESS_SERVER_APDU_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pfill_repository.h"


// do
cp_t P2ProcessServerApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size);

// test
cp_t P2ProcessServerApduTest(QTEST_ARG);
#endif

