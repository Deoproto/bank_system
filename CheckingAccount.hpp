#ifndef CHECKING_ACCOUNT_HPP
#define CHECKING_ACCOUNT_HPP

#include <initializer_list> // Изрично включване най-отгоре
#include "Account.hpp"

class CheckingAccount : public Account {
public:
    CheckingAccount(std::string iban, std::string owner, double initialBalance);
    
    void deposit(double amount) override;
    void withdraw(double amount) override;
};

#endif // CHECKING_ACCOUNT_HPP