#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>

class Transaction {
private:
    std::string type;       // "DEPOSIT", "WITHDRAWAL" и т.н.
    double amount;          // Сума на транзакцията
    std::string timestamp;  // Дата и час на извършване

    // Помощен метод за взимане на текущото време като string
    std::string getCurrentTimestamp() const;

public:
    // Конструктор
    Transaction(std::string type, double amount);
    // Конструктор, използван при зареждане от файл (когато времето вече е записано)
    Transaction(std::string type, double amount, std::string timestamp);

    void display() const;

    // Константни гетери (Капсулация)
    std::string getType() const;
    double getAmount() const;
    std::string getTimestamp() const;
};

#endif // TRANSACTION_HPP