#ifndef  __PDO_H__
#define  __PDO_H__

#include "qos/qcp.h"
#include "qos/qtest.h"


struct PcutS;
struct PdoS;
cp_t PdoProcessOk(struct PdoS *doa, struct PcutS *cut, int ix, const char *whole);
cp_t PdoProcessFail(struct PdoS *doa, struct PcutS *cut, int ix, const char *whole);

typedef cp_t (*PdoProcessT)(struct PdoS *doa, struct PcutS *cut, int ix, const char *whole);

typedef struct PdoS
{
	PdoProcessT process;
} Pdo;
#define PDO_INIT(_process) { _process }
#define kPdoDef		PDO_INIT(PdoProcessOk)


cp_t PdoValid(const Pdo *m);
cp_t PdoTableValid(const Pdo * const *do_table, int len);

cp_t PdoProcess(Pdo *m, struct PcutS *cut, int ix, const char *whole);

// test
cp_t PdoTest(QTEST_ARG);
#endif

