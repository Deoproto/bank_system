#include <iostream>
#include <clocale>
#include "BankSystem.hpp"

int main() {
    // Включваме кирилицата
    std::setlocale(LC_ALL, "bg_BG.UTF-8");
    
    BankSystem bank;

    // Отваряме сметка със 100 BGN баланс и 2 BGN такса
    bank.openCheckingAccount("BG11BNKB1111", "Петър Петров", 100.0, 2.00); 

    std::cout << "\n--- ТЕСТ 1: Опит за теглене на прекалено голяма сума ---\n";
    bank.performWithdraw("BG11BNKB1111", 500.0); // Трябва да хвърли runtime_error (няма толкова пари)

    std::cout << "\n--- ТЕСТ 2: Опит за операция с отрицателна сума ---\n";
    bank.performDeposit("BG11BNKB1111", -20.0); // Трябва да хвърли invalid_argument (нелегална сума)

    std::cout << "\n--- ТЕСТ 3: Легална операция за проверка ---\n";
    bank.performWithdraw("BG11BNKB1111", 50.0); // Трябва да мине успешно (50 + 2 такса = 52 удържани)

    // Показваме сметката, за да видим крайния баланс
    bank.displayAllAccounts();

    return 0;
}