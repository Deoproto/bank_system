#include "SavingsAccount.hpp"
#include <iostream>

// Конструктор: предава данните на базовия клас (Account) и инициализира лихвения процент
SavingsAccount::SavingsAccount(std::string iban, std::string owner, double initialBalance, double rate)
    : Account(iban, owner, initialBalance), interestRate(rate) {}

// Имплементация на депозит
void SavingsAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        transactions.push_back(Transaction("DEPOSIT", amount));
    }
}

// Имплементация на теглене (няма такса, но проверява за наличност)
void SavingsAccount::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        transactions.push_back(Transaction("WITHDRAWAL", amount));
    } else {
        std::cout << "[Грешка] Недостатъчна наличност в спестовната сметка.\n";
    }
}

// Специфичен метод за начисляване на лихва върху текущия баланс
void SavingsAccount::applyInterest() {
    double interestEarned = balance * interestRate;
    if (interestEarned > 0) {
        balance += interestEarned;
        transactions.push_back(Transaction("INTEREST", interestEarned));
        std::cout << "[Лихва] Начислена лихва от " << interestEarned << " BGN към сметка " << iban << "\n";
    }
}

// Гетер за лихвения процент
double SavingsAccount::getInterestRate() const { 
    return interestRate; 
}