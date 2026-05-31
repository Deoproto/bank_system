#include "BankSystem.hpp"
#include "CheckingAccount.hpp"
#include "SavingsAccount.hpp"
#include <iostream>

BankSystem::BankSystem() {}

// Деструктор - минава през вектора и трие обектите от Heap-а, предотвратявайки изтичане на памет
BankSystem::~BankSystem() {
    for (Account* acc : accounts) {
        delete acc; // Задейства виртуалния деструктор на Account
    }
    accounts.clear();
}

// Линеен алгоритъм за търсене на сметка по уникален IBAN
Account* BankSystem::findAccount(const std::string& iban) const {
    for (Account* acc : accounts) {
        if (acc->getIBAN() == iban) {
            return acc;
        }
    }
    return nullptr;
}

void BankSystem::openCheckingAccount(const std::string& iban, const std::string& owner, double initialBalance, double fee) {
    if (findAccount(iban) != nullptr) {
        std::cout << "[Грешка] Сметка с IBAN " << iban << " вече съществува!\n";
        return;
    }
    accounts.push_back(new CheckingAccount(iban, owner, initialBalance, fee));
    std::cout << "[Успех] Отворена Разплащателна сметка за " << owner << "\n";
}

void BankSystem::openSavingsAccount(const std::string& iban, const std::string& owner, double initialBalance, double rate) {
    if (findAccount(iban) != nullptr) {
        std::cout << "[Грешка] Сметка с IBAN " << iban << " вече съществува!\n";
        return;
    }
    accounts.push_back(new SavingsAccount(iban, owner, initialBalance, rate));
    std::cout << "[Успех] Отворена Спестовна сметка за " << owner << "\n";
}

void BankSystem::closeAccount(const std::string& iban) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if ((*it)->getIBAN() == iban) {
            delete *it;          // 1. Първо освобождаваме заделената памет за обекта
            accounts.erase(it);  // 2. След това премахваме самия указател от вектора
            std::cout << "[Успех] Сметката с IBAN " << iban << " беше затворена.\n";
            return;
        }
    }
    std::cout << "[Грешка] Сметка с IBAN " << iban << " не беше намерена.\n";
}

void BankSystem::displayAllAccounts() const {
    std::cout << "\n--- СПИСЪК НА ВСИЧКИ СМЕТКИ В БАНКАТА ---\n";
    for (Account* acc : accounts) {
        std::cout << "IBAN: " << acc->getIBAN() 
                  << " | Титуляр: " << acc->getOwner() 
                  << " | Баланс: " << acc->getBalance() << " BGN\n";
    }
    std::cout << "-------------------------------------------\n";
}

void BankSystem::performDeposit(const std::string& iban, double amount) {
    Account* acc = findAccount(iban);
    if (acc) {
        acc->deposit(amount);
        std::cout << "[Успех] Депозирани " << amount << " BGN в сметка " << iban << "\n";
    } else {
        std::cout << "[Грешка] Сметката не е намерена.\n";
    }
}

void BankSystem::performWithdraw(const std::string& iban, double amount) {
    Account* acc = findAccount(iban);
    if (acc) {
        acc->withdraw(amount); // Динамично свързване (C++ знае дали да извика Checking или Savings версията)
    } else {
        std::cout << "[Грешка] Сметката не е намерена.\n";
    }
}