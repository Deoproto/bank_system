#ifndef BANK_SYSTEM_HPP
#define BANK_SYSTEM_HPP

#include <initializer_list> // Изрично включване най-отгоре
#include <vector>
#include "Account.hpp"

class BankSystem {
private:
    std::vector<Account*> accounts;

public:
    BankSystem();
    ~BankSystem();
    
    void run();
};

#endif // BANK_SYSTEM_HPP