/*This is the menu option. Currently it does not require any input of customer data. After
processing the order, the total ammount will be passed to a function called make_payment()
and send the total ammount to pay as argument of it. Make sure that this function is availble
in the payment class and accessable. I request you to be careful when making any input.
Follow the input instructions,
1. Don't input character or any other thing other when asked for any numarical value.
2. Please don't input any unaccepted character at any phase.
3. When asked for numarical value or a character you should only input the instructed character
   or number.
You can contact me if you need any further clarification.
Don't make any changes to the existing code, if you need to add any function or class
please contact me first. Thank you.

Author: Kazi MD. Sayed Hossain
email: kazimdsayedhossain@outlook.com
Date: 19.10.2025
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cstdio>
#include <thread>
#include <chrono>

using namespace std;

class item;
class menu;
class cart;

static int menu_number = 0;

void access_menu(void);
void add_order(void);
void add_menu(void);
void select_menu(void);
void remove_menu(void);
void remove_item(void);

class item
{
private:
    string name;
    int price;

public:
    item(string s = "Unknown", int p = 0) : name(s), price(p) {}

    void set_name(string s)
    {
        name = s;
    }

    void set_price(int x)
    {
        price = x;
    }

    string get_name(void) const
    {
        return name;
    }

    int get_price(void) const
    {
        return price;
    }

    void show(void) const
    {
        cout << left << setw(20) << name
             << right << setw(10) << price << " Taka" << endl;
        cout << "_____________________________________" << endl;
    }
};

class menu
{
private:
    string fancy_name;
    item *all_items;
    int total_item;

public:
    menu(string f_n = "Regular", bool createFile = false) : total_item(0)
    {
        fancy_name = f_n;
        all_items = nullptr;
        if (createFile)
        {
            f_n = "./all_menu/" + f_n + ".txt";
            cout << f_n << endl;
            ofstream create_file(f_n);
            create_file.close();
        }
    }

    ~menu()
    {
        delete[] all_items;
    }

    menu(const menu &other)
    {
        fancy_name = other.fancy_name;
        total_item = other.total_item;

        if (total_item > 0)
        {
            all_items = new item[total_item];
            for (int i = 0; i < total_item; i++)
            {
                all_items[i] = other.all_items[i];
            }
        }
        else
        {
            all_items = nullptr;
        }
    }

    menu &operator=(const menu &other)
    {

        if (this == &other)
        {
            return *this;
        }
        delete[] all_items;

        fancy_name = other.fancy_name;
        total_item = other.total_item;

        if (total_item > 0)
        {
            all_items = new item[total_item];
            for (int i = 0; i < total_item; i++)
            {
                all_items[i] = other.all_items[i];
            }
        }
        else
        {
            all_items = nullptr;
        }

        return *this;
    }

    void add_item(string s = "Unknown", int p = 0)
    {
        increase_item_count();
        all_items[total_item - 1].set_name(s);
        all_items[total_item - 1].set_price(p);
        string fileName = "./all_menu/" + fancy_name + ".txt";
        ofstream outputFile(fileName, std::ios::app);
        outputFile << s << " " << p << endl;
    }

    void load_item(string s = "Unknown", int p = 0)
    {
        increase_item_count();
        all_items[total_item - 1].set_name(s);
        all_items[total_item - 1].set_price(p);
    }

    void increase_item_count(void)
    {
        item *temp_item = new item[total_item + 1];
        for (int i = 0; i < total_item; i++)
        {
            temp_item[i] = all_items[i];
        }
        delete[] all_items;
        all_items = temp_item;
        total_item++;
    }

    void show_menu(void) const
    {
        cout << "            All items " << endl;
        cout << "            " << fancy_name << endl
             << endl;
        cout << left << setw(4) << "ID"
             << left << setw(23) << "Name"
             << right << setw(10) << "Price" << endl
             << endl;
        for (int i = 0; i < total_item; i++)
        {
            cout << left << setw(4) << i + 1;
            all_items[i].show();
        }
    }

    item get_item(int idx) const
    {
        if (idx >= 0 && idx < total_item)
            return all_items[idx];
        return item();
    }

    friend void add_menu(void);
    friend void select_menu(void);
    friend void add_order(void);
    friend void remove_menu(void);
    friend void remove_item(void);
};

menu *all_menus = nullptr;

void load_menus()
{
    string dirPath = "./all_menu/";

    DIR *dir = opendir(dirPath.c_str());

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {

        if (entry->d_name[0] == '.')
            continue;

        string filename = entry->d_name;
        if (filename.substr(filename.find_last_of('.') + 1) == "txt")
        {

            string menu_name = filename.substr(0, filename.find_last_of('.'));

            menu_number++;
            menu *temp = new menu[menu_number];

            for (int i = 0; i < menu_number - 1; ++i)
            {
                temp[i] = all_menus[i];
            }

            delete[] all_menus;
            all_menus = temp;

            all_menus[menu_number - 1] = menu(menu_name, false);

            ifstream file(dirPath + filename);

            string item_name;
            int item_price;

            cout << "Reading menu file: " << filename << endl;
            while (file >> item_name >> item_price)
            {

                all_menus[menu_number - 1].load_item(item_name, item_price);
                // cout << "Loaded Item: " << item_name << ", Price: " << item_price << endl;
            }

            file.close();
        }
    }

    closedir(dir);
    cout << "Menus loaded successfully.\n";
}

class cart
{
private:
    int total_price;
    item *selected_items;
    int selected;

public:
    cart() : total_price(0), selected(0)
    {
        selected_items = nullptr;
    }

    ~cart()
    {
        delete[] selected_items;
    }

    cart(const cart &other)
    {
        total_price = other.total_price;
        selected = other.selected;
        if (selected > 0)
        {
            selected_items = new item[selected];
            for (int i = 0; i < selected; ++i)
            {
                selected_items[i] = other.selected_items[i];
            }
        }
        else
        {
            selected_items = nullptr;
        }
    }

    cart &operator=(const cart &other)
    {
        if (this == &other)
        {
            return *this;
        }
        delete[] selected_items;

        total_price = other.total_price;
        selected = other.selected;
        if (selected > 0)
        {
            selected_items = new item[selected];
            for (int i = 0; i < selected; ++i)
            {
                selected_items[i] = other.selected_items[i];
            }
        }
        else
        {
            selected_items = nullptr;
        }
        return *this;
    }

    void add_to_cart(item to_add)
    {
        increase_count();
        selected_items[selected - 1] = to_add;
        total_price += to_add.get_price();
    }

    void increase_count(void)
    {
        item *temp_item = new item[selected + 1];
        for (int i = 0; i < selected; i++)
        {
            temp_item[i] = selected_items[i];
        }
        delete[] selected_items;
        selected_items = temp_item;
        selected++;
    }

    int show_cart(void) const
    {
        cout << endl;
        cout << "           CART ITEMS" << endl
             << endl;
        for (int i = 0; i < selected; i++)
        {
            selected_items[i].show();
        }
        cout << left << setw(20) << "Total"
             << right << setw(10) << total_price << " Taka" << endl;
        return total_price;
    }
};

int main()
{
    while (true)
    {
        cout << "\n====== MAIN MENU ======\n";
        cout << "1. Access Menu Options\n";
        cout << "2. Exit\n";
        int choice;
        cin >> choice;

        if (choice == 1)
            access_menu();
        else if (choice == 2)
            break;
        else
            cout << "Invalid option.\n";
    }

    delete[] all_menus;
    return 0;
}

void access_menu(void)
{
    load_menus();
    while (true)
    {
        cout << "\n------ MENU OPTIONS ------\n";
        cout << "1. Add Order\n";
        cout << "2. Add Menu\n";
        cout << "3. Add Item to Existing Menu\n";
        cout << "4. Remove Menu\n";
        cout << "5. Remove item\n";
        cout << "6. Return to Main Menu\n";
        cout << "Choose an option: ";

        int input;
        cin >> input;

        switch (input)
        {
        case 1:
            add_order();
            break;
        case 2:
            add_menu();
            break;
        case 3:
            select_menu();
            break;
        case 4:
            remove_menu();
            break;
        case 5:
            remove_item();
            break;
        case 6:
            return;
        default:
            cout << "Wrong Input. Try again.\n";
        }
    }
}

void add_menu(void)
{
    string name;
    cout << "Enter menu name: ";
    cin >> name;

    menu_number++;
    menu *temp = new menu[menu_number];
    for (int i = 0; i < menu_number - 1; i++)
    {
        temp[i] = all_menus[i];
    }
    delete[] all_menus;
    all_menus = temp;
    all_menus[menu_number - 1] = menu(name, true);
    cout << "Menu added: " << name << endl;
}

void select_menu(void)
{
    if (menu_number == 0)
    {
        cout << "No menus available to select.\n";
        return;
    }

    cout << "Select a menu to add items to:\n";
    for (int i = 0; i < menu_number; i++)
    {
        cout << i + 1 << ". " << all_menus[i].fancy_name << endl;
    }

    int choice;
    cin >> choice;
    if (choice < 1 || choice > menu_number)
    {
        cout << "Invalid menu choice.\n";
        return;
    }

    menu &selected_menu = all_menus[choice - 1];

    while (true)
    {
        string item_name;
        int item_price;

        cout << "Enter item name (or 'q' to quit): ";
        cin >> item_name;
        if (item_name == "q")
            break;

        cout << "Enter item price: ";
        cin >> item_price;
        selected_menu.add_item(item_name, item_price);
        cout << "Item added to menu.\n";
    }
}

void add_order(void)
{
    if (menu_number == 0)
    {
        cout << "No menus available.\n";
        return;
    }

    cout << "Select a menu to order from:\n";
    for (int i = 0; i < menu_number; i++)
    {
        cout << i + 1 << ". " << all_menus[i].fancy_name << endl;
    }

    int choice;
    cin >> choice;
    if (choice < 1 || choice > menu_number)
    {
        cout << "Invalid menu choice.\n";
        return;
    }

    menu &selected_menu = all_menus[choice - 1];
    selected_menu.show_menu();

    cart user_cart;

    while (true)
    {
        int item_choice;
        cout << "Enter item ID to add to cart (0 to finish): ";
        cin >> item_choice;

        if (item_choice == 0)
            break;

        if (item_choice < 1 || item_choice > selected_menu.total_item)
        {
            cout << "Invalid item ID.\n";
            continue;
        }

        user_cart.add_to_cart(selected_menu.get_item(item_choice - 1));
        cout << "Item added to cart.\n";
    }

    int to_pay = user_cart.show_cart();
    char confirmation;
    cout << endl
         << "Press 1 to confirm order and pay or q to return: ";
    cin >> confirmation;
    if (confirmation == '1')
    {
        // make_payment(to_pay);
    }
    else
    {
        cout << "Order cancelled.\n";
    }
}

void remove_menu(void)
{
    if (menu_number == 0)
    {
        cout << "No menus available to select.\n";
        return;
    }

    cout << "Select a menu to remove from the list:\n";
    for (int i = 0; i < menu_number; i++)
    {
        cout << i + 1 << ". " << all_menus[i].fancy_name << endl;
    }
    int choice;
    cin >> choice;
    if (choice < 1 || choice > menu_number)
    {
        cout << "Invalid menu choice.\n";
        return;
    }
    string to_remove = all_menus[choice - 1].fancy_name;
    string dirPath = "./all_menu/";
    to_remove = dirPath + to_remove + ".txt";
    if (remove(to_remove.c_str()) == 0)
    {
        cout << "Menu file '" << to_remove << "' removed successfully.\n";
    }
    else
    {
        perror(("Error deleting file: " + to_remove).c_str());
    }

    cout << "Menu removed from the list.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    delete[] all_menus;
    all_menus = nullptr;
    menu_number = 0;
    void access_menu();
}

void remove_item(void)
{
    if (menu_number == 0)
    {
        cout << "No menus available to select.\n";
        return;
    }

    cout << "Select a menu to delete an item from:\n";
    for (int i = 0; i < menu_number; i++)
    {
        cout << i + 1 << ". " << all_menus[i].fancy_name << endl;
    }

    int mchoice;
    cin >> mchoice;
    if (mchoice < 1 || mchoice > menu_number)
    {
        cout << "Invalid menu choice.\n";
        return;
    }

    menu &sel = all_menus[mchoice - 1];

    if (sel.total_item == 0)
    {
        cout << "Selected menu has no items.\n";
        return;
    }

    sel.show_menu();
    cout << "Enter item ID to remove (0 to cancel): ";
    int id;
    cin >> id;
    if (id == 0)
    {
        cout << "Operation cancelled.\n";
        return;
    }
    if (id < 1 || id > sel.total_item)
    {
        cout << "Invalid item ID.\n";
        return;
    }

    int remove_idx = id - 1;

    int new_count = sel.total_item - 1;
    item *old_items = sel.all_items;
    item *new_items = nullptr;

    if (new_count > 0)
    {
        new_items = new item[new_count];
        int j = 0;
        for (int i = 0; i < sel.total_item; ++i)
        {
            if (i == remove_idx)
                continue;
            new_items[j++] = old_items[i];
        }
    }

    delete[] old_items;
    sel.all_items = new_items;
    sel.total_item = new_count;

    string fileName = "./all_menu/" + sel.fancy_name + ".txt";
    ofstream outputfile(fileName, ios::trunc);

    for (int i = 0; i < sel.total_item; ++i)
    {
        outputfile << sel.all_items[i].get_name() << " " << sel.all_items[i].get_price() << "\n";
    }
    outputfile.close();

    cout << "Item removed successfully from menu and file updated.\n";
}
