#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <list>

using namespace std;

// ----- SHA-256 Implementation (see previous messages for details) -----
typedef unsigned char uint8;
typedef unsigned int uint32;

struct SHA256_CTX
{
    uint32 state[8];
    uint32 count[2];
    uint8 buffer[64];
};

void sha256_transform(SHA256_CTX *ctx, const uint8 data[]);
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8 data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8 hash[]);

#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))
#define ROTRIGHT(a, b) ((a >> b) | (a << (32 - b)))
#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ (x >> 3))
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ (x >> 10))

static const uint32 k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void sha256_transform(SHA256_CTX *ctx, const uint8 data[])
{
    uint32 a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];
    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for (; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for (i = 0; i < 64; ++i)
    {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx)
{
    ctx->count[0] = ctx->count[1] = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const uint8 data[], size_t len)
{
    size_t i, j;
    j = (ctx->count[0] >> 3) & 63;
    if ((ctx->count[0] += len << 3) < (len << 3))
        ctx->count[1]++;
    ctx->count[1] += (len >> 29);
    if ((j + len) > 63)
    {
        memcpy(&ctx->buffer[j], data, (i = 64 - j));
        sha256_transform(ctx, ctx->buffer);
        for (; i + 63 < len; i += 64)
        {
            sha256_transform(ctx, &data[i]);
        }
        j = 0;
    }
    else
        i = 0;
    memcpy(&ctx->buffer[j], &data[i], len - i);
}

void sha256_final(SHA256_CTX *ctx, uint8 hash[])
{
    uint32 i, j;
    uint8 bits[8];
    static uint8 padding[64] = {0x80};
    for (i = 0; i < 8; i++)
        bits[i] = (uint8)((ctx->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);
    j = (ctx->count[0] >> 3) & 63;
    sha256_update(ctx, padding, (j < 56) ? (56 - j) : (120 - j));
    sha256_update(ctx, bits, 8);
    for (i = 0; i < 32; i++)
        hash[i] = (ctx->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255;
}

std::string sha256(const std::string &input)
{
    uint8 hash[32];
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, (const uint8 *)input.c_str(), input.length());
    sha256_final(&ctx, hash);
    std::ostringstream result;
    for (int i = 0; i < 32; i++)
        result << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return result.str();
}

// ----- Transaction & User Classes -----
vector<string> allowedCategories = {"food", "transport", "bills", "entertainment", "shopping", "health", "other"};
const int RECENT_COUNT = 5;

class Transaction
{
    string date, category, description;
    float amount;

public:
    Transaction() : date(""), category(""), description(""), amount(0.0f) {}
    Transaction(string d, string c, string desc, float a) : date(d), category(c), description(desc), amount(a) {}
    string getDate() const { return date; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    float getAmount() const { return amount; }
};

struct User
{
    string username, passwordHash, role; // role: "admin" or "user"
};

// ----- File Handling -----
void saveTransactions(const list<Transaction> &transactions, const string &filename)
{
    ofstream file(filename);
    if (!file)
    {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    for (const auto &t : transactions)
        file << t.getDate() << "," << t.getCategory() << "," << t.getDescription() << "," << t.getAmount() << endl;
    file.close();
}
void loadTransactions(list<Transaction> &transactions, const string &filename)
{
    ifstream file(filename);
    if (!file)
    {
        cout << "(No saved transactions found, starting new...)\n";
        return;
    }
    string line;
    while (getline(file, line))
    {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos)
            continue;
        string date = line.substr(0, pos1);
        string category = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string description = line.substr(pos2 + 1, pos3 - pos2 - 1);
        float amount = stof(line.substr(pos3 + 1));
        transactions.push_back(Transaction(date, category, description, amount));
    }
    file.close();
}

void saveUsers(const vector<User> &users, const string &filename)
{
    ofstream file(filename);
    if (!file)
    {
        cout << "Error saving users!" << endl;
        return;
    }
    for (const auto &u : users)
        file << u.username << "," << u.passwordHash << "," << u.role << endl;
    file.close();
}
void loadUsers(vector<User> &users, const string &filename)
{
    ifstream file(filename);
    if (!file)
        return;
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string username, passwordHash, role;
        getline(ss, username, ',');
        getline(ss, passwordHash, ',');
        getline(ss, role, ',');
        if (!username.empty() && !passwordHash.empty() && !role.empty())
            users.push_back({username, passwordHash, role});
    }
    file.close();
}

// ----- Validation & UI -----
bool isValidDate(const string &date)
{
    if (date.length() != 10 || date[2] != '/' || date[5] != '/')
        return false;
    try
    {
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));
        if (year < 1900 || year > 2100)
            return false;
        if (month < 1 || month > 12)
            return false;
        if (day < 1 || day > 31)
            return false;
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
            return false;
        if (month == 2)
        {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if ((isLeap && day > 29) || (!isLeap && day > 28))
                return false;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}
string getValidCategory()
{
    string category;
    while (true)
    {
        cout << "Enter category (Food, Transport, Bills, Entertainment, Shopping, Health, Other): ";
        getline(cin, category);
        transform(category.begin(), category.end(), category.begin(), ::tolower);
        if (find(allowedCategories.begin(), allowedCategories.end(), category) != allowedCategories.end())
            return category;
        cout << "Invalid category. Try again.\n";
    }
}

void displayMenu(bool isAdmin)
{
    cout << "\n=== Personal Finance Tracker ===\n";
    cout << "1. Add Transaction\n";
    if (isAdmin)
        cout << "2. Delete Transaction (Admin Only)\n";
    cout << "3. Display Transactions\n";
    cout << "4. Logout\n";
    cout << "5. Display Recent Transactions\n";
    cout << "Enter choice: ";
}

// ----- Transaction Management -----
void addTransaction(list<Transaction> &transactions, stack<Transaction> &recentTransactions)
{
    string date, category, description;
    float amount;
    cin.ignore();
    cout << "Enter date (DD/MM/YYYY): ";
    getline(cin, date);
    while (!isValidDate(date))
    {
        cout << "Invalid format or value. Try again (DD/MM/YYYY): ";
        getline(cin, date);
    }
    category = getValidCategory();
    cout << "Enter description: ";
    getline(cin, description);
    cout << "Enter amount: ";
    while (!(cin >> amount))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid amount: ";
    }
    Transaction t(date, category, description, amount);
    transactions.push_back(t);

    // Maintain stack for recent transactions
    recentTransactions.push(t);
    // If more than RECENT_COUNT, rebuild stack (simple, not optimal but works for small N)
    if (recentTransactions.size() > RECENT_COUNT)
    {
        stack<Transaction> temp;
        list<Transaction>::reverse_iterator rit = transactions.rbegin();
        for (int i = 0; i < RECENT_COUNT && rit != transactions.rend(); ++i, ++rit)
            temp.push(*rit);
        // Rebuild correct order (most recent on top)
        stack<Transaction> rebuild;
        while (!temp.empty())
        {
            rebuild.push(temp.top());
            temp.pop();
        }
        recentTransactions = rebuild;
    }

    cout << "Transaction added.\n";
    cin.ignore();
}

void displayTransactions(const list<Transaction> &transactions)
{
    if (transactions.empty())
    {
        cout << "No transactions to display.\n";
        return;
    }
    int i = 0;
    for (const auto &t : transactions)
    {
        cout << "\nIndex: " << i++
             << "\nDate: " << t.getDate()
             << "\nCategory: " << t.getCategory()
             << "\nDescription: " << t.getDescription()
             << "\nAmount: $" << t.getAmount()
             << "\n--------------------------";
    }
}
void deleteTransaction(list<Transaction> &transactions)
{
    if (transactions.empty())
    {
        cout << "No transactions to delete.\n";
        return;
    }
    int i = 0;
    for (const auto &t : transactions)
    {
        cout << "Index: " << i++
             << " | " << t.getDate()
             << " | " << t.getCategory()
             << " | " << t.getDescription()
             << " | $" << t.getAmount() << endl;
    }
    int index;
    cout << "Enter index to delete (0 to " << transactions.size() - 1 << "): ";
    cin >> index;
    if (cin.fail() || index < 0 || index >= (int)transactions.size())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid index.\n";
        return;
    }
    auto it = transactions.begin();
    advance(it, index);
    transactions.erase(it);
    cout << "Transaction deleted.\n";
}

void displayRecentTransactions(stack<Transaction> recentTransactions)
{
    if (recentTransactions.empty())
    {
        cout << "No recent transactions.\n";
        return;
    }
    cout << "Recent Transactions (most recent first):\n";
    int count = 0;
    while (!recentTransactions.empty() && count < RECENT_COUNT)
    {
        Transaction t = recentTransactions.top();
        cout << "\nDate: " << t.getDate()
             << "\nCategory: " << t.getCategory()
             << "\nDescription: " << t.getDescription()
             << "\nAmount: $" << t.getAmount()
             << "\n--------------------------";
        recentTransactions.pop();
        count++;
    }
}

// ----- User Authentication -----
User loginUser(const vector<User> &users)
{
    string uname, pass;
    cout << "Username: ";
    cin >> uname;
    cout << "Password: ";
    cin.ignore();
    getline(cin, pass);
    string hash = sha256(pass);

    for (const auto &u : users)
        if (u.username == uname && u.passwordHash == hash)
            return u;

    cout << "Invalid username or password.\n";
    return {"", "", ""};
}
void registerUser(vector<User> &users, const string &filename, bool isAdmin = false)
{
    string uname, pass, pass2;
    cout << "Register a new " << (isAdmin ? "ADMIN" : "user") << "\nUsername: ";
    cin >> uname;
    for (const auto &u : users)
        if (u.username == uname)
        {
            cout << "Username exists!\n";
            return;
        }

    cout << "Password: ";
    cin.ignore();
    getline(cin, pass);
    cout << "Confirm Password: ";
    getline(cin, pass2);
    if (pass != pass2)
    {
        cout << "Passwords do not match!\n";
        return;
    }

    string hash = sha256(pass);
    users.push_back({uname, hash, isAdmin ? "admin" : "user"});
    saveUsers(users, filename);
    cout << (isAdmin ? "Admin" : "User") << " registered successfully!\n";
}

// ----- Main -----
int main()
{
    list<Transaction> transactions;
    stack<Transaction> recentTransactions;
    vector<User> users;
    string transactionsFile = "transactions.csv";
    string usersFile = "users.csv";

    loadTransactions(transactions, transactionsFile);
    loadUsers(users, usersFile);

    // On load, fill the recent transactions stack with last RECENT_COUNT
    if (!transactions.empty())
    {
        stack<Transaction> temp;
        auto rit = transactions.rbegin();
        for (int i = 0; i < RECENT_COUNT && rit != transactions.rend(); ++i, ++rit)
            temp.push(*rit);
        // Proper stack order
        while (!temp.empty())
        {
            recentTransactions.push(temp.top());
            temp.pop();
        }
    }

    // Force creation of first admin if users file is empty
    if (users.empty())
    {
        cout << "No users found. Registering initial admin account.\n";
        registerUser(users, usersFile, true);
    }

    // Login/register menu loop
    User currentUser;
    while (true)
    {
        cout << "\n1. Login\n2. Register New User\n3. Exit\nChoose: ";
        int mainChoice;
        cin >> mainChoice;
        if (mainChoice == 1)
        {
            currentUser = loginUser(users);
            if (!currentUser.username.empty())
                break; // Successful login
        }
        else if (mainChoice == 2)
        {
            registerUser(users, usersFile, false);
        }
        else if (mainChoice == 3)
        {
            cout << "Goodbye.\n";
            return 0;
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    // Main program loop
    while (true)
    {
        displayMenu(currentUser.role == "admin");
        int choice;
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        switch (choice)
        {
        case 1:
            addTransaction(transactions, recentTransactions);
            saveTransactions(transactions, transactionsFile);
            break;
        case 2:
            if (currentUser.role == "admin")
            {
                deleteTransaction(transactions);
                saveTransactions(transactions, transactionsFile);
            }
            else
            {
                cout << "Access denied. Only admins can delete transactions.\n";
            }
            break;
        case 3:
            displayTransactions(transactions);
            break;
        case 4:
            cout << "Logging out.\n";
            main();
            return 0;
        case 5:
            displayRecentTransactions(recentTransactions);
            break;
        default:
            cout << "Invalid menu choice.\n";
        }
    }
    return 0;
}
