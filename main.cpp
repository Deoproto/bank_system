#include <iostream>
#include <clocale>
#include <limits>
#include <string>
#include "BankSystem.hpp"

bool readLine(const std::string& prompt, std::string& value) {
    std::cout << prompt;
    std::getline(std::cin, value);
    return !std::cin.fail();
}

bool readDouble(const std::string& prompt, double& value) {
    std::cout << prompt;
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

void printMenu() {
    std::cout << "\n=========================================\n";
    std::cout << "          БАНКОВА СИСТЕМА v1.2           \n";
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
    std::cout << "10. Изход от програмата (Записване)\n";
    std::cout << "=========================================\n";
    std::cout << "Изберете опция (1-10): ";
}

int main() {
    if (!std::setlocale(LC_ALL, "bg_BG.UTF-8")) {
        std::setlocale(LC_ALL, "");
    }
    BankSystem bank;
    
    // СТЪПКА 1: Автоматично зареждаме старите данни от файловете при стартиране
    bank.loadFromFile();

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

        std::cin.ignore(); 

        if (choice == 10) {
            // СТЪПКА 2: Автоматично записваме всичко във файловете преди изход
            bank.saveToFile();
            std::cout << "\nБлагодарим ви, че използвахте нашата банка. Довиждане!\n";
            break;
        }

        switch (choice) {
            case 1:
                if (!readLine("Въведете IBAN: ", iban) || !readLine("Въведете Име на титуляр: ", owner)) {
                    std::cout << "[Грешка] Невалидни текстови данни. Опитайте отново.\n";
                    break;
                }
                if (!readDouble("Първоначален баланс: ", amount) || !readDouble("Такса за транзакция (напр. 2.00): ", fee)) {
                    std::cout << "[Грешка] Невалидна числова стойност. Опитайте отново.\n";
                    break;
                }
                bank.openCheckingAccount(iban, owner, amount, fee);
                break;

            case 2:
                if (!readLine("Въведете IBAN: ", iban) || !readLine("Въведете Име на титуляр: ", owner)) {
                    std::cout << "[Грешка] Невалидни текстови данни. Опитайте отново.\n";
                    break;
                }
                if (!readDouble("Първоначален баланс: ", amount) || !readDouble("Лихвен процент (напр. 0.03 за 3%): ", rate)) {
                    std::cout << "[Грешка] Невалидна числова стойност. Опитайте отново.\n";
                    break;
                }
                bank.openSavingsAccount(iban, owner, amount, rate);
                break;

            case 3:
                if (!readLine("Въведете IBAN на сметката за затваряне: ", iban)) {
                    std::cout << "[Грешка] Невалиден IBAN. Опитайте отново.\n";
                    break;
                }
                bank.closeAccount(iban);
                break;

            case 4:
                if (!readLine("Въведете IBAN: ", iban) || !readDouble("Сума за депозит: ", amount)) {
                    std::cout << "[Грешка] Невалидни данни за депозит. Опитайте отново.\n";
                    break;
                }
                bank.performDeposit(iban, amount);
                break;

            case 5:
                if (!readLine("Въведете IBAN: ", iban) || !readDouble("Сума за теглене: ", amount)) {
                    std::cout << "[Грешка] Невалидни данни за теглене. Опитайте отново.\n";
                    break;
                }
                bank.performWithdraw(iban, amount);
                break;

            case 6:
                if (!readLine("Въведете IBAN на изпращача: ", iban) || !readLine("Въведете IBAN на получателя: ", toIban) || !readDouble("Сума за превод: ", amount)) {
                    std::cout << "[Грешка] Невалидни данни за превод. Опитайте отново.\n";
                    break;
                }
                bank.performTransfer(iban, toIban, amount);
                break;

            case 7:
                if (!readLine("Въведете IBAN на спестовната сметка: ", iban)) {
                    std::cout << "[Грешка] Невалиден IBAN. Опитайте отново.\n";
                    break;
                }
                bank.performApplyInterest(iban);
                break;

            case 8:
                if (!readLine("Въведете IBAN на сметката за справка: ", iban)) {
                    std::cout << "[Грешка] Невалиден IBAN. Опитайте отново.\n";
                    break;
                }
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