#include "Account.hpp"

// Конструктор с инициализиращ списък (OOP_Lesson3)
Account::Account(std::string iban, std::string owner, double initialBalance)
    : iban(iban), owner(owner), balance(initialBalance) {}

// Празен виртуален деструктор
Account::~Account() {}

// Имплементация на гетерите
std::string Account::getIBAN() const { return iban; }
std::string Account::getOwner() const { return owner; }
double Account::getBalance() const { return balance; }

const std::vector<Transaction>& Account::getTransactions() const {
    return transactions;
}
void Account::addTransaction(const Transaction& tx) {
    transactions.push_back(tx);
}