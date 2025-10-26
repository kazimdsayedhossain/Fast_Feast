#include "merge.h"
#include <cstdlib> // for rand()

// Initialize static member
int payment::payment_count = 0;

void payment::set_payment(int id, float am, float d, float re) {
    payment_id = id;
    amount = am;
    due = d;
    refund_extra = re;
    payment_count++;

    string filename = "payment/paid_and_due.txt";
    ofstream fout(filename, ios::app);  // Append mode
    if(fout.is_open()) {
        fout << payment_id << " " << amount << " " << due << " " << refund_extra << endl;
        fout.close();
        cout << "Payment saved to file: " << filename << endl;
    } else {
        cerr << "Error opening file: " << filename << endl;
    }
}

void payment::process_payment(double to_pay, cart &user_selected) {
    int id;
    float due = 0, refund_extra = 0;
    cout << "\n--- PAYMENT PROCESSING ---\n";
    cout<<" \nEnter border number or 0 for if Guest Meal: \n";
    cin>>id;
    if(id==0)
    {
        guest_data((int)to_pay);
        return ;
    }
    if (!(Customer::searchCustomer(to_string(id)))) {
    cout << "Border not found!!!" << endl;
    return;
    }

    cout << "Total to pay: " << to_pay << " Taka\n";
    cout << "Enter amount given by customer: ";
    float given;
    cin >> given;

    if (given > to_pay)
        refund_extra = given - to_pay;
    else if (given < to_pay)
        due = to_pay - given;

    set_payment(id, to_pay, due, refund_extra);
    display_payment();
}

void payment::display_payment() const {
    cout << "\n-----Display Payment-----\n";
    cout << "Payment ID      : " << payment_id << endl;
    cout << "Amount          : " << amount << " Taka" << endl;
    cout << "Pending Balance : " << due << " Taka" << endl;
    cout << "Refund Extra    : " << refund_extra << " Taka" << endl;
    cout << "---------------------------" << endl;
}

void payment::show_payment_count() {
    cout << "Total Payments Recorded : " << payment_count << endl;
}

void show_all_record() {
    ifstream fin("payment/paid_and_due.txt");
    if (!fin.is_open()) {
        cout << "No payment records found.\n";
        return;
    }

    int id;
    float am, d, re;
    float total = 0;

    cout << "\n                            ==== All Payment Records ====\n\n";
    cout << "\n===========================================Details======================================\n";
    while (fin >> id >> am >> d >> re) {
        string c_name;
        if(id>99999)
        {
            c_name=getGuestName(id);
        }
        else c_name=getCustomerName(id);

            cout << "\n Border: " <<setw(8)<<left<< id
             << " Name: "<<setw(10)<<left<<c_name
             << " Amount: " <<setw(5)<<left<<am
             << " Pending Balance: " <<setw(5)<<left<< d
             << " Refund Extra: " << re;
             cout<< "\n________________________________________________________________________________________";
        total += am;
    }

    cout << "\nTotal Amount Collected : " << total << " Taka" << endl;

    fin.close();
}
