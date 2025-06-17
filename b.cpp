
// #include <iostream>
// #include <string>
// #include <limits>
// #include <vector>
// #include <algorithm>
// #include <cctype>

// using namespace std;

// // Constants
// const int MAX_TRANS = 100; // Maximum number of transactions

// // Allowed categories
// vector<string> allowedCategories = {"food", "transport", "bills", "entertainment", "shopping", "health", "other"};

// // Transaction class encapsulating transaction details
// class Transaction {
// private:
//     string date;
//     string category;
//     string description;
//     float amount;

// public:
//     // Default constructor
//     Transaction() : date(""), category(""), description(""), amount(0.0f) {}

//     // Parameterized constructor
//     Transaction(string d, string c, string desc, float a)
//         : date(d), category(c), description(desc), amount(a) {}

//     // Getter methods
//     string getDate() const { return date; }
//     string getCategory() const { return category; }
//     string getDescription() const { return description; }
//     float getAmount() const { return amount; }
// };

// // Function declarations
// void displayMenu();
// void addTransaction(Transaction transactions[], int& count);
// void deleteTransaction(Transaction transactions[], int& count);
// void searchTransaction(const Transaction transactions[], int count);
// void displayTransactions(Transaction transactions[], int count);
// void sortByDate(Transaction transactions[], int count);
// void sortByAmount(Transaction transactions[], int count);
// bool isValidDate(const string& date);
// string getValidCategory();
// void searchByDate(const Transaction transactions[], int count, string date);
// void searchByCategory(const Transaction transactions[], int count, string category);

// // Main function
// int main() {
//     Transaction transactions[MAX_TRANS];
//     int transactionCount = 0;
//     int choice;

//     // Menu loop
//     while (true) {
//         displayMenu();
//         cin >> choice;

//         // Input validation
//         if (cin.fail()) {
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             cout << "Invalid input. Please enter a number.\n";
//             continue;
//         }

//         // Handle menu choices
//         switch (choice) {
//             case 1:
//                 addTransaction(transactions, transactionCount);
//                 break;
//             case 2:
//                 deleteTransaction(transactions, transactionCount);
//                 break;
//             case 3:
//                 searchTransaction(transactions, transactionCount);
//                 break;
//             case 4:
//                 displayTransactions(transactions, transactionCount);
//                 break;
//             case 5:
//                 cout << "Exiting program.\n";
//                 return 0;
//             default:
//                 cout << "Please choose a valid option (1–5).\n";
//         }
//     }
// }

// // Display menu to user
// void displayMenu() {
//     cout << "\n=== Personal Finance Tracker ===\n";
//     cout << "1. Add Transaction\n";
//     cout << "2. Delete Transaction\n";
//     cout << "3. Search Transaction\n";
//     cout << "4. Display Transactions\n";
//     cout << "5. Exit\n";
//     cout << "Enter choice: ";
// }

// // Validate if entered date is in proper format and logical range
// bool isValidDate(const string& date) {
//     if (date.length() != 10 || date[2] != '/' || date[5] != '/')
//         return false;
//     try {
//         int day = stoi(date.substr(0, 2));
//         int month = stoi(date.substr(3, 2));
//         int year = stoi(date.substr(6, 4));

//         if (year < 1900 || year > 2100) return false;
//         if (month < 1 || month > 12) return false;
//         if (day < 1 || day > 31) return false;
//         if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
//         if (month == 2) {
//             bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
//             if ((isLeap && day > 29) || (!isLeap && day > 28)) return false;
//         }
//     } catch (...) {
//         return false;
//     }
//     return true;
// }

// // Prompt and validate category input from user
// string getValidCategory() {
//     string category;
//     while (true) {
//         cout << "Enter category (Food, Transport, Bills, Entertainment, Shopping, Health, Other): ";
//         getline(cin, category);
//         transform(category.begin(), category.end(), category.begin(), ::tolower);
//         if (find(allowedCategories.begin(), allowedCategories.end(), category) != allowedCategories.end()) {
//             return category;
//         }
//         cout << "Invalid category. Try again.\n";
//     }
// }

// // Add a new transaction
// void addTransaction(Transaction transactions[], int& count) {
//     if (count >= MAX_TRANS) {
//         cout << "Transaction limit reached.\n";
//         return;
//     }

//     string date, category, description;
//     float amount;

//     cin.ignore(); // Clear input buffer
//     cout << "Enter date (DD/MM/YYYY): ";
//     getline(cin, date);
//     while (!isValidDate(date)) {
//         cout << "Invalid format or value. Try again (DD/MM/YYYY): ";
//         getline(cin, date);
//     }

//     category = getValidCategory();

//     cout << "Enter description: ";
//     getline(cin, description);

//     cout << "Enter amount: ";
//     while (true) {
//         try {
//             cin >> amount;
//             if (cin.fail()) throw runtime_error("Invalid input.");
//             break;
//         } catch (...) {
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             cout << "Please enter a valid amount: ";
//         }
//     }

//     transactions[count++] = Transaction(date, category, description, amount);
//     cout << "Transaction added.\n";
// }

// // Delete a transaction by index
// void deleteTransaction(Transaction transactions[], int& count) {
//     if (count == 0) {
//         cout << "No transactions to delete.\n";
//         return;
//     }

//     // Manual transaction listing
//     cout << "\n--- Transactions List ---\n";
//     for (int i = 0; i < count; i++) {
//         cout << "Index: " << i << " | "
//              << transactions[i].getDate() << " | "
//              << transactions[i].getCategory() << " | "
//              << transactions[i].getDescription() << " | $"
//              << transactions[i].getAmount() << endl;
//     }

//     int index;
//     cout << "Enter index to delete (0 to " << count - 1 << "): ";
//     cin >> index;

//     if (cin.fail() || index < 0 || index >= count) {
//         cin.clear();
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         cout << "Invalid index.\n";
//         return;
//     }

//     // Shift elements to overwrite the deleted one
//     for (int i = index; i < count - 1; i++) {
//         transactions[i] = transactions[i + 1];
//     }
//     count--;
//     cout << "Transaction deleted.\n";
// }

// // Display all transactions with sorting options
// void displayTransactions(Transaction transactions[], int count) {
//     if (count == 0) {
//         cout << "No transactions to display.\n";
//         return;
//     }

//     int sortChoice;
//     cout << "Sort by:\n1. Date\n2. Amount\n3. None\nEnter choice: ";
//     cin >> sortChoice;

//     if (sortChoice == 1) sortByDate(transactions, count);
//     else if (sortChoice == 2) sortByAmount(transactions, count);

//     // Output transactions
//     for (int i = 0; i < count; i++) {
//         cout << "\nIndex: " << i
//              << "\nDate: " << transactions[i].getDate()
//              << "\nCategory: " << transactions[i].getCategory()
//              << "\nDescription: " << transactions[i].getDescription()
//              << "\nAmount: $" << transactions[i].getAmount()
//              << "\n--------------------------";
//     }
// }

// // Bubble sort by date
// void sortByDate(Transaction transactions[], int count) {
//     for (int i = 0; i < count - 1; i++) {
//         for (int j = 0; j < count - i - 1; j++) {
//             string d1 = transactions[j].getDate();
//             string d2 = transactions[j + 1].getDate();
//             int y1 = stoi(d1.substr(6, 4)), m1 = stoi(d1.substr(3, 2)), day1 = stoi(d1.substr(0, 2));
//             int y2 = stoi(d2.substr(6, 4)), m2 = stoi(d2.substr(3, 2)), day2 = stoi(d2.substr(0, 2));
//             if (y1 > y2 || (y1 == y2 && m1 > m2) || (y1 == y2 && m1 == m2 && day1 > day2)) {
//                 swap(transactions[j], transactions[j + 1]);
//             }
//         }
//     }
// }

// // Bubble sort by amount
// void sortByAmount(Transaction transactions[], int count) {
//     for (int i = 0; i < count - 1; i++) {
//         for (int j = 0; j < count - i - 1; j++) {
//             if (transactions[j].getAmount() > transactions[j + 1].getAmount()) {
//                 swap(transactions[j], transactions[j + 1]);
//             }
//         }
//     }
// }

// // Allow user to search transactions by date or category
// void searchTransaction(const Transaction transactions[], int count) {
//     if (count == 0) {
//         cout << "No transactions to search.\n";
//         return;
//     }

//     int choice;
//     cout << "Search by:\n1. Date\n2. Category\nEnter choice: ";
//     cin >> choice;

//     cin.ignore(); // Flush buffer
//     if (choice == 1) {
//         string date;
//         cout << "Enter date (DD/MM/YYYY): ";
//         getline(cin, date);
//         searchByDate(transactions, count, date);
//     } else if (choice == 2) {
//         string category = getValidCategory();
//         searchByCategory(transactions, count, category);
//     } else {
//         cout << "Invalid search option.\n";
//     }
// }

// // Search transactions by date
// void searchByDate(const Transaction transactions[], int count, string date) {
//     bool found = false;
//     for (int i = 0; i < count; i++) {
//         if (transactions[i].getDate() == date) {
//             cout << "Found at index " << i << ": $" << transactions[i].getAmount()
//                  << " [" << transactions[i].getDescription() << "]\n";
//             found = true;
//         }
//     }
//     if (!found) cout << "No transactions on this date.\n";
// }

// // Search transactions by category
// void searchByCategory(const Transaction transactions[], int count, string category) {
//     bool found = false;
//     for (int i = 0; i < count; i++) {
//         if (transactions[i].getCategory() == category) {
//             cout << "Found at index " << i << ": " << transactions[i].getDate()
//                  << " - $" << transactions[i].getAmount() << " - " << transactions[i].getDescription() << endl;
//             found = true;
//         }
//     }
//     if (!found) cout << "No transactions in this category.\n";
// }



#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <fstream>  // <-- For file I/O

using namespace std;

// Allowed categories
vector<string> allowedCategories = {"food", "transport", "bills", "entertainment", "shopping", "health", "other"};

class Transaction {
private:
    string date, category, description;
    float amount;

public:
    Transaction() : date(""), category(""), description(""), amount(0.0f) {}
    Transaction(string d, string c, string desc, float a)
        : date(d), category(c), description(desc), amount(a) {}

    string getDate() const { return date; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    float getAmount() const { return amount; }
    // For file save/load, we’ll need "friend" or accessors (above are enough)
};

// ========== FILE HANDLING CODE ==========

// Save transactions to file (CSV format)
void saveTransactions(const vector<Transaction>& transactions, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    for (const auto& t : transactions) {
        file << t.getDate() << "," << t.getCategory() << "," << t.getDescription() << "," << t.getAmount() << endl;
    }
    file.close();
}

// Load transactions from file
void loadTransactions(vector<Transaction>& transactions, const string& filename) {
    ifstream file(filename);
    if (!file) {
        // No file? Start fresh
        cout << "(No saved transactions found, starting new...)" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) continue;
        string date = line.substr(0, pos1);
        string category = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string description = line.substr(pos2 + 1, pos3 - pos2 - 1);
        float amount = stof(line.substr(pos3 + 1));
        transactions.push_back(Transaction(date, category, description, amount));
    }
    file.close();
}

// ========== EXISTING FUNCTIONS ==========
// (Use your existing code for isValidDate, getValidCategory, addTransaction, etc.)
// Modify addTransaction and deleteTransaction to work with vector<Transaction>& instead of array.

void displayMenu() {
    cout << "\n=== Personal Finance Tracker ===\n";
    cout << "1. Add Transaction\n";
    cout << "2. Delete Transaction\n";
    cout << "3. Display Transactions\n";
    cout << "4. Exit\n";
    cout << "Enter choice: ";
}

bool isValidDate(const string& date) {
    if (date.length() != 10 || date[2] != '/' || date[5] != '/')
        return false;
    try {
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
        if (month == 2) {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if ((isLeap && day > 29) || (!isLeap && day > 28)) return false;
        }
    } catch (...) {
        return false;
    }
    return true;
}

string getValidCategory() {
    string category;
    while (true) {
        cout << "Enter category (Food, Transport, Bills, Entertainment, Shopping, Health, Other): ";
        getline(cin, category);
        transform(category.begin(), category.end(), category.begin(), ::tolower);
        if (find(allowedCategories.begin(), allowedCategories.end(), category) != allowedCategories.end()) {
            return category;
        }
        cout << "Invalid category. Try again.\n";
    }
}

void addTransaction(vector<Transaction>& transactions) {
    string date, category, description;
    float amount;

    cin.ignore();
    cout << "Enter date (DD/MM/YYYY): ";
    getline(cin, date);
    while (!isValidDate(date)) {
        cout << "Invalid format or value. Try again (DD/MM/YYYY): ";
        getline(cin, date);
    }

    category = getValidCategory();

    cout << "Enter description: ";
    getline(cin, description);

    cout << "Enter amount: ";
    while (!(cin >> amount)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid amount: ";
    }

    transactions.push_back(Transaction(date, category, description, amount));
    cout << "Transaction added.\n";
    cin.ignore();
}

void displayTransactions(const vector<Transaction>& transactions) {
    if (transactions.empty()) {
        cout << "No transactions to display.\n";
        return;
    }
    for (size_t i = 0; i < transactions.size(); ++i) {
        cout << "\nIndex: " << i
            << "\nDate: " << transactions[i].getDate()
            << "\nCategory: " << transactions[i].getCategory()
            << "\nDescription: " << transactions[i].getDescription()
            << "\nAmount: $" << transactions[i].getAmount()
            << "\n--------------------------";
    }
}

void deleteTransaction(vector<Transaction>& transactions) {
    if (transactions.empty()) {
        cout << "No transactions to delete.\n";
        return;
    }

    for (size_t i = 0; i < transactions.size(); i++) {
        cout << "Index: " << i << " | "
            << transactions[i].getDate() << " | "
            << transactions[i].getCategory() << " | "
            << transactions[i].getDescription() << " | $"
            << transactions[i].getAmount() << endl;
    }

    int index;
    cout << "Enter index to delete (0 to " << transactions.size() - 1 << "): ";
    cin >> index;
    if (cin.fail() || index < 0 || (size_t)index >= transactions.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid index.\n";
        return;
    }
    transactions.erase(transactions.begin() + index);
    cout << "Transaction deleted.\n";
}

// ========== MAIN FUNCTION ==========

int main() {
    vector<Transaction> transactions;
    string filename = "transactions.csv";
    loadTransactions(transactions, filename);
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        switch (choice) {
            case 1:
                addTransaction(transactions);
                saveTransactions(transactions, filename);
                break;
            case 2:
                deleteTransaction(transactions);
                saveTransactions(transactions, filename);
                break;
            case 3:
                displayTransactions(transactions);
                break;
            case 4:
                saveTransactions(transactions, filename); // Just in case
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Please choose a valid option (1–4).\n";
        }
    }
}
