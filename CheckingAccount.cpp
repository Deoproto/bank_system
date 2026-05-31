#include "CheckingAccount.hpp"
#include <iostream>

CheckingAccount::CheckingAccount(std::string iban, std::string owner, double initialBalance, double fee)
    : Account(iban, owner, initialBalance), transactionFee(fee) {}

void CheckingAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        transactions.push_back(Transaction("DEPOSIT", amount));
    }
}

void CheckingAccount::withdraw(double amount) {
    double totalDeduction = amount + transactionFee;
    if (amount > 0 && balance >= totalDeduction) {
        balance -= totalDeduction;
        transactions.push_back(Transaction("WITHDRAWAL", amount));
        transactions.push_back(Transaction("FEE", transactionFee)); // Записваме таксата като отделно събитие
    } else {
        std::cout << "[Грешка] Недостатъчна наличност (Сума + Такса: " << totalDeduction << " BGN)\n";
    }
}

double CheckingAccount::getFee() const { return transactionFee; }