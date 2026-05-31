#include "CheckingAccount.hpp"
#include <iostream>
#include <stdexcept>

CheckingAccount::CheckingAccount(std::string iban, std::string owner, double initialBalance, double fee)
    : Account(iban, owner, initialBalance), transactionFee(fee) {}

void CheckingAccount::deposit(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Сумата за депозит трябва да бъде положително число!");
    }
    balance += amount;
    transactions.push_back(Transaction("DEPOSIT", amount));
}

void CheckingAccount::withdraw(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Сумата за теглене трябва да бъде положително число!");
    }
    
    double totalDeduction = amount + transactionFee;
    if (balance < totalDeduction) {
        throw std::runtime_error("Недостатъчна наличност! (Изисквани: " + std::to_string(totalDeduction) + " BGN с таксата)");
    }
    
    balance -= totalDeduction;
    transactions.push_back(Transaction("WITHDRAWAL", amount));
    transactions.push_back(Transaction("FEE", transactionFee));
}

double CheckingAccount::getFee() const { return transactionFee; }