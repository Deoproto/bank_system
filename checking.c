#include "checking.h"
#include <stdio.h>
#include <string.h>

static int chk_deposit(AccountBase *self, double amount, const char *desc) {
    if (amount <= 0) { printf("  Грешка: невалидна сума.\n"); return -1; }
    self->balance += amount;
    acc_add_tx(self, TX_DEPOSIT, amount, desc);
    printf("  Депозирано %.2f лв. Ново салдо: %.2f лв\n", amount, self->balance);
    return 0;
}

static int chk_withdraw(AccountBase *self, double amount, const char *desc) {
    CheckingAccount *ca = (CheckingAccount *)self;
    if (amount <= 0) { printf("  Грешка: невалидна сума.\n"); return -1; }
    if (self->balance - amount < -ca->overdraftLimit) {
        printf("  Грешка: надвишава овърдрафт лимита (%.2f лв).\n", ca->overdraftLimit);
        return -2;
    }
    self->balance -= amount;
    acc_add_tx(self, TX_WITHDRAWAL, amount, desc);
    printf("  Изтеглено %.2f лв. Ново салдо: %.2f лв\n", amount, self->balance);
    return 0;
}

static void chk_apply_periodic(AccountBase *self) {
    CheckingAccount *ca = (CheckingAccount *)self;
    self->balance -= ca->monthlyFee;
    acc_add_tx(self, TX_FEE, ca->monthlyFee, "Месечна такса");
    printf("  Месечна такса %.2f лв начислена. Ново салдо: %.2f лв\n",
           ca->monthlyFee, self->balance);
}

static void chk_print_details(const AccountBase *self) {
    const CheckingAccount *ca = (const CheckingAccount *)self;
    printf("  [Разплащателна #%d] Салдо: %.2f лв | Овърдрафт: %.2f лв | Такса: %.2f лв | %s\n",
           self->accountId, self->balance,
           ca->overdraftLimit, ca->monthlyFee,
           self->active ? "Активна" : "Закрита");
}

static const AccountVtable checking_vtable = {
    chk_deposit,
    chk_withdraw,
    chk_apply_periodic,
    chk_print_details
};

void checking_init(CheckingAccount *ca, int id, int ownerId,
                   double overdraftLimit, double monthlyFee) {
    memset(ca, 0, sizeof(*ca));
    ca->base.vtable        = &checking_vtable;
    ca->base.type          = ACC_CHECKING;
    ca->base.accountId     = id;
    ca->base.ownerId       = ownerId;
    ca->base.balance       = 0.0;
    ca->base.active        = 1;
    ca->overdraftLimit     = overdraftLimit;
    ca->monthlyFee         = monthlyFee;
}

int checking_transfer(CheckingAccount *src, AccountBase *dest, double amount) {
    if (amount <= 0) { printf("  Грешка: невалидна сума.\n"); return -1; }
    CheckingAccount *ca = src;
    AccountBase *s = &src->base;
    if (s->balance - amount < -ca->overdraftLimit) {
        printf("  Грешка: недостатъчно салдо за превод.\n");
        return -2;
    }
    s->balance -= amount;
    acc_add_tx(s, TX_TRANSFER_OUT, amount, "Изходящ превод");
    dest->balance += amount;
    acc_add_tx(dest, TX_TRANSFER_IN, amount, "Входящ превод");
    printf("  Превод %.2f лв от сметка #%d към сметка #%d — успешен.\n",
           amount, s->accountId, dest->accountId);
    return 0;
}
