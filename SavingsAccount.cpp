#include "SavingsAccount.hpp"
#include <iostream>
#include <stdexcept>

SavingsAccount::SavingsAccount(std::string iban, std::string owner, double initialBalance, double rate)
    : Account(iban, owner, initialBalance), interestRate(rate) {}

void SavingsAccount::deposit(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Сумата за депозит трябва да бъде положително число!");
    }
    balance += amount;
    transactions.push_back(Transaction("DEPOSIT", amount));
}

void SavingsAccount::withdraw(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Сумата за теглене трябва да бъде положително число!");
    }
    if (balance < amount) {
        throw std::runtime_error("Недостатъчна наличност в спестовната сметка!");
    }
    
    balance -= amount;
    transactions.push_back(Transaction("WITHDRAWAL", amount));
}

void SavingsAccount::applyInterest() {
    double interestEarned = balance * interestRate;
    if (interestEarned > 0) {
        balance += interestEarned;
        transactions.push_back(Transaction("INTEREST", interestEarned));
        std::cout << "[Лихва] Начислена лихва от " << interestEarned << " BGN към сметка " << iban << "\n";
    }
}

double SavingsAccount::getInterestRate() const { return interestRate; }