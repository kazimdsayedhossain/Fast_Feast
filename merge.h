#ifndef MERGE_H
#define MERGE_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// -------------------- Customer Class --------------------

class Customer {
private:
    string name;
    string border;
    string address;
    static int customerCount;
    

public:
    Customer();
    void saveToFile();
    static void viewCustomers();
    static bool searchCustomer(const string &searchborder);
    static void deleteCustomer(const string &deleteborder);
    static void updateCustomer(const string &updateborder);
    static void totalCustomers();
    static string getFilePath();

    friend void showCustomer(Customer c);
    friend ostream& operator<<(ostream &out, Customer &c);
    friend string getCustomerName(int id);
};

// Friend & operator functions
string getCustomerName(int id);
void showCustomer(Customer c);
ostream& operator<<(ostream &out, Customer &c);

// -------------------- Payment Class --------------------
class cart; // Forward declaration

class payment {
private:
    int payment_id;
    float amount;
    float due;
    float refund_extra;
    static int payment_count;

public:
    void set_payment(int id, float am, float d, float re);
    void process_payment(double to_pay, cart &user_selected);
    void display_payment() const;
    static void show_payment_count();
    friend void show_all_record();
};

class guest_meal;

string getGuestName(int id);
void guest_data(int to_pay);

// Friend function for payment
void show_all_record();

// Optional menu function for customer
void customer();

#endif // MERGE_H
