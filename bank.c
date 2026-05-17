#include "bank.h"
#include <stdio.h>
#include <string.h>

void bank_init(Bank *b) {
    memset(b, 0, sizeof(*b));
    b->next_acc_id  = 1001;
    b->next_user_id = 1;
}

/* ---- Users ---- */

User *bank_create_user(Bank *b, const char *name, const char *pin) {
    if (b->userCount >= MAX_USERS) {
        printf("  Грешка: максимален брой потребители.\n"); return NULL;
    }
    User *u = &b->users[b->userCount++];
    u->userId  = b->next_user_id++;
    strncpy(u->name, name, sizeof(u->name) - 1);
    u->pinHash = hash_pin(pin);
    u->active  = 1;
    printf("  Потребител '%s' създаден (ID: %d).\n", u->name, u->userId);
    return u;
}

User *bank_find_user(Bank *b, int userId) {
    for (int i = 0; i < b->userCount; i++)
        if (b->users[i].userId == userId && b->users[i].active)
            return &b->users[i];
    return NULL;
}

void bank_list_users(const Bank *b) {
    printf("\n  === Потребители (%d) ===\n", b->userCount);
    for (int i = 0; i < b->userCount; i++)
        if (b->users[i].active) user_print(&b->users[i]);
}

/* ---- Accounts ---- */

AccountBase *bank_create_account(Bank *b, int ownerId, AccountType type,
                                  double param1, double param2) {
    if (b->accountCount >= MAX_ACCOUNTS) {
        printf("  Грешка: максимален брой сметки.\n"); return NULL;
    }
    User *owner = bank_find_user(b, ownerId);
    if (!owner) { printf("  Грешка: потребителят не е намерен.\n"); return NULL; }
    if (owner->accountCount >= MAX_ACC_USER) {
        printf("  Грешка: потребителят има твърде много сметки.\n"); return NULL;
    }

    AccountBase *acc = NULL;
    int id = b->next_acc_id++;

    if (type == ACC_SAVINGS) {
        SavingsAccount *sa = &b->savings_pool[b->savings_count++];
        savings_init(sa, id, ownerId, param1, param2);
        acc = &sa->base;
        printf("  Спестовна сметка #%d открита (лихва: %.2f%%, мин.салдо: %.2f).\n",
               id, param1 * 100, param2);
    } else {
        CheckingAccount *ca = &b->checking_pool[b->checking_count++];
        checking_init(ca, id, ownerId, param1, param2);
        acc = &ca->base;
        printf("  Разплащателна сметка #%d открита (овърдрафт: %.2f, такса: %.2f).\n",
               id, param1, param2);
    }

    b->accounts[b->accountCount++] = acc;
    owner->accountIds[owner->accountCount++] = id;
    return acc;
}

AccountBase *bank_find_account(Bank *b, int accountId) {
    for (int i = 0; i < b->accountCount; i++)
        if (b->accounts[i]->accountId == accountId && b->accounts[i]->active)
            return b->accounts[i];
    return NULL;
}

void bank_list_accounts(const Bank *b) {
    printf("\n  === Сметки (%d активни) ===\n", b->accountCount);
    for (int i = 0; i < b->accountCount; i++)
        if (b->accounts[i]->active)
            b->accounts[i]->vtable->print_details(b->accounts[i]);
}

void bank_close_account(Bank *b, int accountId) {
    AccountBase *acc = bank_find_account(b, accountId);
    if (!acc) { printf("  Грешка: сметката не е намерена.\n"); return; }
    acc->active = 0;
    printf("  Сметка #%d е закрита.\n", accountId);
}

/* ---- Periodic operations ---- */

void bank_apply_interest(Bank *b) {
    printf("\n  Начисляване на лихва по всички спестовни сметки...\n");
    for (int i = 0; i < b->accountCount; i++) {
        AccountBase *a = b->accounts[i];
        if (a->active && a->type == ACC_SAVINGS)
            a->vtable->apply_periodic(a);
    }
}

void bank_apply_fees(Bank *b) {
    printf("\n  Начисляване на месечни такси по разплащателните сметки...\n");
    for (int i = 0; i < b->accountCount; i++) {
        AccountBase *a = b->accounts[i];
        if (a->active && a->type == ACC_CHECKING)
            a->vtable->apply_periodic(a);
    }
}

/* ---- System report ---- */

void bank_system_report(const Bank *b) {
    double total_balance = 0;
    int    tx_total = 0;
    AccountBase *top = NULL;

    printf("\n  ╔══════════════════════════════════════╗\n");
    printf("  ║       СИСТЕМЕН ДОКЛАД — БАНКА        ║\n");
    printf("  ╚══════════════════════════════════════╝\n");
    printf("  Потребители      : %d\n", b->userCount);
    printf("  Активни сметки   : %d\n", b->accountCount);

    for (int i = 0; i < b->accountCount; i++) {
        AccountBase *a = b->accounts[i];
        if (!a->active) continue;
        total_balance += a->balance;
        tx_total      += a->histCount;
        if (!top || a->balance > top->balance) top = a;
    }

    printf("  Общо салдо       : %.2f лв\n", total_balance);
    printf("  Общо транзакции  : %d\n", tx_total);
    if (top)
        printf("  Топ сметка       : #%d (%.2f лв)\n", top->accountId, top->balance);
    printf("\n");
}
