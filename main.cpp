#include <iostream>
#include <clocale>
#include <string>
#include "BankSystem.hpp"

void printMenu() {
    std::cout << "\n=========================================\n";
    std::cout << "          БАНКОВА СИСТЕМА v1.1           \n";
    std::cout << "=========================================\n";
    std::cout << "1. Отваряне на Разплащателна сметка\n";
    std::cout << "2. Отваряне на Спестовна сметка\n";
    std::cout << "3. Затваряне на сметка\n";
    std::cout << "4. Депозиране на сума\n";
    std::cout << "5. Теглене на сума\n";
    std::cout << "6. Вътрешен трансфер (Превод)\n";
    std::cout << "7. Начисляване на лихва (Само за Спестовна)\n";
    std::cout << "8. Справка / История на транзакциите\n";
    std::cout << "9. Показване на всички сметки\n";
    std::cout << "10. Изход от програмата\n";
    std::cout << "=========================================\n";
    std::cout << "Изберете опция (1-10): ";
}

int main() {
    std::setlocale(LC_ALL, "bg_BG.UTF-8");
    BankSystem bank;
    
    // Автоматични начални данни за бърз тест
    bank.openCheckingAccount("BG11BNKB1111", "Петър Петров", 500.0, 2.00);
    bank.openSavingsAccount("BG22BNKB2222", "Елена Георгиева", 1500.0, 0.03); // 3% лихва

    int choice = 0;
    std::string iban, owner, toIban;
    double amount, fee, rate;

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cout << "[Грешка] Моля, въведете валидно число!\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        std::cin.ignore(); // Изчистваме буфера

        if (choice == 10) {
            std::cout << "\nБлагодарим ви, че използвахте нашата банка. Довиждане!\n";
            break;
        }

        switch (choice) {
            case 1:
                std::cout << "Въведете IBAN: "; std::getline(std::cin, iban);
                std::cout << "Въведете Име на титуляр: "; std::getline(std::cin, owner);
                std::cout << "Първоначален баланс: "; std::cin >> amount;
                std::cout << "Такса за транзакция (напр. 2.00): "; std::cin >> fee;
                bank.openCheckingAccount(iban, owner, amount, fee);
                break;

            case 2:
                std::cout << "Въведете IBAN: "; std::getline(std::cin, iban);
                std::cout << "Въведете Име на титуляр: "; std::getline(std::cin, owner);
                std::cout << "Първоначален баланс: "; std::cin >> amount;
                std::cout << "Лихвен процент (напр. 0.03 за 3%): "; std::cin >> rate;
                bank.openSavingsAccount(iban, owner, amount, rate);
                break;

            case 3:
                std::cout << "Въведете IBAN на сметката за затваряне: "; std::getline(std::cin, iban);
                bank.closeAccount(iban);
                break;

            case 4:
                std::cout << "Въведете IBAN: "; std::getline(std::cin, iban);
                std::cout << "Сума за депозит: "; std::cin >> amount;
                bank.performDeposit(iban, amount);
                break;

            case 5:
                std::cout << "Въведете IBAN: "; std::getline(std::cin, iban);
                std::cout << "Сума за теглене: "; std::cin >> amount;
                bank.performWithdraw(iban, amount);
                break;

            case 6:
                std::cout << "Въведете IBAN на изпращача: "; std::getline(std::cin, iban);
                std::cout << "Въведете IBAN на получателя: "; std::getline(std::cin, toIban);
                std::cout << "Сума за превод: "; std::cin >> amount;
                bank.performTransfer(iban, toIban, amount);
                break;

            case 7:
                std::cout << "Въведете IBAN на спестовната сметка: "; std::getline(std::cin, iban);
                bank.performApplyInterest(iban);
                break;

            case 8:
                std::cout << "Въведете IBAN на сметката за справка: "; std::getline(std::cin, iban);
                bank.displayAccountHistory(iban);
                break;

            case 9:
                bank.displayAllAccounts();
                break;

            default:
                std::cout << "[Грешка] Невалидна опция. Изберете между 1 и 10.\n";
                break;
        }
    }

    return 0;
}