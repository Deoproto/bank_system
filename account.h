#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "transaction.h"
#include "user.h"

/* ---- Account type discriminator ---- */
typedef enum { ACC_SAVINGS = 1, ACC_CHECKING = 2 } AccountType;

/* ---- Forward declaration ---- */
typedef struct AccountBase AccountBase;

/* ---- Virtual function table ---- */
typedef struct {
    int    (*deposit)       (AccountBase *self, double amount, const char *desc);
    int    (*withdraw)      (AccountBase *self, double amount, const char *desc);
    void   (*apply_periodic)(AccountBase *self);   /* interest or monthly fee */
    void   (*print_details) (const AccountBase *self);
} AccountVtable;

/* ---- Base "class" ---- */
struct AccountBase {
    const AccountVtable *vtable;
    AccountType          type;
    int                  accountId;
    double               balance;
    int                  ownerId;
    Transaction          history[MAX_TX];
    int                  histCount;
    int                  active;
};

/* Helpers shared by both subtypes */
int  acc_add_tx(AccountBase *a, TxType t, double amount, const char *desc);
void acc_print_history(const AccountBase *a);
void acc_balance_report(const AccountBase *a);

/* Generic wrappers that call through vtable */
int  acc_deposit (AccountBase *a, double amount, const char *desc);
int  acc_withdraw(AccountBase *a, double amount, const char *desc);

#endif
