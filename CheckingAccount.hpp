#ifndef CHECKING_ACCOUNT_HPP
#define CHECKING_ACCOUNT_HPP

#include <initializer_list> 
#include <string>
#include "Account.hpp"

class CheckingAccount : public Account {
private:
    double transactionFee; // Такса за всяко теглене

public:
    CheckingAccount(std::string iban, std::string owner, double initialBalance, double fee = 2.00);
    
    void deposit(double amount) override;
    void withdraw(double amount) override;

    double getFee() const;
};

#endif // CHECKING_ACCOUNT_HPP