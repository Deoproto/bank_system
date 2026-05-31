#include <iostream>
#include <clocale>
#include "BankSystem.hpp"

int main() {
    std::setlocale(LC_ALL, "bg_BG.UTF-8");
    BankSystem bank;

    // Създаваме две сметки
    bank.openCheckingAccount("BG11BNKB1111", "Петър", 200.0, 2.00); // 200 лв., 2 лв. такса
    bank.openSavingsAccount("BG22BNKB2222", "Елена", 500.0, 0.0);   // 500 лв.

    bank.displayAllAccounts();

    std::cout << "\n--- ТЕСТ 1: Успешен трансфер (Петър праща 50 лв. на Елена) ---\n";
    bank.performTransfer("BG11BNKB1111", "BG22BNKB2222", 50.0);
    // Петър трябва да има: 200 - 50 - 2 такса = 148 лв.
    // Елена трябва да има: 500 + 50 = 550 лв.

    bank.displayAllAccounts();

    std::cout << "\n--- ТЕСТ 2: Неуспешен трансфер (Петър иска да прати 200 лв., но има само 148) ---\n";
    bank.performTransfer("BG11BNKB1111", "BG22BNKB2222", 200.0); // Трябва да се откаже и балансите да не се променят

    bank.displayAllAccounts();

    return 0;
}