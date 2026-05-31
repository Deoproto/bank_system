#include "CheckingAccount.hpp"

CheckingAccount::CheckingAccount(std::string iban, std::string owner, double initialBalance)
    : Account(iban, owner, initialBalance) {}

void CheckingAccount::deposit(double amount) {}
void CheckingAccount::withdraw(double amount) {}