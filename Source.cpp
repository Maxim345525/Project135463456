#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <ctime>
using namespace std;

// ���� ��� ���������� ����������
class Transaction {
public:
    double amount;
    string category;
    time_t date;

    Transaction(double amt, const string& cat) : amount(amt), category(cat) {
        date = chrono::system_clock::to_time_t(chrono::system_clock::now());
    }
};

// ���� ��� ��������� ��������� ��������
class Account {
public:
    string name;
    double balance;
    vector<Transaction> transactions;

    Account(const string& accountName, double initialBalance) : name(accountName), balance(initialBalance) {}

    // ���������� �������
    void deposit(double amount) {
        balance += amount;
        cout << "Account balance " << name << " replenished by" << amount << ". New balance: " << balance << "\n";
    }

    // ������ �������
    void spend(double amount, const string& category) {
        if (amount > balance) {
            cout << "Insufficient funds to spend " << amount << ".\n";
            return;
        }
        balance -= amount;
        transactions.emplace_back(amount, category);
        cout << "spent " << amount << " per category " << category << ". New balance: " << balance << "\n";
    }

    // �������� ���������� � �������� �����
    vector<Transaction> getTransactions(time_t start, time_t end) const {
        vector<Transaction> result;
        for (const auto& t : transactions) {
            if (difftime(t.date, start) >= 0 && difftime(end, t.date) >= 0) {
                result.push_back(t);
            }
        }
        return result;
    }
};

// ���� ��� ��������� ���������
class FinanceManager {
private:
    map<string, Account> accounts; // ����� �������� ������ (��'� -> �������� �����)

public:
    // ������ �������� �����
    void addAccount(const string& name, double initialBalance) {
        accounts.emplace(name, Account(name, initialBalance));
        cout << "Account " << name << " attached with balance sheet" << initialBalance << ".\n";
    }

    // �������� �������� �����
    Account* getAccount(const string& name) {
        auto it = accounts.find(name);
        return it != accounts.end() ? &it->second : nullptr;
    }

    // ��� �� ����, ������� �� �����
    void generateReport(Account* account, time_t start, time_t end, const string& period) {
        vector<Transaction> transactions = account->getTransactions(start, end);
        map<string, double> categorySpending;

        cout << "Report for " << period << ":\n";
        for (const auto& t : transactions) {
            cout << "Category: " << t.category << ", Summ: " << t.amount << "\n";
            categorySpending[t.category] += t.amount;
        }

    }

    // ���-3 ������ �� ����, ������� �� �����
    void generateTopSpending(Account* account, time_t start, time_t end, const string& period) {
        vector<Transaction> transactions = account->getTransactions(start, end);
        sort(transactions.begin(), transactions.end(), [](const Transaction& a, const Transaction& b) {
            return b.amount > a.amount;
            });

        // ���� ���-3 ������
        cout << "TOP-3 expenses for " << period << ":\n";
        for (int i = 0; i < 3 && i < transactions.size(); i++) {
            const auto& t = transactions[i];
            cout << i + 1 << ". Summ: " << t.amount << ", Category: " << t.category << "\n";
        }
    }
};
// ������� ��� ���������� ������� ���, ����� �� �����
void calculatePeriodStartDates(time_t& startOfDay, time_t& startOfWeek, time_t& startOfMonth) {
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm* localTime = localtime(&now);
    localTime->tm_hour = 0;
    localTime->tm_min = 0;
    localTime->tm_sec = 0;

    startOfDay = mktime(localTime);
    startOfWeek = startOfDay - (localTime->tm_wday * 24 * 60 * 60);

    localTime->tm_mday = 1;
    startOfMonth = mktime(localTime);
}

int main() {
    FinanceManager manager;

    // ������ ������ ������
    manager.addAccount("Purse", 1000);
    manager.addAccount("Card", 2000);

    // �������� ������ ������
    Account* wallet = manager.getAccount("Purse");
    Account* card = manager.getAccount("Card");

    // ���������� �������
    wallet->deposit(500);

    // �������� ������
    wallet->spend(150, "Eat");
    wallet->spend(100, "Transport");
    card->spend(300, "Technologies");
    card->spend(50, "Entertainment");

    // ��������� ������� ���, ����� �� �����
    time_t startOfDay, startOfWeek, startOfMonth;
    calculatePeriodStartDates(startOfDay, startOfWeek, startOfMonth);

    // ��� �� ����, ������� �� �����
    manager.generateReport(wallet, startOfDay, startOfDay + 86400, "day");
    manager.generateReport(wallet, startOfWeek, startOfDay, "week");
    manager.generateReport(wallet, startOfMonth, startOfDay, "month");

    // ���-3 ������ �� ����, ������� �� �����
    manager.generateTopSpending(wallet, startOfDay, startOfDay + 86400, "day");
    manager.generateTopSpending(wallet, startOfWeek, startOfDay, "week");
    manager.generateTopSpending(wallet, startOfMonth, startOfDay, "month");

    return 0;
}
