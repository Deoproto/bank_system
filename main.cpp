#include <iostream>
#include "Transaction.hpp"

int main() {
    std::cout << "--- Testing Transaction Class ---" << std::endl;

    // Създаваме обект транзакция за депозит от 150.50 лв.
    Transaction t1("DEPOSIT", 150.50);

    // Извеждаме данните чрез публичния интерфейс (гетерите)
    std::cout << "Type:      " << t1.getType() << std::endl;
    std::cout << "Amount:    " << t1.getAmount() << " BGN" << std::endl;
    std::cout << "Timestamp: " << t1.getTimestamp() << std::endl;

    return 0;
}