#include "SavingsAccount.hpp"

SavingsAccount::SavingsAccount(std::string iban, std::string owner, double initialBalance)
    : Account(iban, owner, initialBalance) {}

void SavingsAccount::deposit(double amount) {}
void SavingsAccount::withdraw(double amount) {}