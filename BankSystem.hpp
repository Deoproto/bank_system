#ifndef BANK_SYSTEM_HPP
#define BANK_SYSTEM_HPP

#include <vector>
#include <string>
#include "Account.hpp"

class BankSystem {
private:
    std::vector<Account*> accounts; // Полиморфна колекция, съдържаща всички сметки

    // Вътрешен помощен метод за намиране на сметка по IBAN
    Account* findAccount(const std::string& iban) const;

public:
    // Добави това в BankSystem.hpp в публичната секция (public):
    BankSystem();
    ~BankSystem(); // Деструкторът ще чисти заделената динамична памет, за да няма Memory Leaks

    // Функционалност 3: Отваряне на сметки
    void openCheckingAccount(const std::string& iban, const std::string& owner, double initialBalance, double fee);
    void openSavingsAccount(const std::string& iban, const std::string& owner, double initialBalance, double rate);

    // Функционалност 4: Затваряне на сметка (Изтриване)
    void closeAccount(const std::string& iban);

    // Помощни методи за извършване на транзакции през мениджъра
    void displayAllAccounts() const;
    void performDeposit(const std::string& iban, double amount);
    void performWithdraw(const std::string& iban, double amount);
    void performTransfer(const std::string& fromIban, const std::string& toIban, double amount);
    void performApplyInterest(const std::string& iban);
    void displayAccountHistory(const std::string& iban) const;

    void saveToFile() const;
    void loadFromFile();
};

#endif // BANK_SYSTEM_HPP