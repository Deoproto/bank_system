#ifndef SAVINGS_ACCOUNT_HPP
#define SAVINGS_ACCOUNT_HPP

#include <initializer_list> 
#include <string>
#include "Account.hpp"

class SavingsAccount : public Account {
private:
    double interestRate; // Лихвен процент

public:
    // Добавяме rate = 0.025 (2.5% лихва по подразбиране)
    SavingsAccount(std::string iban, std::string owner, double initialBalance, double rate = 0.025);
    
    void deposit(double amount) override;
    void withdraw(double amount) override;
    
    void applyInterest(); // Метод за начисляване на лихва
    double getInterestRate() const;
};

#endif // SAVINGS_ACCOUNT_HPP