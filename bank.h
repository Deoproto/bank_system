#ifndef BANK_H
#define BANK_H

#include "account.h"
#include "savings.h"
#include "checking.h"
#include "user.h"

#define MAX_ACCOUNTS 100

typedef struct {
    /* Storage pools */
    SavingsAccount  savings_pool [MAX_ACCOUNTS / 2];
    CheckingAccount checking_pool[MAX_ACCOUNTS / 2];
    int             savings_count;
    int             checking_count;

    /* Unified pointer array for polymorphic access */
    AccountBase    *accounts[MAX_ACCOUNTS];
    int             accountCount;
    int             next_acc_id;

    User            users[MAX_USERS];
    int             userCount;
    int             next_user_id;
} Bank;

void         bank_init   (Bank *b);

/* User operations */
User        *bank_create_user  (Bank *b, const char *name, const char *pin);
User        *bank_find_user    (Bank *b, int userId);
void         bank_list_users   (const Bank *b);

/* Account operations */
AccountBase *bank_create_account(Bank *b, int ownerId, AccountType type,
                                  double param1, double param2);
AccountBase *bank_find_account  (Bank *b, int accountId);
void         bank_list_accounts (const Bank *b);
void         bank_close_account (Bank *b, int accountId);

/* Bulk periodic operations */
void         bank_apply_interest(Bank *b);   /* all savings */
void         bank_apply_fees    (Bank *b);   /* all checking */

/* Reports */
void         bank_system_report (const Bank *b);

#endif
