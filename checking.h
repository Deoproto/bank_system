#ifndef CHECKING_H
#define CHECKING_H

#include "account.h"

typedef struct {
    AccountBase base;
    double      overdraftLimit;
    double      monthlyFee;
} CheckingAccount;

void checking_init(CheckingAccount *ca, int id, int ownerId,
                   double overdraftLimit, double monthlyFee);

int  checking_transfer(CheckingAccount *src, AccountBase *dest, double amount);

#endif
