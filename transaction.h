#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <time.h>

#define MAX_TX 200

typedef enum {
    TX_DEPOSIT = 0,
    TX_WITHDRAWAL,
    TX_TRANSFER_IN,
    TX_TRANSFER_OUT,
    TX_INTEREST,
    TX_FEE
} TxType;

typedef struct {
    int     txId;
    TxType  type;
    double  amount;
    time_t  timestamp;
    char    description[128];
} Transaction;

const char *tx_type_name(TxType t);
void        tx_print(const Transaction *tx);

#endif
