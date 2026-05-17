#include "account.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static int next_tx_id = 1;

int acc_add_tx(AccountBase *a, TxType t, double amount, const char *desc) {
    if (a->histCount >= MAX_TX) return -1;
    Transaction *tx = &a->history[a->histCount++];
    tx->txId      = next_tx_id++;
    tx->type      = t;
    tx->amount    = amount;
    tx->timestamp = time(NULL);
    strncpy(tx->description, desc ? desc : "", sizeof(tx->description) - 1);
    tx->description[sizeof(tx->description) - 1] = '\0';
    return 0;
}

void acc_print_history(const AccountBase *a) {
    printf("\n  История на сметка #%d (%d записа):\n", a->accountId, a->histCount);
    printf("  %-6s %-18s %10s   %-20s  %s\n",
           "ID", "Тип", "Сума (лв)", "Дата", "Описание");
    printf("  %s\n", "-----------------------------------------------------------------------");
    for (int i = 0; i < a->histCount; i++) tx_print(&a->history[i]);
}

void acc_balance_report(const AccountBase *a) {
    double total_in = 0, total_out = 0;
    for (int i = 0; i < a->histCount; i++) {
        const Transaction *t = &a->history[i];
        if (t->type == TX_DEPOSIT || t->type == TX_TRANSFER_IN || t->type == TX_INTEREST)
            total_in += t->amount;
        else
            total_out += t->amount;
    }
    printf("\n  === Баланс справка — сметка #%d ===\n", a->accountId);
    printf("  Текущо салдо : %10.2f лв\n", a->balance);
    printf("  Общо внесено : %10.2f лв\n", total_in);
    printf("  Общо изтеглено:%10.2f лв\n", total_out);
    printf("  Нетна промяна: %10.2f лв\n", total_in - total_out);
}

int acc_deposit(AccountBase *a, double amount, const char *desc) {
    return a->vtable->deposit(a, amount, desc);
}

int acc_withdraw(AccountBase *a, double amount, const char *desc) {
    return a->vtable->withdraw(a, amount, desc);
}
