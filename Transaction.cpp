#include "Transaction.hpp"
#include <iostream>
#include <ctime>

// Автоматично генериране на текущото време при нова транзакция
std::string Transaction::getCurrentTimestamp() const {
    std::time_t now = std::time(nullptr);
    char buf[20];
    // Форматира времето във вид: ГГГГ-ММ-ДД ЧЧ:ММ:СС
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

// Конструктор за нови транзакции (в реално време)
Transaction::Transaction(std::string type, double amount) 
    : type(type), amount(amount) {
    this->timestamp = getCurrentTimestamp();
}

// Конструктор за транзакции, прочетени от база данни / файл
Transaction::Transaction(std::string type, double amount, std::string timestamp)
    : type(type), amount(amount), timestamp(timestamp) {}

// Имплементация на гетерите
std::string Transaction::getType() const { return type; }
double Transaction::getAmount() const { return amount; }
std::string Transaction::getTimestamp() const { return timestamp; }

// Добави този метод най-отдолу в Transaction.cpp:
void Transaction::display() const {
    std::cout << "  [" << timestamp << "] " 
              << type << ": " 
              << amount << " BGN\n";
}