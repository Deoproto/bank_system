#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>     // <--- Добавянето на това НАЙ-ОТГОРЕ оправи грешката с size_t
#include <vector>
#include "Transaction.hpp"

class Account {
protected:
    std::string iban;
    std::string owner;
    double balance;
    std::vector<Transaction> transactions;

public:
    // Конструктор
    Account(std::string iban, std::string owner, double initialBalance);
    
    // Виртуален деструктор - Задължителен при полиморфизъм!
    virtual ~Account();

    // Чисто виртуални функции (Правят класа абстрактен - OOP_Lesson8)
    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;

    // Константни гетери за капсулация (OOP_Lesson5)
    std::string getIBAN() const;
    std::string getOwner() const;
    double getBalance() const;
    const std::vector<Transaction>& getTransactions() const;
};

#endif // ACCOUNT_HPP