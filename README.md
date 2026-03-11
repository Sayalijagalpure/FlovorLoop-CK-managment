# 🍱 FlavorLoop – Cloud Kitchen Management System

FlavorLoop is a C++ console-based Cloud Kitchen Management System that simulates a real-world food ordering platform.
The application allows users to register, login, browse multiple cloud kitchen outlets, place food orders, and manage order history.

The system demonstrates the use of Object-Oriented Programming (OOP), file handling, data structures, and basic system design concepts.

## 🚀 Features

👤 Customer Registration & Login

🔐 Hidden password input for secure login

🍽️ Multiple cloud kitchen outlets

📋 Dynamic menu display

🛒 Place and manage food orders

💳 Payment options

     ● Online (UPI)

     ● Cash on Delivery

🧾 Automatic bill generation

📦 Order history tracking

💾 Persistent storage using files

🔢 Unique transaction ID generation

⏳ Simulated payment processing delay

## 🛠️ Technologies Used

Language: C++

Concepts Used:

 ● Object-Oriented Programming (OOP)

 ● Inheritance

 ● Polymorphism

 ● Encapsulation

 ● STL (Vector, Map, Unordered Map)

Libraries Used:

    <vector>

    <map>

    <unordered_map>

    <fstream>

    <thread>

    <chrono>

    <conio.h>

    <windows.h>
    
## 🧠 OOP Concepts Implemented
1️⃣ Inheritance

   ● Entity → Base class

   ● Kitchens and Orders inherit from Entity

   ● User → Base class

   ● Customer inherits from User

2️⃣ Polymorphism

   ● Virtual function display() is overridden in derived classes.

3️⃣ Encapsulation

   ● User credentials and order data are stored securely within classes.

## 💳 Payment System

Two payment modes are available:

Online Payment (UPI)

   ● User enters UPI ID

   ● Unique Transaction ID (TXN1001, TXN1002, etc.) is generated.

Cash on Delivery

   ● Payment done at delivery time.

## 💾 File Storage

The system stores data using text files:

customers.txt	-> Stores registered usernames and passwords
orders.txt	   -> Stores order history
counter.txt	   -> Stores last transaction ID counter

This ensures data persistence between program runs.

## 📂 Project Structure
FlavorLoop-CK-managment<br>
│<br>
├── main.cpp<br>
├── customers.txt<br>
├── orders.txt<br>
├── counter.txt<br>
└── README.md<br>

## ⚙️ How to Run the Project
1️⃣ Clone the repository
git clone https://github.com/Sayalijagalpure/FlovorLoop-CK-managment.git<br>
2️⃣ Open the folder
cd FlovorLoop-CK-managment<br>
3️⃣ Compile the program
g++ main.cpp -o flavorloop<br>
4️⃣ Run the program
./flavorloop

(Windows users may run flavorloop.exe)

## 📸 Sample Workflow

1. Register a new customer account

2. Login with username and password

3. View available kitchen outlets

4. Select an outlet and choose food items

5. Confirm order and select payment mode

6. Order details are saved in the system

## 🎯 Learning Objectives

This project was built to practice:

   ● Object-Oriented Programming in C++

   ● STL containers

   ● File handling

   ● Console-based system design

   ● Simulation of a real-world ordering system

## 🔮 Future Improvements

   ● GUI version using Qt / C++ GUI
  
   ● Database integration (MySQL)

   ● Admin dashboard

   ● Online delivery tracking

   ● Mobile / Web interface
