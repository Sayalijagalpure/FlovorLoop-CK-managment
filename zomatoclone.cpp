#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <conio.h>   // For _getch()
#include <thread>    // For sleep
#include <chrono>    // For time delays
using namespace std;

// ===== Base Entity =====
class Entity {
public:
    string name;
    Entity(const string& n) : name(n) {}
    virtual void display() const = 0;
};

// ===== Restaurant =====
class Restaurant : public Entity {
public:
    vector<string> menu;
    vector<double> prices;

    Restaurant(const string& n, const vector<string>& m, const vector<double>& p)
        : Entity(n), menu(m), prices(p) {}

    void displayMenu() const {
        cout << "\nMenu of " << name << ":\n";
        for (size_t i = 0; i < menu.size(); i++) {
            cout << i + 1 << ". " << menu[i] << " - Rs. " << prices[i] << "\n";
        }
    }

    void display() const override {
        cout << "Restaurant: " << name << "\n";
    }
};

// ===== Order =====
class Order : public Entity {
public:
    vector<string> items;
    double total;
    string paymentMode;
    string transactionId;

    Order(const string& r, const vector<string>& i, double t, const string& pm, const string& tid)
        : Entity(r), items(i), total(t), paymentMode(pm), transactionId(tid) {}

    void display() const override {
        cout << "\nYour Current Order (" << name << "):\n";
        for (auto& item : items) {
            cout << "- " << item << "\n";
        }
        cout << "Total: Rs. " << total << "\n";
        cout << "Payment Mode: " << paymentMode << "\n";
        if (!transactionId.empty()) {
            cout << "Transaction ID: " << transactionId << "\n";
        }
    }
};

// ===== User & Customer =====
class User {
protected:
    string username;
    string password;

public:
    User(const string& u, const string& p) : username(u), password(p) {}

    bool login(const string& u, const string& p) const {
        return username == u && password == p;
    }

    string getUsername() const { return username; }
};

class Customer : public User {
    vector<Order> myOrders;

public:
    Customer(const string& u, const string& p) : User(u, p) {}

    void addOrder(const Order& order) {
        myOrders.push_back(order);
    }

    void showOrders() const {
        if (myOrders.empty()) {
            cout << "No orders yet.\n";
            return;
        }
        for (auto& order : myOrders) {
            order.display();
        }
    }
};

// ===== Zomato Clone App =====
class ZomatoClone {
    vector<Restaurant> restaurants;
    unordered_map<string, Customer> customers;
    Customer* currentUser = nullptr;
    int transactionCounter = 1000;

    string inputHiddenPassword() {
        string pass;
        char ch;
        while ((ch = _getch()) != '\r') { // Enter key
            if (ch == '\b') { // backspace
                if (!pass.empty()) {
                    pass.pop_back();
                    cout << "\b \b";
                }
            } else {
                pass.push_back(ch);
                cout << '*';
            }
        }
        cout << "\n";
        return pass;
    }

public:
    void addRestaurant(const Restaurant& r) {
        restaurants.push_back(r);
    }

    void registerCustomer(const string& u) {
        if (customers.find(u) != customers.end()) {
            cout << "Username already exists! Login to continue.\n";
            return;
        }
        cout << "Enter password: ";
        string p = inputHiddenPassword();
        customers.emplace(u, Customer(u, p));
        cout << "Registered successfully!\n";
        loginCustomer(u);
    }

    void loginCustomer(const string& u) {
        auto it = customers.find(u);
        if (it == customers.end()) {
            cout << "No such user. Please register first.\n";
            return;
        }
        cout << "Enter password: ";
        string p = inputHiddenPassword();
        if (it->second.login(u, p)) {
            currentUser = &(it->second);
            cout << "Welcome, " << u << "!\n";
        } else {
            cout << "Invalid login.\n";
        }
    }

    void logoutCustomer() {
        if (currentUser) {
            cout << "Logged out " << currentUser->getUsername() << ".\n";
            currentUser = nullptr;
        }
    }

    void showRestaurants() const {
        cout << "\nAvailable Restaurants:\n";
        for (size_t i = 0; i < restaurants.size(); i++) {
            cout << i + 1 << ". " << restaurants[i].name << "\n";
        }
    }

    void placeOrder() {
        if (!currentUser) {
            cout << "Login first!\n";
            return;
        }

        showRestaurants();
        cout << "Choose restaurant number: ";
        int rChoice;
        cin >> rChoice;
        if (rChoice < 1 || rChoice > restaurants.size()) {
            cout << "Invalid choice.\n";
            return;
        }

        const Restaurant& r = restaurants[rChoice - 1];
        vector<string> items;
        double total = 0;

        while (true) {
            r.displayMenu();
            cout << "Select item number (0 to finish): ";
            int choice;
            cin >> choice;
            if (choice == 0) break;
            if (choice >= 1 && choice <= r.menu.size()) {
                items.push_back(r.menu[choice - 1]);
                total += r.prices[choice - 1];
            } else {
                cout << "Invalid item choice.\n";
            }
        }

        if (items.empty()) {
            cout << "No items selected.\n";
            return;
        }

        cout << "\nYour total bill is: Rs. " << total << "\n";

        cout << "\nChoose Payment Mode:\n1. Online (UPI)\n2. Cash on Delivery\nEnter choice: ";
        int payChoice;
        cin >> payChoice;
        string paymentMode, transactionId = "";
        if (payChoice == 1) {
            string upi;
            cout << "Enter your UPI ID: ";
            cin >> upi;
            transactionCounter++;
            transactionId = "TXN" + to_string(transactionCounter);
            paymentMode = "Online (UPI: " + upi + ")";
        } else {
            paymentMode = "Cash on Delivery";
        }

        Order newOrder(r.name, items, total, paymentMode, transactionId);
        newOrder.display();

        int action;
        while (true) {
            cout << "\n1. Confirm Order\n2. Add More Items\n3. Cancel Order\nChoose option: ";
            cin >> action;

            if (action == 1) {
                cout << "Processing payment...\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                cout << "Payment successful ✅\n";
                cout << "Order Accepted! 🎉\n";
                currentUser->addOrder(newOrder);
                break;
            } else if (action == 2) {
                r.displayMenu();
                cout << "Select extra item (0 to stop): ";
                int extra;
                cin >> extra;
                if (extra >= 1 && extra <= r.menu.size()) {
                    newOrder.items.push_back(r.menu[extra - 1]);
                    newOrder.total += r.prices[extra - 1];
                    cout << "Item added.\n";
                    cout << "Updated total bill: Rs. " << newOrder.total << "\n";
                    newOrder.display();
                } else if (extra == 0) {
                    cout << "No more items added.\n";
                } else {
                    cout << "Invalid choice.\n";
                }
            } else if (action == 3) {
                cout << "Order cancelled.\n";
                break;
            } else {
                cout << "Invalid option.\n";
            }
        }
    }

    void showMyOrders() const {
        if (!currentUser) {
            cout << "Login first!\n";
            return;
        }
        currentUser->showOrders();
    }
};

// ===== Main =====
int main() {
    ZomatoClone app;
    app.addRestaurant(Restaurant("Pizza Palace", {"Pizza", "Pasta"}, {250, 180}));
    app.addRestaurant(Restaurant("Burger Hub", {"Burger", "Fries"}, {120, 60}));

    while (true) {
        cout << "\n--- Zomato Clone ---\n";
        cout << "1. Register\n2. Login\n3. Show Restaurants\n4. Place Order\n5. Show My Orders\n6. Logout\n7. Exit\n";
        cout << "Enter choice: ";
        int ch;
        cin >> ch;

        string user;
        switch (ch) {
            case 1:
                cout << "Enter username: ";
                cin >> user;
                app.registerCustomer(user);
                break;
            case 2:
                cout << "Enter username: ";
                cin >> user;
                app.loginCustomer(user);
                break;
            case 3:
                app.showRestaurants();
                break;
            case 4:
                app.placeOrder();
                break;
            case 5:
                app.showMyOrders();
                break;
            case 6:
                app.logoutCustomer();
                break;
            case 7:
                cout << "Exiting... Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
}
