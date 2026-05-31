#include "BankSystem.hpp"
#include "CheckingAccount.hpp"
#include "SavingsAccount.hpp"
#include <iostream>
#include <stdexcept>

BankSystem::BankSystem() {}

BankSystem::~BankSystem() {
    for (Account* acc : accounts) {
        delete acc; 
    }
    accounts.clear();
}

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
            delete *it;          
            accounts.erase(it);  
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
    if (!acc) {
        std::cout << "[Грешка] Сметката с IBAN " << iban << " не е намерена.\n";
        return;
    }
    
    try {
        acc->deposit(amount);
        std::cout << "[Успех] Депозирани " << amount << " BGN в сметка " << iban << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Отказ на операция] Грешка: " << e.what() << "\n";
    }
}

void BankSystem::performWithdraw(const std::string& iban, double amount) {
    Account* acc = findAccount(iban);
    if (!acc) {
        std::cout << "[Грешка] Сметката с IBAN " << iban << " не е намерена.\n";
        return;
    }
    
    try {
        acc->withdraw(amount);
        std::cout << "[Успех] Изтеглени " << amount << " BGN от сметка " << iban << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Отказ на operation] Грешка: " << e.what() << "\n";
    }
}

void BankSystem::performTransfer(const std::string& fromIban, const std::string& toIban, double amount) {
    Account* fromAcc = findAccount(fromIban);
    Account* toAcc = findAccount(toIban);

    if (!fromAcc) {
        std::cout << "[Трансфер - Грешка] Сметката на изпращача (" << fromIban << ") не е намерена.\n";
        return;
    }
    if (!toAcc) {
        std::cout << "[Трансфер - Грешка] Сметката на получателя (" << toIban << ") не е намерена.\n";
        return;
    }

    try {
        // 1. Опитваме да изтеглим парите от изпращача (ако няма наличност, тук ще хвърли изключение)
        fromAcc->withdraw(amount);
        
        // 2. Ако тегленето е успешно, депозираме парите при получателя
        toAcc->deposit(amount);
        
        std::cout << "[Успешен трансфер] Преведени са " << amount << " BGN от " << fromIban << " към " << toIban << "\n";
    } catch (const std::exception& e) {
        // Ако withdraw хвърли изключение, кодът скача тук и депозитът на получателя НЕ се изпълнява
        std::cout << "[Трансфер - Отказан] Операцията пропадна: " << e.what() << "\n";
    }
}
void BankSystem::performApplyInterest(const std::string& iban) {
    Account* acc = findAccount(iban);
    if (!acc) {
        std::cout << "[Грешка] Сметката с IBAN " << iban << " не е намерена.\n";
        return;
    }

    // dynamic_cast ни позволява безопасно да проверим дали обектът зад Account* е всъщност SavingsAccount*
    SavingsAccount* savingsAcc = dynamic_cast<SavingsAccount*>(acc);
    if (savingsAcc) {
        savingsAcc->applyInterest();
    } else {
        std::cout << "[Отказ] Операцията е невъзможна. Тази сметка не е Спестовна!\n";
    }
}

void BankSystem::displayAccountHistory(const std::string& iban) const {
    Account* acc = findAccount(iban);
    if (!acc) {
        std::cout << "[Грешка] Сметката с IBAN " << iban << " не е намерена.\n";
        return;
    }

    std::cout << "\n==================================================\n";
    std::cout << "  ИСТОРИЯ НА ТРАНЗАКЦИИТЕ ЗА СМЕТКА: " << iban << "\n";
    std::cout << "  Титуляр: " << acc->getOwner() << " | Текущ баланс: " << acc->getBalance() << " BGN\n";
    std::cout << "==================================================\n";

    // Взимаме вектора с транзакции през гетъра, който написахме в Checkpoint 2
    const std::vector<Transaction>& history = acc->getTransactions();

    if (history.empty()) {
        std::cout << "  Няма извършени транзакции по тази сметка.\n";
    } else {
        // Обхождаме и печатаме всяка транзакция
        for (const Transaction& tx : history) {
            tx.display(); // Извикваме метода display() от Transaction.cpp
        }
    }
    std::cout << "==================================================\n";
}