#include <iostream>
#include <cstdlib>
#include <ctime> 
#include "merge.h"


int total_bill(int pay);
class guest_meal
{
    string name;
    string phone;
    string address;
    int to_pay;
    int id;
    public:
    guest_meal(string n, string p, string a,int pay=0):name(n),phone(p), address(a),to_pay(pay)
    {
        to_pay=total_bill(pay);
        cout<<"Total bill: "<<to_pay<<endl;
        id=writeData();
        saveFile();
    }
    string FilePath();
    int writeData(void);
    void saveFile();
    friend int total_bill(int pay);
};

string guest_meal::FilePath()
    {
    string folder = "all_customer";
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
    }
    return folder + "/guest.txt";
}
void guest_meal::saveFile(void)
{
    string path = FilePath();
    ofstream file(path, ios::app);
    if (!file) {
        cout << "Error opening file: " << path << endl;
        return;
    }
    file << name << "|" << id << "|" << address << "\n";
    file.close();
    cout << "\n✅ Guest saved successfully in " << path << "!\n";
}

int guest_meal::writeData(void)
{
    std::srand(std::time(nullptr));
    int generatedID = std::rand()%1000000;
    while(generatedID<=99999) generatedID = std::rand()%1000000;
    string filename = "payment/paid_and_due.txt";
    ofstream fout(filename, ios::app);
    if(fout.is_open()) {
    fout << generatedID << " " << to_pay << " " << 0 << " " << 0 << endl;
    fout.close();
    cout << "Payment saved to file: " << filename << endl;
    } else {
        cerr << "Error opening file: " << filename << endl;
    }
    return generatedID;
}
int total_bill(int pay)
{
        float charge;
        cout<<"Guest Meal charge (%): ";
        cin>>charge;
        return (charge/100)*pay;
}

void guest_data(int to_pay)
{
    string name,phone, address;
    cout<<"Enter name: ";
    cin>>name;
    cout<<"Enter contact number: ";
    cin>>phone;
    cout<<"Enter address: ";
    cin>>address;

    guest_meal guestADD(name,phone,address);
}

string getGuestName(int id)
{

    string path = "all_customer/guest.txt";
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