#include "savings.h"
#include <stdio.h>
#include <string.h>

/* ---- vtable implementations ---- */

static int sav_deposit(AccountBase *self, double amount, const char *desc) {
    if (amount <= 0) { printf("  Грешка: невалидна сума.\n"); return -1; }
    self->balance += amount;
    acc_add_tx(self, TX_DEPOSIT, amount, desc);
    printf("  Депозирано %.2f лв. Ново салдо: %.2f лв\n", amount, self->balance);
    return 0;
}

static int sav_withdraw(AccountBase *self, double amount, const char *desc) {
    SavingsAccount *sa = (SavingsAccount *)self;
    if (amount <= 0) { printf("  Грешка: невалидна сума.\n"); return -1; }
    if (self->balance - amount < sa->minBalance) {
        printf("  Грешка: би нарушило минималното салдо (%.2f лв).\n", sa->minBalance);
        return -2;
    }
    self->balance -= amount;
    acc_add_tx(self, TX_WITHDRAWAL, amount, desc);
    printf("  Изтеглено %.2f лв. Ново салдо: %.2f лв\n", amount, self->balance);
    return 0;
}

static void sav_apply_periodic(AccountBase *self) {
    SavingsAccount *sa = (SavingsAccount *)self;
    double interest = self->balance * sa->interestRate;
    self->balance += interest;
    acc_add_tx(self, TX_INTEREST, interest, "Годишна лихва");
    printf("  Лихва %.4f (%.2f%%) начислена: +%.2f лв. Ново салдо: %.2f лв\n",
           sa->interestRate, sa->interestRate * 100, interest, self->balance);
}

static void sav_print_details(const AccountBase *self) {
    const SavingsAccount *sa = (const SavingsAccount *)self;
    printf("  [Спестовна #%d] Салдо: %.2f лв | Лихва: %.2f%% | Мин.салдо: %.2f лв | %s\n",
           self->accountId, self->balance,
           sa->interestRate * 100, sa->minBalance,
           self->active ? "Активна" : "Закрита");
}

static const AccountVtable savings_vtable = {
    sav_deposit,
    sav_withdraw,
    sav_apply_periodic,
    sav_print_details
};

/* ---- Init ---- */
void savings_init(SavingsAccount *sa, int id, int ownerId,
                  double interestRate, double minBalance) {
    memset(sa, 0, sizeof(*sa));
    sa->base.vtable      = &savings_vtable;
    sa->base.type        = ACC_SAVINGS;
    sa->base.accountId   = id;
    sa->base.ownerId     = ownerId;
    sa->base.balance     = 0.0;
    sa->base.active      = 1;
    sa->interestRate     = interestRate;
    sa->minBalance       = minBalance;
}
