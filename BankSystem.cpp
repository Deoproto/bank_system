#include "BankSystem.hpp"
#include "CheckingAccount.hpp"
#include "SavingsAccount.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream> 
#include <sstream> 

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
    if (iban.empty() || owner.empty()) {
        std::cout << "[Грешка] IBAN и име на титуляр не могат да бъдат празни.\n";
        return;
    }
    if (findAccount(iban) != nullptr) {
        std::cout << "[Грешка] Сметка с IBAN " << iban << " вече съществува!\n";
        return;
    }
    if (initialBalance < 0.0) {
        std::cout << "[Грешка] Първоначалният баланс не може да бъде отрицателен.\n";
        return;
    }
    if (fee < 0.0) {
        std::cout << "[Грешка] Таксата за транзакция не може да бъде отрицателна.\n";
        return;
    }
    accounts.push_back(new CheckingAccount(iban, owner, initialBalance, fee));
    std::cout << "[Успех] Отворена Разплащателна сметка за " << owner << "\n";
}

void BankSystem::openSavingsAccount(const std::string& iban, const std::string& owner, double initialBalance, double rate) {
    if (iban.empty() || owner.empty()) {
        std::cout << "[Грешка] IBAN и име на титуляр не могат да бъдат празни.\n";
        return;
    }
    if (findAccount(iban) != nullptr) {
        std::cout << "[Грешка] Сметка с IBAN " << iban << " вече съществува!\n";
        return;
    }
    if (initialBalance < 0.0) {
        std::cout << "[Грешка] Първоначалният баланс не може да бъде отрицателен.\n";
        return;
    }
    if (rate < 0.0) {
        std::cout << "[Грешка] Лихвеният процент не може да бъде отрицателен.\n";
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
        std::cout << "[Отказ на операция] Грешка: " << e.what() << "\n";
    }
}

void BankSystem::performTransfer(const std::string& fromIban, const std::string& toIban, double amount) {
    if (fromIban == toIban) {
        std::cout << "[Трансфер - Грешка] Не може да превеждате към същата сметка.\n";
        return;
    }
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
void BankSystem::saveToFile() const {
    std::ofstream accFile("accounts.txt");
    std::ofstream txFile("transactions.txt");

    if (!accFile.is_open() || !txFile.is_open()) {
        std::cout << "[Грешка] Неуспешно отваряне на файловете за запис!\n";
        return;
    }

    for (Account* acc : accounts) {
        // 1. Проверяваме типа на сметката чрез dynamic_cast
        std::string type = "CHECKING";
        double specialValue = 0.0; // Ще пази или таксата, или лихвата

        SavingsAccount* savings = dynamic_cast<SavingsAccount*>(acc);
        CheckingAccount* checking = dynamic_cast<CheckingAccount*>(acc);

        if (savings) {
            type = "SAVINGS";
            specialValue = savings->getInterestRate();
        } else if (checking) {
            type = "CHECKING";
            specialValue = checking->getFee();
        }

        // 2. Записваме сметката в accounts.txt
        accFile << type << "|" 
                << acc->getIBAN() << "|" 
                << acc->getOwner() << "|" 
                << acc->getBalance() << "|" 
                << specialValue << "\n";

        // 3. Записваме нейните транзакции в transactions.txt
        for (const Transaction& tx : acc->getTransactions()) {
            txFile << acc->getIBAN() << "|" 
                   << tx.getType() << "|" 
                   << tx.getAmount() << "|" 
                   << tx.getTimestamp() << "\n";
        }
    }

    accFile.close();
    txFile.close();
    std::cout << "[Система] Данните бяха успешно записани във файловете.\n";
}

void BankSystem::loadFromFile() {
    std::ifstream accFile("accounts.txt");
    if (!accFile.is_open()) {
        return; // Ако файлът го няма още, просто излизаме
    }

    std::string line;
    // 1. Зареждаме сметките
    while (std::getline(accFile, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string type, iban, owner, balanceStr, specialStr;
        
        std::getline(ss, type, '|');
        std::getline(ss, iban, '|');
        std::getline(ss, owner, '|');
        std::getline(ss, balanceStr, '|');
        std::getline(ss, specialStr, '|');

        if (type.empty() || iban.empty() || owner.empty() || balanceStr.empty() || specialStr.empty()) {
            continue;
        }

        double balance = 0.0;
        double specialValue = 0.0;
        try {
            balance = std::stod(balanceStr);
            specialValue = std::stod(specialStr);
        } catch (const std::exception&) {
            continue;
        }

        if (type == "CHECKING") {
            accounts.push_back(new CheckingAccount(iban, owner, balance, specialValue));
        } else if (type == "SAVINGS") {
            accounts.push_back(new SavingsAccount(iban, owner, balance, specialValue));
        }
    }
    accFile.close();

    // 2. Зареждаме транзакциите
    std::ifstream txFile("transactions.txt");
    if (txFile.is_open()) {
        while (std::getline(txFile, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string iban, txType, amountStr, timestamp;

            std::getline(ss, iban, '|');
            std::getline(ss, txType, '|');
            std::getline(ss, amountStr, '|');
            std::getline(ss, timestamp, '|'); // Прочитаме старото време директно

            if (iban.empty() || txType.empty() || amountStr.empty() || timestamp.empty()) {
                continue;
            }

            double amount = 0.0;
            try {
                amount = std::stod(amountStr);
            } catch (const std::exception&) {
                continue;
            }

            Account* acc = findAccount(iban);
            if (acc) {
                acc->addTransaction(Transaction(txType, amount, timestamp));
            }
        }
        txFile.close();
    }
    std::cout << "[Система] Данните бяха успешно заредени от файловете (" << accounts.size() << " сметки).\n";
}