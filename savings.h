#ifndef SAVINGS_H
#define SAVINGS_H

#include "account.h"

typedef struct {
    AccountBase base;
    double      interestRate;
    double      minBalance;
} SavingsAccount;

void savings_init(SavingsAccount *sa, int id, int ownerId,
                  double interestRate, double minBalance);

#endif
