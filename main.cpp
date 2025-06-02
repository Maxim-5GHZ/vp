#include "Models/Menu/CMenu.h"
#include "Models/Menu/CMenuItem.h"
#include "Models/User/Employee.h"
#include "Models/User/Client.h"
#include "Models/User/Deposit.h"
#include "Models/myVector/myVector.h"
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <memory>

using namespace std;

// Use shared_ptr for automatic memory management
vector<shared_ptr<Employee>> employees;
vector<shared_ptr<Client>> clients;
vector<Deposit> deposits;

// Password hashing function
string hashPassword(const string& password) {
    return to_string(hash<string>{}(password));
}

int addEmployee() {
    try {
        string first, last, login, pass, position;
        int age;
        
        cout << "Enter first name: ";
        cin >> first;
        cout << "Enter last name: ";
        cin >> last;
        cout << "Enter age: ";
        cin >> age;
        cout << "Enter login: ";
        cin >> login;
        cout << "Enter password: ";
        cin >> pass;
        cout << "Enter position: ";
        cin.ignore();
        getline(cin, position);

        employees.push_back(make_shared<Employee>(first, last, age, login, hashPassword(pass), position));
        cout << "Employee added successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}

int addClient() {
    try {
        string first, last, login, pass;
        int age;
        double deposit;
        
        cout << "Enter first name: ";
        cin >> first;
        cout << "Enter last name: ";
        cin >> last;
        cout << "Enter age: ";
        cin >> age;
        cout << "Enter login: ";
        cin >> login;
        cout << "Enter password: ";
        cin >> pass;
        cout << "Enter initial deposit: ";
        cin >> deposit;

        clients.push_back(make_shared<Client>(
            first, last, age, login, hashPassword(pass), deposit));
        cout << "Client added successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}

int addDeposit() {
    if (clients.empty()) {
        cout << "No clients available!" << endl;
        return 0;
    }

    try {
        cout << "Select client:" << endl;
        for (size_t i = 0; i < clients.size(); ++i) {
            cout << i << ". " << *clients[i] << endl;
        }

        size_t index;
        while (true) {
            cout << "Enter client index: ";
            if (cin >> index && index < clients.size()) {
                break;
            }
            cout << "Invalid index!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        string name;
        double amount, rate;
        
        cout << "Enter deposit name: ";
        cin >> name;
        cout << "Enter amount: ";
        cin >> amount;
        cout << "Enter interest rate: ";
        cin >> rate;

        deposits.emplace_back(name, clients[index].get(), amount, rate);
        cout << "Deposit added successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}

int deleteEmployee() {
    if (employees.empty()) {
        cout << "No employees to delete!" << endl;
        return 0;
    }

    try {
        for (size_t i = 0; i < employees.size(); ++i) {
            cout << i << ". " << *employees[i] << endl;
        }

        size_t index;
        while (true) {
            cout << "Enter index: ";
            if (cin >> index && index < employees.size()) {
                break;
            }
            cout << "Invalid index!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        employees.erase(employees.begin() + index);
        cout << "Employee deleted successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}

int deleteClient() {
    if (clients.empty()) {
        cout << "No clients to delete!" << endl;
        return 0;
    }

    try {
        for (size_t i = 0; i < clients.size(); ++i) {
            cout << i << ". " << *clients[i] << endl;
        }

        size_t index;
        while (true) {
            cout << "Enter index: ";
            if (cin >> index && index < clients.size()) {
                break;
            }
            cout << "Invalid index!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Remove associated deposits
        auto clientPtr = clients[index].get();
        deposits.erase(remove_if(deposits.begin(), deposits.end(), 
            [clientPtr](const Deposit& d) { return d.getClient() == clientPtr; }), 
            deposits.end());

        clients.erase(clients.begin() + index);
        cout << "Client and associated deposits deleted successfully!" << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}

void displayAllClients() {
    if (clients.empty()) {
        cout << "No clients available!" << endl;
        return;
    }
    for (const auto& client : clients) {
        cout << *client << endl;
    }
}

int sortClients() {
    sort(clients.begin(), clients.end(), 
        [](const shared_ptr<Client>& a, const shared_ptr<Client>& b) {
            return *a < *b;
        });
    cout << "Clients sorted successfully!" << endl;
    displayAllClients();
    return 0;
}

int displayDeposits() {
    if (deposits.empty()) {
        cout << "No deposits available!" << endl;
        return 0;
    }
    for (const auto& deposit : deposits) {
        cout << deposit << endl;
    }
    return 0;
}

int sortDeposits() {
    sort(deposits.begin(), deposits.end());
    cout << "Deposits sorted by amount:" << endl;
    displayDeposits();
    return 0;
}

int findDepositsByClient() {
    if (clients.empty()) {
        cout << "No clients available!" << endl;
        return 0;
    }

    displayAllClients();
    size_t index;
    while (true) {
        cout << "Enter client index: ";
        if (cin >> index && index < clients.size()) {
            break;
        }
        cout << "Invalid index!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    auto clientPtr = clients[index].get();
    bool found = false;
    for (const auto& deposit : deposits) {
        if (deposit.getClient() == clientPtr) {
            cout << deposit << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No deposits found for this client." << endl;
    }
    return 0;
}

int main() {
    CMenuItem items[] = {
        CMenuItem("Add Employee", addEmployee),
        CMenuItem("Delete Employee", deleteEmployee),
        CMenuItem("Add Client", addClient),
        CMenuItem("Delete Client", deleteClient),
        CMenuItem("Sort Clients", sortClients),
        CMenuItem("List All Clients", []() { displayAllClients(); return 0; }),
        CMenuItem("Add Deposit", addDeposit),
        CMenuItem("Display Deposits", displayDeposits),
        CMenuItem("Sort Deposits", sortDeposits),
        CMenuItem("Find Deposits by Client", findDepositsByClient),
        CMenuItem("Exit", []() { return 1; })
    };

    CMenu menu("Bank Management System", items, sizeof(items)/sizeof(items[0]));

    while (true) {
        try {
            if (menu.runCommand() == 1) break;
        } catch (const exception& e) {
            cerr << "System error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return 0;
}