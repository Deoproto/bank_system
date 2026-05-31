#include <iostream>
#include "CheckingAccount.hpp"
#include "SavingsAccount.hpp"

int main() {
    std::cout << "=== СТАРТИРАНЕ НА ТЕСТ ЗА СМЕТКИТЕ ===" << std::endl;

    // 1. Създаваме Разплащателна сметка: 500 лв. първоначален баланс и 2.50 лв. такса
    CheckingAccount checking("BG99BNKB1111", "Иван Иванов", 500.0, 2.50);

    // 2. Създаваме Спестовна сметка: 1000 лв. първоначален баланс и 5% (0.05) лихва
    SavingsAccount savings("BG99BNKB2222", "Мария Петрова", 1000.0, 0.05);

    // --- ТЕСТ 1: Теглене от разплащателна сметка (трябва да вземе сумата + таксата) ---
    std::cout << "\n[Иван - Преди] Баланс: " << checking.getBalance() << " BGN\n";
    checking.withdraw(100.0); // Теглим 100 лв. + 2.50 лв. такса = 102.50 лв. удържани
    std::cout << "[Иван - След] Баланс: " << checking.getBalance() << " BGN\n";

    // --- ТЕСТ 2: Начисляване на лихва върху спестовна сметка ---
    std::cout << "\n[Мария - Преди] Баланс: " << savings.getBalance() << " BGN\n";
    savings.applyInterest(); // Начислява 5% върху 1000 лв. = +50 лв.
    std::cout << "[Мария - След] Баланс: " << savings.getBalance() << " BGN\n";

    return 0;
}