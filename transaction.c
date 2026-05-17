#include "transaction.h"
#include <stdio.h>

const char *tx_type_name(TxType t) {
    switch (t) {
        case TX_DEPOSIT:      return "Депозит";
        case TX_WITHDRAWAL:   return "Теглене";
        case TX_TRANSFER_IN:  return "Получен превод";
        case TX_TRANSFER_OUT: return "Изходящ превод";
        case TX_INTEREST:     return "Лихва";
        case TX_FEE:          return "Такса";
        default:              return "Неизвестно";
    }
}

void tx_print(const Transaction *tx) {
    char buf[32];
    struct tm *tm_info = localtime(&tx->timestamp);
    strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M", tm_info);
    printf("  [%4d] %-18s %10.2f лв   %-20s  %s\n",
           tx->txId,
           tx_type_name(tx->type),
           tx->amount,
           buf,
           tx->description);
}
