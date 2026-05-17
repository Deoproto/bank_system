#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Bank bank;

/* ---- Utility ---- */
static void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int read_int(const char *prompt) {
    int v;
    printf("%s", prompt);
    if (scanf("%d", &v) != 1) { clear_input(); return -1; }
    clear_input();
    return v;
}

static double read_double(const char *prompt) {
    double v;
    printf("%s", prompt);
    if (scanf("%lf", &v) != 1) { clear_input(); return -1; }
    clear_input();
    return v;
}

static void read_str(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin)) {
        int len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
    }
}

/* ---- Menus ---- */

static void menu_users(void) {
    int ch;
    do {
        printf("\n--- Потребители ---\n");
        printf("1. Нов потребител\n");
        printf("2. Списък потребители\n");
        printf("3. Автентикация (тест)\n");
        printf("0. Назад\n");
        ch = read_int("> ");
        if (ch == 1) {
            char name[64], pin[8];
            read_str("Име: ", name, sizeof(name));
            read_str("PIN (4 цифри): ", pin, sizeof(pin));
            bank_create_user(&bank, name, pin);
        } else if (ch == 2) {
            bank_list_users(&bank);
        } else if (ch == 3) {
            int uid = read_int("Потребителски ID: ");
            char pin[8];
            read_str("PIN: ", pin, sizeof(pin));
            User *u = bank_find_user(&bank, uid);
            if (!u) { printf("  Потребителят не е намерен.\n"); }
            else if (user_authenticate(u, pin)) printf("  Автентикацията е успешна!\n");
            else printf("  Грешен PIN.\n");
        }
    } while (ch != 0);
}

static void menu_accounts(void) {
    int ch;
    do {
        printf("\n--- Сметки ---\n");
        printf("1. Нова сметка\n");
        printf("2. Списък сметки\n");
        printf("3. Закрий сметка\n");
        printf("0. Назад\n");
        ch = read_int("> ");
        if (ch == 1) {
            int uid = read_int("Потребителски ID: ");
            printf("Тип: 1=Спестовна  2=Разплащателна\n");
            int t = read_int("> ");
            if (t == 1) {
                double rate = read_double("Годишна лихва (напр. 0.03): ");
                double minb = read_double("Минимално салдо: ");
                bank_create_account(&bank, uid, ACC_SAVINGS, rate, minb);
            } else if (t == 2) {
                double ovrd = read_double("Овърдрафт лимит: ");
                double fee  = read_double("Месечна такса: ");
                bank_create_account(&bank, uid, ACC_CHECKING, ovrd, fee);
            }
        } else if (ch == 2) {
            bank_list_accounts(&bank);
        } else if (ch == 3) {
            int aid = read_int("ID на сметка: ");
            bank_close_account(&bank, aid);
        }
    } while (ch != 0);
}

static void menu_transactions(void) {
    int ch;
    do {
        printf("\n--- Транзакции ---\n");
        printf("1. Депозит\n");
        printf("2. Теглене\n");
        printf("3. Превод (от разплащателна)\n");
        printf("4. История по сметка\n");
        printf("5. Баланс справка\n");
        printf("0. Назад\n");
        ch = read_int("> ");
        if (ch == 1) {
            int aid = read_int("ID на сметка: ");
            double amt = read_double("Сума: ");
            char desc[128]; read_str("Описание: ", desc, sizeof(desc));
            AccountBase *a = bank_find_account(&bank, aid);
            if (!a) printf("  Сметката не е намерена.\n");
            else acc_deposit(a, amt, desc);
        } else if (ch == 2) {
            int aid = read_int("ID на сметка: ");
            double amt = read_double("Сума: ");
            char desc[128]; read_str("Описание: ", desc, sizeof(desc));
            AccountBase *a = bank_find_account(&bank, aid);
            if (!a) printf("  Сметката не е намерена.\n");
            else acc_withdraw(a, amt, desc);
        } else if (ch == 3) {
            int src = read_int("ID на изходяща сметка (разплащателна): ");
            int dst = read_int("ID на целева сметка: ");
            double amt = read_double("Сума: ");
            AccountBase *as = bank_find_account(&bank, src);
            AccountBase *ad = bank_find_account(&bank, dst);
            if (!as || !ad) { printf("  Сметката не е намерена.\n"); }
            else if (as->type != ACC_CHECKING) { printf("  Само разплащателни сметки могат да правят преводи.\n"); }
            else checking_transfer((CheckingAccount *)as, ad, amt);
        } else if (ch == 4) {
            int aid = read_int("ID на сметка: ");
            AccountBase *a = bank_find_account(&bank, aid);
            if (!a) printf("  Сметката не е намерена.\n");
            else acc_print_history(a);
        } else if (ch == 5) {
            int aid = read_int("ID на сметка: ");
            AccountBase *a = bank_find_account(&bank, aid);
            if (!a) printf("  Сметката не е намерена.\n");
            else acc_balance_report(a);
        }
    } while (ch != 0);
}

static void menu_periodic(void) {
    int ch;
    do {
        printf("\n--- Периодични операции ---\n");
        printf("1. Начисли лихва (всички спестовни)\n");
        printf("2. Начисли месечни такси (всички разплащателни)\n");
        printf("0. Назад\n");
        ch = read_int("> ");
        if (ch == 1) bank_apply_interest(&bank);
        else if (ch == 2) bank_apply_fees(&bank);
    } while (ch != 0);
}

/* ---- Main ---- */
int main(void) {
    bank_init(&bank);

    /* Demo seed data */
    User *u1 = bank_create_user(&bank, "Иван Иванов", "1234");
    User *u2 = bank_create_user(&bank, "Мария Петрова", "5678");
    (void)u2;
    AccountBase *a1 = bank_create_account(&bank, u1->userId, ACC_SAVINGS,   0.03, 100.0);
    AccountBase *a2 = bank_create_account(&bank, u1->userId, ACC_CHECKING, 500.0,  5.0);
    acc_deposit(a1, 2000.0, "Начален депозит");
    acc_deposit(a2, 1500.0, "Начален депозит");
    (void)a2;

    int ch;
    do {
        printf("\n╔═══════════════════════════╗\n");
        printf("║     БАНКОВА СИСТЕМА       ║\n");
        printf("╠═══════════════════════════╣\n");
        printf("║ 1. Потребители            ║\n");
        printf("║ 2. Сметки                 ║\n");
        printf("║ 3. Транзакции             ║\n");
        printf("║ 4. Периодични операции    ║\n");
        printf("║ 5. Системен доклад        ║\n");
        printf("║ 0. Изход                  ║\n");
        printf("╚═══════════════════════════╝\n");
        ch = read_int("> ");
        switch (ch) {
            case 1: menu_users();        break;
            case 2: menu_accounts();     break;
            case 3: menu_transactions(); break;
            case 4: menu_periodic();     break;
            case 5: bank_system_report(&bank); break;
        }
    } while (ch != 0);

    printf("Довиждане!\n");
    return 0;
}
