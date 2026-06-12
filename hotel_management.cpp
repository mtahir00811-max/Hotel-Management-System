#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct FoodItem {
    string name;
    double price;
    int stock;
};

struct Room {
    int roomNumber;
    string type;       // Single, Double, Suite
    double pricePerNight;
    bool isBooked;
    string guestName;
    int nights;
    vector<int> orderedFoodIndexes; // indexes of food items ordered
};

// ---- GLOBAL DATA ----

vector<Room> rooms = {
    {101, "Single", 2000, false, "", 0, {}},
    {102, "Single", 2000, false, "", 0, {}},
    {103, "Double", 3500, false, "", 0, {}},
    {104, "Double", 3500, false, "", 0, {}},
    {105, "Suite",  7000, false, "", 0, {}},
    {106, "Suite",  7000, false, "", 0, {}}
};

vector<FoodItem> foodMenu = {
    {"Biryani",       350, 20},
    {"Burger",        200, 30},
    {"Pizza",         500, 15},
    {"Karahi",        450, 20},
    {"Chai",           50, 50},
    {"Cold Drink",     80, 40},
    {"Grilled Chicken",400, 18},
    {"Nihari",        300, 12}
};

double totalDayCollection = 0;

// ---- HELPER FUNCTIONS ----

void printLine() {
    cout << "  ============================================" << endl;
}

void printHeader(string title) {
    printLine();
    cout << "       " << title << endl;
    printLine();
}

// ---- DISPLAY FUNCTIONS ----

void showAllRooms() {
    printHeader("ROOM STATUS");
    cout << setw(8) << "Room No"
         << setw(10) << "Type"
         << setw(14) << "Price/Night"
         << setw(12) << "Status"
         << setw(15) << "Guest Name" << endl;
    printLine();
    for (auto& r : rooms) {
        cout << setw(8) << r.roomNumber
             << setw(10) << r.type
             << setw(12) << "Rs." << r.pricePerNight
             << setw(12) << (r.isBooked ? "Booked" : "Available")
             << setw(15) << (r.isBooked ? r.guestName : "-") << endl;
    }
    cout << endl;
}

void showFoodMenu() {
    printHeader("FOOD MENU");
    cout << setw(5) << "No."
         << setw(20) << "Item"
         << setw(12) << "Price"
         << setw(10) << "Stock" << endl;
    printLine();
    for (int i = 0; i < (int)foodMenu.size(); i++) {
        cout << setw(5) << i + 1
             << setw(20) << foodMenu[i].name
             << setw(10) << "Rs." << foodMenu[i].price
             << setw(10) << foodMenu[i].stock << endl;
    }
    cout << endl;
}

// ---- ROOM BOOKING ----

void bookRoom() {
    printHeader("BOOK A ROOM");
    showAllRooms();

    int roomNo;
    cout << "  Enter Room Number to Book: ";
    cin >> roomNo;

    bool found = false;
    for (auto& r : rooms) {
        if (r.roomNumber == roomNo) {
            found = true;
            if (r.isBooked) {
                cout << "  Sorry! Room " << roomNo << " is already booked." << endl;
            } else {
                cin.ignore();
                cout << "  Enter Guest Name: ";
                getline(cin, r.guestName);
                cout << "  Enter Number of Nights: ";
                cin >> r.nights;
                r.isBooked = true;
                cout << "\n  Room " << roomNo << " booked successfully for " << r.guestName << "!" << endl;
                cout << "  Total Room Charge: Rs." << r.pricePerNight * r.nights << endl;
            }
            break;
        }
    }
    if (!found) cout << "  Room not found!" << endl;
}

// ---- ORDER FOOD ----

void orderFood() {
    printHeader("ORDER FOOD");
    showAllRooms();

    int roomNo;
    cout << "  Enter Room Number to Order Food For: ";
    cin >> roomNo;

    Room* selectedRoom = nullptr;
    for (auto& r : rooms) {
        if (r.roomNumber == roomNo) {
            selectedRoom = &r;
            break;
        }
    }

    if (!selectedRoom) { cout << "  Room not found!" << endl; return; }
    if (!selectedRoom->isBooked) { cout << "  Room is not booked!" << endl; return; }

    showFoodMenu();

    char more = 'y';
    while (tolower(more) == 'y') {
        int choice;
        cout << "  Enter Food Item Number: ";
        cin >> choice;
        choice--;

        if (choice < 0 || choice >= (int)foodMenu.size()) {
            cout << "  Invalid choice!" << endl;
        } else if (foodMenu[choice].stock == 0) {
            cout << "  Sorry, " << foodMenu[choice].name << " is out of stock!" << endl;
        } else {
            selectedRoom->orderedFoodIndexes.push_back(choice);
            foodMenu[choice].stock--;
            cout << "  " << foodMenu[choice].name << " added to order!" << endl;
        }

        cout << "  Order more? (y/n): ";
        cin >> more;
    }
    cout << "  Food order placed for Room " << roomNo << "!" << endl;
}

// ---- CHECKOUT ----

void checkoutRoom() {
    printHeader("CHECKOUT");
    showAllRooms();

    int roomNo;
    cout << "  Enter Room Number to Checkout: ";
    cin >> roomNo;

    bool found = false;
    for (auto& r : rooms) {
        if (r.roomNumber == roomNo) {
            found = true;
            if (!r.isBooked) {
                cout << "  Room " << roomNo << " is not booked!" << endl;
                break;
            }

            // Calculate bill
            double roomCharge = r.pricePerNight * r.nights;
            double foodCharge = 0;

            printLine();
            cout << "  BILL FOR: " << r.guestName << " | Room: " << roomNo << endl;
            printLine();
            cout << "  Room Type     : " << r.type << endl;
            cout << "  Nights Stayed : " << r.nights << endl;
            cout << "  Room Charge   : Rs." << roomCharge << endl;

            if (!r.orderedFoodIndexes.empty()) {
                cout << "\n  Food Orders:" << endl;
                for (int idx : r.orderedFoodIndexes) {
                    cout << "    - " << foodMenu[idx].name
                         << "  Rs." << foodMenu[idx].price << endl;
                    foodCharge += foodMenu[idx].price;
                }
            }

            double total = roomCharge + foodCharge;
            printLine();
            cout << "  Food Charge   : Rs." << foodCharge << endl;
            cout << "  TOTAL BILL    : Rs." << total << endl;
            printLine();

            totalDayCollection += total;

            // Reset room
            r.isBooked = false;
            r.guestName = "";
            r.nights = 0;
            r.orderedFoodIndexes.clear();

            cout << "\n  Checkout successful! Room " << roomNo << " is now available." << endl;
            break;
        }
    }
    if (!found) cout << "  Room not found!" << endl;
}

// ---- FOOD STOCK ----

void showFoodStock() {
    printHeader("FOOD STOCK STATUS");
    cout << setw(5) << "No."
         << setw(20) << "Item"
         << setw(12) << "Price"
         << setw(10) << "Stock" << endl;
    printLine();
    for (int i = 0; i < (int)foodMenu.size(); i++) {
        cout << setw(5) << i + 1
             << setw(20) << foodMenu[i].name
             << setw(10) << "Rs." << foodMenu[i].price
             << setw(10) << foodMenu[i].stock;
        if (foodMenu[i].stock < 5)
            cout << "  *** LOW STOCK ***";
        cout << endl;
    }
    cout << endl;
}

// ---- DAILY COLLECTION ----

void showDailyCollection() {
    printHeader("DAILY COLLECTION REPORT");
    cout << "  Total Collection Today : Rs." << totalDayCollection << endl;

    int bookedCount = 0;
    for (auto& r : rooms)
        if (r.isBooked) bookedCount++;

    cout << "  Rooms Currently Booked : " << bookedCount << "/" << rooms.size() << endl;
    cout << "  Rooms Available        : " << rooms.size() - bookedCount << "/" << rooms.size() << endl;
    printLine();
}

// ---- MAIN MENU ----

int main() {
    int choice;

    cout << "\n  ============================================" << endl;
    cout << "       HOTEL MANAGEMENT SYSTEM" << endl;
    cout << "       Developed for Crixsoft Solution" << endl;
    cout << "       C++ Internship - Task 2" << endl;
    cout << "  ============================================" << endl;

    do {
        cout << "\n  -------- MAIN MENU --------" << endl;
        cout << "  1. View All Rooms" << endl;
        cout << "  2. Book a Room" << endl;
        cout << "  3. Order Food" << endl;
        cout << "  4. Checkout & Generate Bill" << endl;
        cout << "  5. View Food Menu & Stock" << endl;
        cout << "  6. Daily Collection Report" << endl;
        cout << "  0. Exit" << endl;
        cout << "  Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: showAllRooms();         break;
            case 2: bookRoom();             break;
            case 3: orderFood();            break;
            case 4: checkoutRoom();         break;
            case 5: showFoodStock();        break;
            case 6: showDailyCollection();  break;
            case 0: cout << "\n  Thank you! Goodbye!" << endl; break;
            default: cout << "  Invalid choice! Try again." << endl;
        }

    } while (choice != 0);

    return 0;
}
