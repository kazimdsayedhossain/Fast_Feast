#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <filesystem>
#include "merge.h"

using namespace std;
namespace fs = std::filesystem;

// -------------------- Static Member Initialization --------------------
int Customer::customerCount = 0;

Customer::Customer() : name(""), border(""), address("") {}

// -------------------- Customer Member Function Definitions --------------------

// Helper to get file path
string Customer::getFilePath() {
    string folder = "all_customer";
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }
    return folder + "/customer.txt";
}

// Save customer to file
void Customer::saveToFile() {
    string path = getFilePath();
    ofstream file(path, ios::app);
    if (!file) {
        cout << "Error opening file: " << path << endl;
        return;
    }
    file << name << "|" << border << "|" << address << "\n";
    file.close();
    cout << "\n✅ Customer saved successfully in " << path << "!\n";
}

// View all customers
void Customer::viewCustomers() {
    string path = getFilePath();
    ifstream file(path);
    if (!file) {
        cout << "No customer records found.\n";
        return;
    }

    string n, p, a;
    cout << "\n-------------------- All Customers --------------------\n";
    cout << left << setw(20) << "Name" << setw(20) << "Border" << "Address\n";
    cout << "--------------------------------------------------------\n";

    while (getline(file, n, '|')) {
        getline(file, p, '|');
        getline(file, a);
        cout << left << setw(20) << n << setw(20) << p << a << endl;
    }
    cout << "--------------------------------------------------------\n";
    file.close();
}

// Search customer by border number
bool Customer::searchCustomer(const string &searchborder) {
    string path = getFilePath();
    ifstream file(path);
    if (!file) {
        cout << "No customer records found.\n";
        return false;
    }

    string n, p, a;
    while (getline(file, n, '|')) {
        getline(file, p, '|');
        getline(file, a);
        if (p == searchborder) {
            cout << "\nCustomer Found!\n";
            cout << "Name: " << n << "\nBorder: " << p << "\nAddress: " << a << endl;
            file.close();
            return true;
        }
    }

    cout << "\nNo customer found with border number: " << searchborder << endl;
    file.close();
    return false;
}

string getCustomerName(int id)
{
    string path = Customer::getFilePath();
    ifstream file(path);
    string n,p,a;
        while (getline(file, n, '|')) {
        getline(file, p, '|');
        getline(file, a);
        if (p == to_string(id)) {
            
            file.close();
            return n;
        }
    }
    return "Unknown";
}

// Delete customer by border number
void Customer::deleteCustomer(const string &deleteborder) {
    string path = getFilePath();
    ifstream file(path);
    if (!file) {
        cout << "No records to delete.\n";
        return;
    }

    ofstream temp("temp.txt");
    string n, p, a;
    bool deleted = false;

    while (getline(file, n, '|')) {
        getline(file, p, '|');
        getline(file, a);

        if (p != deleteborder) {
            temp << n << "|" << p << "|" << a << "\n";
        } else {
            deleted = true;
        }
    }

    file.close();
    temp.close();
    remove(path.c_str());
    rename("temp.txt", path.c_str());

    if (deleted)
        cout << "\n✅ Customer deleted successfully!\n";
    else
        cout << "\n⚠️ No customer found with that border number.\n";
}

// Update customer info
void Customer::updateCustomer(const string &updateborder) {
    string path = getFilePath();
    ifstream file(path);
    if (!file) {
        cout << "No records to update.\n";
        return;
    }

    ofstream temp("temp.txt");
    string n, p, a;
    bool updated = false;

    while (getline(file, n, '|')) {
        getline(file, p, '|');
        getline(file, a);

        if (p == updateborder) {
            cout << "\nEnter new details for this customer:\n";
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, n);
            cout << "Enter Border: ";
            getline(cin, p);
            cout << "Enter Address: ";
            getline(cin, a);
            updated = true;
        }

        temp << n << "|" << p << "|" << a << "\n";
    }

    file.close();
    temp.close();
    remove(path.c_str());
    rename("temp.txt", path.c_str());

    if (updated)
        cout << "\n✅ Customer updated successfully!\n";
    else
        cout << "\n⚠️ No customer found with that border number.\n";
}

// Show total number of customers
void Customer::totalCustomers() {
    cout << "\nTotal Customers (this session): " << customerCount << endl;
}

// -------------------- Friend & Operator Function Definitions --------------------
void showCustomer(Customer c) {
    cout << "\nCustomer Details:\n";
    cout << "Name: " << c.name << "\nBorder: " << c.border << "\nAddress: " << c.address << endl;
}

ostream& operator<<(ostream &out, Customer &c) {
    cout << "\nEnter Customer Name: ";
    cin.ignore();
    getline(cin, c.name);
    cout << "Enter Border Number: ";
    getline(cin, c.border);
    cout << "Enter Address: ";
    getline(cin, c.address);
    Customer::customerCount++;
    return out;
}

// -------------------- Customer Menu Function --------------------
void customer() {
    Customer c;
    int choice;
    string border;

    do {
        cout << "\n=========== Feast Management System: Customer Menu ===========";
        cout << "\n1. Add Customer";
        cout << "\n2. View All Customers";
        cout << "\n3. Search Customer by Border";
        cout << "\n4. Update Customer Info";
        cout << "\n5. Delete Customer";
        cout << "\n6. Show Total Customer Count";
        cout << "\n7. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << c;
            c.saveToFile();
            break;
        case 2:
            Customer::viewCustomers();
            break;
        case 3:
            cout << "Enter border number to search: ";
            cin >> border;
            Customer::searchCustomer(border);
            break;
        case 4:
            cout << "Enter border number to update: ";
            cin >> border;
            Customer::updateCustomer(border);
            break;
        case 5:
            cout << "Enter border number to delete: ";
            cin >> border;
            Customer::deleteCustomer(border);
            break;
        case 6:
            Customer::totalCustomers();
            break;
        case 7:
            cout << "\nExiting customer menu...\n";
            break;
        default:
            cout << "\nInvalid choice. Try again!\n";
        }

    } while (choice != 7);
}
