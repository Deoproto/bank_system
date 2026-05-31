#ifndef SAVINGS_ACCOUNT_HPP
#define SAVINGS_ACCOUNT_HPP

#include <initializer_list> // Изрично включване най-отгоре
#include "Account.hpp"

class SavingsAccount : public Account {
public:
    SavingsAccount(std::string iban, std::string owner, double initialBalance);
    
    void deposit(double amount) override;
    void withdraw(double amount) override;
};

#endif // SAVINGS_ACCOUNT_HPP