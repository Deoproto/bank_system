#include <iostream>
#include "BankSystem.hpp"

int main() {
    // Създаваме банковата система
    BankSystem bank;

    // 1. Тест на Функционалност 3: Отваряне на сметки през системата
    bank.openCheckingAccount("BG11BNKB1111", "Петър Петров", 300.0, 2.00); // 2.00 лв. такса
    bank.openSavingsAccount("BG22BNKB2222", "Елена Георгиева", 2000.0, 0.03); // 3% лихва

    bank.displayAllAccounts();

    // 2. Тест на транзакции през мениджъра
    std::cout << "\n[Тест] Петър тегли 50 BGN...\n";
    bank.performWithdraw("BG11BNKB1111", 50.0); // Трябва да удържа 50 + 2 лв. такса

    std::cout << "\n[Тест] Елена депозира 500 BGN...\n";
    bank.performDeposit("BG22BNKB2222", 500.0);

    bank.displayAllAccounts();

    // 3. Тест на Функционалност 4: Затваряне (изтриване) на сметка
    std::cout << "\n[Тест] Затваряне на разплащателната сметка на Петър...\n";
    bank.closeAccount("BG11BNKB1111");

    // Показваме финалния списък (трябва да е останала само Елена)
    bank.displayAllAccounts();

    return 0;
}