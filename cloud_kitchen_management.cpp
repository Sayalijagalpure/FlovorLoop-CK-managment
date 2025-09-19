#include <iostream>
#include <string>        // to store words
#include <vector>        // to resize an array
#include <map>           //to share key value pairs (search a word based on key)
#include <unordered_map> //Fast lookup of customers by username for login
#include <conio.h>       //to hide password input
#include <thread>        //to allow multi tasking
#include <chrono>        //to handle time(delays)
#include <fstream>      //to handle file operations
using namespace std;

class Entity // base class1
{
public:
  string name;
  Entity(const string &n) : name(n) {} // parameterized constructor
  virtual void display() const = 0;    // virtual function (polymorphism included)
};

class Kitchens : public Entity // derived class1(from base class1)
{
public:
  vector<string> menu;                                                       // a dictinary to add or remove items
  vector<double> price;                                                      // a dictionary to store price of each item
  Kitchens(const string n, const vector<string> &m, const vector<double> &p) // parameterized constructor used const to only read , used & to avoid copying of data
      : Entity(n), menu(m), price(p)
  {
  } // member initializer list

  void displayMenu() const // function to display menu
  {
    cout << "\nMenu of" << name << endl;
    for (size_t i = 0; i < menu.size(); i++) // for loop (size_t is used to avoid negative values)
    {
      cout << i + 1 << ". " << menu[i] << " - Rs. " << price[i] << endl; // i+1 to start from 1 instead of 0
    }
  }

  void display() const override // use of polymorphism to override base class function
  {
    cout << " Cloud Kitchen outlets: " << name << endl;
  }
};

// Move Orders, User, Customer, and CloudKitchen classes to global scope

class Orders : public Entity // derived class2(from base class1)
{
public:
  vector<string> items; // to store ordered items
  double total;         // to store total price of order
  string paymentMode;   // to store payment mode
  string transactionId; // to store transaction id for online payment

  Orders(const string &r, const vector<string> &i, double t, const string &pm, const string &tid) // parameterized constructor
      : Entity(r), items(i), total(t), paymentMode(pm), transactionId(tid)
  {
  } // member initializer list

  void display() const override // use of polymorphism to override base class function
  {
    cout << "\nYour current order(" << name << ")" << endl;
    for (auto &item : items)
    {
      cout << " ◉ " << item << endl;
    }
    cout << "Total: Rs. " << total << endl;
    cout << "Payment Mode: " << paymentMode << endl;
    if (!transactionId.empty()) // useing Logical NOT operator to check if transaction Id is not empty
    {
      cout << "Transaction ID: " << transactionId << endl;
    }
  }
  void saveOrder(const Orders& order, const string& username) {
  ofstream fout("orders.txt", ios::app);
  fout << "Customer: " << username << "\n";
  fout << "Restaurant: " << order.name << "\n";
  fout << "Items: ";
  for (auto& item : order.items) fout << item << ", ";
  fout << "\nTotal: Rs. " << order.total << "\n";
  fout << "Transaction ID: " << order.transactionId << "\n";
  fout << "-----------------------\n";
  fout.close();
}

};

class User // base class2
{
protected:
  string username;
  string password;

public:
  User(const string &u, const string &p) : username(u), password(p) {} // parameterized constructor with member initializer list
  bool login(const string &u, const string &p) const
  {
    return username == u && password == p; // using logical AND operator to check both username and password
  }
  string getUsername() const
  {
    return username;
  }
};

class Customer : public User // derived class3(from base class2)
{
public:
  vector<Orders> myOrders;                                   // to store all orders of a customer
  Customer(const string &u, const string &p) : User(u, p) {} // parameterized constructor with member initializer list
  void addOrder(const Orders &order)                         // function to add order
  {
    myOrders.push_back(order); // push_back function to add order at the end of vector
  }
  void showOrders() const // function to show all orders
  {
    if (myOrders.empty()) // function to check if vector is empty
    {
      cout << "No orders yet." << endl;
      return;
    }
    for (auto &order : myOrders) // range based for loop to iterate through all orders
    {
      order.display(); // display function to show order details
    }
  }
}; // <-- Properly close Customer class here

// Removed duplicate CloudKitchen class definition

class CloudKitchen // base class3( main class of the application)
{
  vector<Kitchens> kitchens;                 // to store list of all outlets available in the application
  unordered_map<string, Customer> customers; // to store list of all registered customers
  Customer *currentUser = nullptr;           // a pointer to a customer object to keep track of currently logged in user(nullptr is used to indicate no user is logged in)
  int transactionCounter = 1000;             // to generate unique transaction IDs for online payments

public:
  void addKitchen(const Kitchens& kitchen) {
    kitchens.push_back(kitchen);
  }

  
  void saveCustomer(const string& user, const string& pass) {
    ofstream fout("customers.txt", ios::app); // append mode
    fout << user << " " << pass << "\n";      // write into file
    fout.close();
}
void loadCustomers() {
    ifstream fin("customers.txt");
    string u, p;
    while (fin >> u >> p) {
        customers.emplace(u, Customer(u, p));
    }
    fin.close();
}


  string inputHiddenPassword() // string function to take password input without dsplaying it on screen
  {
    string pass;
    char ch;
    while ((ch = _getch()) != '\r') // loop until the user presses Enter key('\r)
    {
      if (ch == '\b') // checks if the user presses backspace key('\b)
      {
        if (!pass.empty()) // if password is not empty
        {
          pass.pop_back(); // remove last character from password
          cout << "\b \b"; // move cursor back, print space to erase character, move cursor back again
        }
      }
      else
      {
        pass.push_back(ch); // add character to password
        cout << "*";        // print * to indicate a characte has been entered
      }
    }
    cout << endl; // move to next line after user presses Enter key
    return pass;  // return the entered password
  }

  void registerCustomer(const string &u) // function to register a new customer
  {
    if (customers.find(u) != customers.end()) //.find(u) checks if username already exists in the map, .end() represents "not found"
    {
      cout << "Username already exists! Login to continue." << endl;
      return; // return to main function if username already exists
    }
    cout << "Enter password: ";
    string p = inputHiddenPassword();     // password is saved in string p
    customers.emplace(u, Customer(u, p)); // emplace inserts object(customer)into the unordered_map without extra copying

     // ✅ Save to file
    ofstream fout("customers.txt", ios::app); // open in append mode
    fout << u << " " << p << "\n";
    fout.close();

    cout << "Registered successfully!" << endl;
    loginCustomer(u); // automatically open login page after successful registration
  }

  void loginCustomer(const string &u) // function to login an existing customer
  {
    auto it = customers.find(u); //.find(u) returns an iterator pointing to the key–value pair (like a pointer to {username to Customer}).
    if (it == customers.end())   // If iterator points to .end(), username was not found.
    {
      cout << "No such user. Please register first." << endl;
      return;
    }
    cout << "Enter password: ";
    string p = inputHiddenPassword(); // password is saved in string p
    if (it->second.login(u, p))
    {                              // second.login(u,p) accesses the customer object and calls its login method to verify credentials
      currentUser = &(it->second); // store the address of this Customer object into currentUser pointer.
      cout << "Welcome, " << u << "!" << endl;
    }
    else
    {
      cout << "Invalid login." << endl;
    }
  }

  void logoutCustomer() // function to log out currently logged in customer
  {
    if (currentUser) //*currentUser is a pointer
    {
      cout << "Logged out " << currentUser->getUsername() << "." << endl; // getUsername() returns the username of the currently logged in customer
      currentUser = nullptr;                                              // pointer currentUser is null which means no one is logged in
    }
  }
  void showKitchens() const {
        cout << "\nAvailable Kitchen outlets:\n";
        for (size_t i = 0; i < kitchens.size(); i++) {
            cout << i + 1 << ". " << kitchens[i].name << endl;
        }
    }

 

  void placeOrder() // function to place an order
  {
    if (!currentUser) // to check if a user is logged in
    {
      cout << "Please login to place an order!" << endl;
      return;
    }
    int kChoice;    // variable to store user's kitchen choice
    cout << "Choose outlet number: ";
    cin >> kChoice;
    if (kChoice < 1 || kChoice > kitchens.size()) // to check if the choice exists
    {
      cout << "Invalid choice." << endl;
      return;
    }
    const Kitchens &k = kitchens[kChoice - 1]; // reference to the selected kitchen outlet
    vector<string> items;                      // to store orderd items
    double total = 0;                          // to store total price of order

    while (true) // while loop to allow user to add multiple times
    {
      k.displayMenu();                                       // display menu of selected kitchen outlet
      cout << k.menu.size() + 1 << ". Finish Order" << endl; // option to finish order

      cout << "Select item number: ";
      int choice;
      cin >> choice;
      if (choice == k.menu.size() + 1) // if user chooses to finish order
      {
        break;
      }

      if (choice >= 1 && choice <= k.menu.size()) // to give valid range of item choice
      {
        items.push_back(k.menu[choice - 1]);                     // add selected item to order list
        total += k.price[choice - 1];                            // add price of the selected items to total
        cout << k.menu[choice - 1] << " added to your order.\n"; // confirmation message
      }
      else
      {
        cout << "Invalid item choice." << endl;
      }
    }
    if (items.empty()) // checks if no items were selected
    {
      cout << "No items selected." << endl;
      return;
    }
      // Display selected items before showing the total bill
      cout << "\nFood items added to your list:" << endl;
      for (size_t i = 0; i < items.size(); ++i) {
          cout << "  " << i + 1 << ". " << items[i] << " - Rs. " << k.price[k.menu.size() > i ? i : 0] << endl;
      }
    cout << "\nYour total bill is: Rs. " << total << endl;
    cout << "\nChoose Payment Mode:" << endl
         << "1. Online (UPI)" << endl
         << "2. Cash on Delivery" << endl
         << "Enter choice: ";

    int payChoice;                               // variable to store user's payment choice
    cin >> payChoice;                            // input payment choice
    string paymentMode = "", transactionId = ""; // to initialize paymentMode and transactionId as empty strings
    string upi;                                  // variable to store user's UPI ID if online payment is chosen
    if (payChoice == 1)                          // if user uses online payment
    {
      cout << "Enter your UPI ID: ";
      cin >> upi;
      transactionCounter++;                                  // incrementing transaction counter to ensure unique transaction ID for each transaction
      transactionId = "TXN" + to_string(transactionCounter); // generating transaction Id by concatenating(joining) "TXN" with the current value of transactionCounter converted to string using to_string function
      paymentMode = "Online (UPI: " + upi + ")";             // setting paymentMode to indicate online payment along with the provided UPI ID
    }
    else // if user chooses cash on delivery
    {
      paymentMode = "Cash on Delivery"; // setting paymentMode to indicate COD
    }
    Orders newOrder(k.name, items, total, paymentMode, transactionId); // creating a new Orders object
    newOrder.display();                                                // displaying order details
    int action;                                                        // variable to store user's action choice
    while (true)                                                       // while loop to allow user to confirm, add more items, or cancel the order
    {
      cout << "\n1. Confirm Order" << endl
           << "2. Add More Items" << endl
           << "3. Cancel Order" << endl
           << "Choose option: ";
      cin >> action;

      if (action == 1) // if user chooses to confirm the order
      {
        cout << "Processing payment..." << endl;              // simulating payment processing
        std::this_thread::sleep_for(std::chrono::seconds(3)); // introducing a 3 seconds delay to mimic real-world payment processing time
        cout << "Payment successful ✅" << endl;              // confirmation message
        cout << "Order Accepted! 🎉" << endl;
        currentUser->addOrder(newOrder); // adding the confirmed order to the current user's order history
        newOrder.saveOrder(newOrder, currentUser->getUsername()); // save order to file
        break;
      }
      else if (action == 2) // if user chooses to add more items
      {
        cout << "Select extra item (0 to stop): ";
        int extra;
        cin >> extra;
        if (extra >= 1 && extra <= k.menu.size()) // to give valid range of extra item choice
        {
          newOrder.items.push_back(k.menu[extra - 1]); // add selected extra item to order list
          newOrder.total += k.price[extra - 1];        // add price of the selected extra item to total
          cout << "Item added." << endl;
          cout << "Updated total bill: Rs. " << newOrder.total << endl; // to display updated bill
          newOrder.display();                                           // display updated order list
        }
        else if (extra == 0) // if user enters ) to stop adding more items
        {
          cout << "No more items added." << endl;
        }
        else
        {
          cout << "Invalid choice." << endl;
        }
      }
      else if (action == 3) // if user chooses to cancel the order
      {
        cout << "Order cancelled." << endl;
        break; // exit the loop and return to main menu
      }
      else
      {
        cout << "Invalid option." << endl;
      }
    }
  }

  void showMyOrders() const // function to display all orders of the currently logged in customer
  {
    if (!currentUser) // to check if a user is logged in
    {
      cout << "Login first!" << endl;
      return;
    }
    currentUser->showOrders(); // calling showOrders method of the currently logged in customer to display their order history
  }
};
int main()
{
  CloudKitchen app;
  app.loadCustomers();
  app.addKitchen(Kitchens(" 🍢 Spicy Tandoor", {"Butter Chicken", "Paneer Butter Masala", "Dal Tadka", "Chicken Tikka", "Garlic Naan", "Roti", "Gulab Jamun"}, {250, 220, 150, 200, 50, 30, 90}));
  app.addKitchen(Kitchens(" 🍔 Burger Hub   ", {"Veg Burger", "Chicken Burger", "Cheese Burger", "French Fries", "Onion Rings", "Cold Coffee", "Coke"}, {150, 180, 170, 90, 100, 90, 80}));
  app.addKitchen(Kitchens(" 🍕 Pizza Planet ", {"Margherita Pizza", "Cheese Pizza", "Paneer Pizza", "Veggie Pizza", "Chicken Pizza", "Garlic Bread", "Jalpeno Poppers", "Chocolate Brownie"}, {250, 280, 300, 270, 320, 90, 100, 120}));
  app.addKitchen(Kitchens(" 🍜 Noodle Treat ", {"Veg Hakka Noodles", "Chicken Hakka Noodles", "Veg Fried Rice", "Chicken Fried Rice", "Veg Manchurian", "Chicken Manchurian", "Spring Rolls", "Hot & Sour Soup"}, {180, 200, 170, 200, 150, 180, 120, 100}));
  app.addKitchen(Kitchens(" 🌯 Wraps & Rolls", {"Chicken Roll", "Paneer Wrap", "Egg Roll", "Veg Roll", "Cheese Wrap", "Cold Drink", "Lemonade"}, {150, 140, 130, 120, 150, 50, 30}));
  app.addKitchen(Kitchens(" 🍩 Sweet Treats ", {"Chocolate Brownie", "Gulab Jamun", "Ice Cream Sundae", "Fruit Salad", "Milkshake", "Coffee", "Donuts"}, {100, 80, 120, 90, 100, 70, 80}));
  app.addKitchen(Kitchens(" 🍽️ Grill & Thrill", {"BBQ Chicken Wings (6 pcs)", "Grilled Paneer Skewers (4 pcs)", "Seekh Kebab (2pcs)", "Grilled Veg Platter", "Corn on the Cob (2pcs)", "Spicy Potato Wedges", "Mint Yogurt Dip", "Spiced Lemonade", "Iced Peach Tea"}, {249, 219, 279, 229, 129, 109, 49, 89, 99}));
  app.addKitchen(Kitchens(" 🌾 Rice Dice     ", {"Veg Biryani", "Chicken Biryani", "Egg Biryani", "Paneer Pulao", "Veg Fried Rice", "Raita", "Papad", "Sweet Lassi"}, {180, 220, 190, 200, 170, 200, 50, 30, 70}));
  app.addKitchen(Kitchens(" 🥗 Continental Cravings ", {"Grilled Chicken with Mashed Potatoes", "Spaghetti in Tomato Sauce", "Mac & Cheese", "Veg Lasagna", "Caesar Salad", "Cream of Mushroom Soup", "Roast Veggies with Herbs", "Club Sandwich", "Chocolate Mousse"}, {320, 250, 220, 260, 180, 150, 170, 200, 120}));
  app.addKitchen(Kitchens("🍴 Dosa Darbaar  ", {"Plain Dosa", "Masala Dosa", "Onion Dosa", "Rava Dosa", "Set Dosa", "Mysore Masala Dosa", "Idli (2 pcs)", "Vada (2 pcs)", "Filter Coffee"}, {80, 120, 100, 110, 130, 150, 60, 70, 50}));

  while (true)
  {
    cout << "\n----------🔄🍱🛵----- Flavor Loop Cloud Kitchen -----🛵🍱🔄----------\n";
    cout << "Dive into the FlavorLoop — your go-to cloud kitchen for endless delicious loops of joy." << endl;
    cout << "1. Register" << endl
         << "2. Login" << endl
         << "3. Show Kitchen Outlets" << endl
         << "4. Place Order" << endl
         << "5. Show My Orders" << endl
         << "6. Logout" << endl
         << "7. Exit" << endl;
    cout << "Enter choice: ";
    int ch;
    cin >> ch;
    string user;
    switch (ch)
    {
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
     app.showKitchens();
                 break;
  
    case 4:
    app.showKitchens();
      app.placeOrder();
      break;
    case 5:
      app.showMyOrders();
      break;
    case 6:
      app.logoutCustomer();
      break;
    case 7:
      cout << "Exiting... Goodbye!" << endl;
      return 0;
    default:
      cout << "Invalid choice." << endl;
    }
  }
}