#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

struct Transaction {
    char type[20];
    double amount;
    char date[20];
};

struct User {
    char name[50];
    char cnic[20];
    char pin[6];
    double balance;
    Transaction* history;
    int historyCount;
    int historyCap;
};

User* users = NULL;
int userCount = 0;
int userCap = 0;

void initUsersArray() {
    userCap = 4;
    users = new User[userCap];
    userCount = 0;
}

void ensureUserCapacity() {
    if (users == NULL) {
        initUsersArray();
        return;
    }
    if (userCount < userCap) return;
    int newCap = userCap * 2;
    User* temp = new User[newCap];
    for (int i = 0; i < userCount; ++i) temp[i] = users[i];
    delete [] users;
    users = temp;
    userCap = newCap;
}

void initHistory(User &u) {
    u.historyCap = 4;
    u.historyCount = 0;
    u.history = new Transaction[u.historyCap];
}

void ensureHistoryCapacity(User &u) {
    if (u.history == NULL) { initHistory(u); return; }
    if (u.historyCount < u.historyCap) return;
    int newCap = u.historyCap * 2;
    Transaction* temp = new Transaction[newCap];
    for (int i = 0; i < u.historyCount; ++i) temp[i] = u.history[i];
    delete [] u.history;
    u.history = temp;
    u.historyCap = newCap;
}

void currentDateString(char out[20]) {
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    sprintf(out, "%04d%02d%02d_%02d%02d%02d",
            lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
            lt->tm_hour, lt->tm_min, lt->tm_sec);
}

void pauseSimple() {
    cout << "Press Enter to continue...";
    char ch;
    while (cin.get(ch)) {
        if (ch == '\n') break;
    }
    cin.get();
}

void saveUsersBinary(const char* filename) {
    ofstream fout(filename, ios::binary);
    if (!fout) {
        cout << "Error saving users (binary).\n";
        return;
    }
    fout.write((char*)&userCount, sizeof(int));
    for (int i = 0; i < userCount; ++i) {
        fout.write(users[i].name, 50);
        fout.write(users[i].cnic, 20);
        fout.write(users[i].pin, 6);
        fout.write((char*)&users[i].balance, sizeof(double));
    }
    fout.close();
}

void loadUsersBinary(const char* filename) {
    ifstream fin(filename, ios::binary);
    if (!fin) return;
    int count = 0;
    fin.read((char*)&count, sizeof(int));
    if (count <= 0) { fin.close(); return; }

    if (users != NULL) delete [] users;
    userCap = (count > 4) ? count : 4;
    users = new User[userCap];
    userCount = 0;

    for (int i = 0; i < count; ++i) {
        User tmp;
        fin.read(tmp.name, 50);
        fin.read(tmp.cnic, 20);
        fin.read(tmp.pin, 6);
        fin.read((char*)&tmp.balance, sizeof(double));
        tmp.history = NULL;
        tmp.historyCount = 0;
        tmp.historyCap = 0;
        users[userCount++] = tmp;
    }
    fin.close();
}

void saveHistoryText(const char* filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Error saving history text.\n";
        return;
    }
    for (int i = 0; i < userCount; ++i) {
        fout << users[i].cnic << "\n";
        fout << users[i].historyCount << "\n";
        for (int j = 0; j < users[i].historyCount; ++j) {
            fout << users[i].history[j].type << " "
                 << users[i].history[j].amount << " "
                 << users[i].history[j].date << "\n";
        }
        fout << "\n";
    }
    fout.close();
}

void loadHistoryText(const char* filename) {
    ifstream fin(filename);
    if (!fin) return;
    while (!fin.eof()) {
        char cnicBuf[20];
        fin >> cnicBuf;
        if (!fin) break;
        int n = 0;
        fin >> n;
        if (!fin) break;
        int idx = -1;
        for (int i = 0; i < userCount; ++i) {
            if (strcmp(users[i].cnic, cnicBuf) == 0) { idx = i; break; }
        }
        if (idx == -1) {
            for (int k = 0; k < n; ++k) {
                char skipType[20], skipDate[20];
                double skipAmt;
                fin >> skipType >> skipAmt >> skipDate;
            }
            continue;
        }
        if (users[idx].history != NULL) delete [] users[idx].history;
        users[idx].historyCap = (n > 4) ? n : 4;
        users[idx].history = new Transaction[users[idx].historyCap];
        users[idx].historyCount = 0;
        for (int j = 0; j < n; ++j) {
            Transaction t;
            fin >> t.type >> t.amount >> t.date;
            users[idx].history[users[idx].historyCount++] = t;
        }
    }
    fin.close();
}

bool validateName(const char* name) {
    int len = strlen(name);
    if (len == 0 || len >= 50) return false;

    for (int i = 0; i < len; i++) {
        if (!isalpha(name[i])) return false;
    }
    return true;
}

int findUserByCNIC(const char* cnic) {
    for (int i = 0; i < userCount; ++i) {
        if (strcmp(users[i].cnic, cnic) == 0) return i;
    }
    return -1;
}
bool validateCNIC(const char* cnic) {
    if (strlen(cnic) != 15) return false;

    for (int i = 0; i < 15; i++) {
        if (i == 5 || i == 13) {
            if (cnic[i] != '-') return false; 
        } else {
            if (!isdigit(cnic[i])) return false;
        }
    }
    return true;
}

void createUser() {
    ensureUserCapacity();
    User u;

    cout << "Enter name (letters only): ";
    cin >> u.name;

    for (int i = 0; u.name[i] != '\0'; i++) {
        if (!((u.name[i] >= 'A' && u.name[i] <= 'Z') ||
              (u.name[i] >= 'a' && u.name[i] <= 'z'))) {
            cout << "Invalid name! Only letters allowed.\n";
            return;
        }
    }

    cout << "Enter CNIC (format 12345-1234567-1): ";
    cin >> u.cnic;

    if (!validateCNIC(u.cnic)) {
        cout << "Invalid CNIC format!\n";
        return;
    }

    if (findUserByCNIC(u.cnic) != -1) {
        cout << "User with this CNIC already exists.\n";
        return;
    }

    cout << "Enter 4-digit PIN: ";
    cin >> u.pin;

    if (strlen(u.pin) != 4 || !isdigit(u.pin[0]) || !isdigit(u.pin[1]) ||
        !isdigit(u.pin[2]) || !isdigit(u.pin[3])) {
        cout << "Invalid PIN! Must be 4 digits.\n";
        return;
    }

    u.balance = 0.0;
    u.history = NULL;
    u.historyCount = 0;
    u.historyCap = 0;

    users[userCount++] = u;
    cout << "User created successfully.\n";
}

void removeUser() {
    char cnic[20];
    cout << "Enter CNIC of user to delete: ";
    cin >> cnic;
    int idx = findUserByCNIC(cnic);
    if (idx == -1) { cout << "No user found.\n"; return; }
    if (users[idx].history != NULL) delete [] users[idx].history;
    for (int i = idx; i < userCount - 1; ++i) users[i] = users[i+1];
    userCount--;
    cout << "User deleted.\n";
}

void addTransactionToUser(User &u, const char* type, double amount) {
    ensureHistoryCapacity(u);
    Transaction t;
    int k = 0;
    while (type[k] != '\0' && k < 19) { t.type[k] = type[k]; k++; }
    t.type[k] = '\0';
    t.amount = amount;
    currentDateString(t.date);
    u.history[u.historyCount++] = t;
}

bool addMoneyToUser(User &u, double amt) {
    if (amt <= 0.0) return false;
    u.balance += amt;
    addTransactionToUser(u, "CashIn", amt);
    return true;
}

bool withdrawFromUser(User &u, double amt) {
    if (amt <= 0.0) return false;
    if (amt > u.balance) return false;
    u.balance -= amt;
    addTransactionToUser(u, "CashOut", -amt);
    return true;
}

bool transferBetweenUsers(User &from, User &to, double amt) {
    if (amt <= 0.0) return false;
    if (amt > from.balance) return false;
    from.balance -= amt;
    to.balance += amt;
    addTransactionToUser(from, "TransferOut", -amt);
    addTransactionToUser(to, "TransferIn", amt);
    return true;
}

void printAllUsers() {
    cout << "All users:\n";
    for (int i = 0; i < userCount; ++i) {
        cout << i+1 << ") " << users[i].name << " | " << users[i].cnic << " | bal: " << users[i].balance << "\n";
    }
}

void showUserStatement(const User &u) {
    cout << "Statement for " << u.name << " (" << u.cnic << ")\n";
    cout << "Balance: " << u.balance << "\n";
    cout << "Transactions (" << u.historyCount << "):\n";
    for (int i = 0; i < u.historyCount; ++i) {
        cout << i+1 << ") " << u.history[i].type << " " << u.history[i].amount << " " << u.history[i].date << "\n";
    }
}

int loginSimple() {
    char cnic[20], pin[6];
    cout << "Login - Enter CNIC: ";
    cin >> cnic;
    cout << "PIN: ";
    cin >> pin;
    int idx = findUserByCNIC(cnic);
    if (idx == -1) { cout << "User not found.\n"; return -1; }
    if (strcmp(users[idx].pin, pin) != 0) { cout << "Wrong PIN.\n"; return -1; }
    cout << "Welcome, " << users[idx].name << "\n";
    return idx;
}

void showMainMenu() {
    cout << "\n=== MAIN MENU ===\n";
    cout << "1. Create user\n";
    cout << "2. Login\n";
    cout << "3. Delete user\n";
    cout << "4. List users\n";
    cout << "5. Save and exit\n";
    cout << "Choice: ";
}
bool payInternetBill(User &u, double amount) {
    if (amount <= 0) {
        cout << "Invalid bill amount.\n";
        return false;
    }
    if (amount > u.balance) {
        cout << "Insufficient balance.\n";
        return false;
    }

    u.balance -= amount;
    addTransactionToUser(u, "InternetBill", -amount);

    cout << "Internet bill paid.\n";
    return true;
}
bool payElectricityBill(User &u, double amount) {
    if (amount <= 0) {
        cout << "Invalid bill amount.\n";
        return false;
    }
    if (amount > u.balance) {
        cout << "Insufficient balance.\n";
        return false;
    }

    u.balance -= amount;
    addTransactionToUser(u, "ElectricityBill", -amount);

    cout << "Electricity bill paid successfully.\n";
    return true;
}
bool payGasBill(User &u, double amount) {
    if (amount <= 0) {
        cout << "Invalid bill amount.\n";
        return false;
    }
    if (amount > u.balance) {
        cout << "Insufficient balance.\n";
        return false;
    }

    u.balance -= amount;
    addTransactionToUser(u, "GasBill", -amount);

    cout << "Gas bill paid successfully.\n";
    return true;
}
bool payTrafficChallan(User &u, double amount) {
    if (amount <= 0) {
        cout << "Invalid challan amount.\n";
        return false;
    }
    if (amount > u.balance) {
        cout << "Insufficient balance.\n";
        return false;
    }

    u.balance -= amount;
    addTransactionToUser(u, "TrafficChallan", -amount);

    cout << "Traffic challan paid successfully.\n";
    return true;
}

void showWalletMenu() {
    cout << "\n--- WALLET MENU ---\n";
    cout << "1. Add money\n";
    cout << "2. Cash out\n";
    cout << "3. Check balance\n";
    cout << "4. Statement\n";
    cout << "5. Transfer\n";
    cout << "6. Change PIN\n";
    cout << "7. Pay Bills\n";
    cout << "8. Logout\n";
    cout << "Choice: ";
}

void changePinForUser(User &u) {
    char oldp[6], newp[6];
    cout << "Enter current PIN: ";
    cin >> oldp;
    if (strcmp(oldp, u.pin) != 0) { cout << "Wrong PIN.\n"; return; }
    cout << "Enter new 4-digit PIN: ";
    cin >> newp;
    int i = 0;
    while (newp[i] != '\0' && i < 5) { u.pin[i] = newp[i]; i++; }
    u.pin[i] = '\0';
    cout << "PIN changed.\n";
}
void showBillMenu() {
    cout << "\n--- BILL PAYMENT MENU ---\n";
    cout << "1. Electricity Bill\n";
    cout << "2. Internet Bill\n";
    cout << "3. Gas Bill\n";
    cout << "4. Traffic Challan\n";
    cout << "5. Go Back\n";
    cout << "Choice: ";
}

int main() {
    srand((unsigned)time(NULL));

    loadUsersBinary("users.dat");
    loadHistoryText("history.txt");

    bool running = true;
    while (running) {
        showMainMenu();
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            char tmp;
            cin >> tmp;
            continue;
        }
        if (choice == 1) {
            createUser();
        } else if (choice == 2) {
            int idx = loginSimple();
            if (idx == -1) continue;
            bool inWallet = true;
            while (inWallet) {
                showWalletMenu();
                int w;
                cin >> w;
                if (w == 1) {
                    double amt;
                    cout << "Amount to add: ";
                    cin >> amt;
                    if (addMoneyToUser(users[idx], amt)) cout << "Added.\n";
                    else cout << "Invalid amount.\n";
                }
                else if (w == 2) {
                    double amt;
                    cout << "Amount to cash out: ";
                    cin >> amt;
                    if (withdrawFromUser(users[idx], amt)) cout << "Cash-out done.\n";
                    else cout << "Failed (insufficient or invalid).\n";
                }
                else if (w == 3) {
                    cout << "Balance: " << users[idx].balance << "\n";
                }
                else if (w == 4) {
                    showUserStatement(users[idx]);
                }
                else if (w == 5) {
                    char toCnic[20];
                    cout << "Recipient CNIC: ";
                    cin >> toCnic;
                    int tidx = findUserByCNIC(toCnic);
                    if (tidx == -1) { cout << "Recipient not found.\n"; }
                    else {
                        double amt;
                        cout << "Amount to transfer: ";
                        cin >> amt;
                        if (transferBetweenUsers(users[idx], users[tidx], amt)) cout << "Transfer done.\n";
                        else cout << "Transfer failed.\n";
                    }
                }
                else if (w == 6) {
                    changePinForUser(users[idx]);
                }
                else if (w == 8) {
                    inWallet = false;
                }
                else if (w == 7) {
                    bool paying = true;
                    while (paying) {
                        showBillMenu();
                        int b;
                        cin >> b;

                        if (b == 1) {
                            double amt;
                            cout << "Enter electricity bill amount: ";
                            cin >> amt;
                            payElectricityBill(users[idx], amt);
                        }
                        else if (b == 2) {
                            double amt;
                            cout << "Enter internet bill amount: ";
                            cin >> amt;
                            payInternetBill(users[idx], amt);
                        }else if (b == 3) {
                            double amt;
                            cout << "Enter gas bill amount: ";
                            cin >> amt;
                            payGasBill(users[idx], amt);
                        }
                        else if (b == 4) {
                            double amt;
                            cout << "Enter challan amount: ";
                            cin >> amt;
                            payTrafficChallan(users[idx], amt);
                        }
                        else if (b == 5) {
                            paying = false;
                        }
                        else {
                            cout << "Invalid option.\n";
                        }
                    }
                }
                else {
                    cout << "Invalid option.\n";
                }
            }
        } else if (choice == 3) {
            removeUser();
        } else if (choice == 4) {
            printAllUsers();
        } else if (choice == 5) {
            saveUsersBinary("users.dat");
            saveHistoryText("history.txt");
            running = false;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    for (int i = 0; i < userCount; ++i) {
        if (users[i].history != NULL) delete [] users[i].history;
    }
    if (users != NULL) delete [] users;

    cout << "Program closed.\n";
    return 0;
}