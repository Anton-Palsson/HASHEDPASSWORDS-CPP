#include <iostream>
#include <string>
#include <regex>
#include "md5.h"
#include <fstream>
#include <limits>

using namespace std;

struct Users {
    string username;
    string password;
};

void LoadUsersFromFile(Users users[], int& userCount) {
    ifstream inFile("users.txt");
    if (!inFile) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    userCount = 0;
    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string username = line.substr(0, pos);
            string password = line.substr(pos + 1);
            users[userCount].username = username;
            users[userCount].password = password;
            userCount++;
        }
    }

    inFile.close();
}

void SaveUsersToFile(const Users users[], int userCount) {
    ofstream outFile("users.txt");
    if (!outFile) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    for (int i = 0; i < userCount; ++i) {
        outFile << users[i].username << ":" << users[i].password << endl;
    }

    outFile.close();
}

void TestLogin(const Users users[], int userCount, const string& username, const string& password) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username && md5(password) == users[i].password) {
            cout << "Login successful!\n";
            return;
        }
    }
    cout << "Invalid username or password.\n";
}

bool IsPasswordValid(const string& password) {
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;
    for (char ch : password) {
        if (islower(ch)) hasLower = true;
        else if (isupper(ch)) hasUpper = true;
        else if (isdigit(ch)) hasDigit = true;
        else hasSpecial = true;
    }
    return password.length() >= 8 && hasLower && hasUpper && hasDigit && hasSpecial;
}

bool IsEmailValid(const string& email) {
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

    return regex_match(email, pattern);
}

bool UserExists(const Users users[], int userCount, const string& username) {
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username) {
            return true;
        }
    }
    return false;
}

void CreateUser(Users users[], int& userCount, const string& username, const string& password, ofstream& outFile) {
    if (!IsEmailValid(username)) {
        cout << "Invalid email address format. Please enter a valid email address.\n";
        return;
    }

    if (UserExists(users, userCount, username)) {
        cout << "Username already exists. Please choose another username.\n";
        return;
    }

    string hashedPassword = md5(password);

    users[userCount].username = username;
    users[userCount].password = hashedPassword;

    userCount++;

    cout << "User created with username: " << username << " and hashed password: " << hashedPassword << endl;

    outFile << username << ":" << hashedPassword << endl;
    cout << "User data saved to file." << endl;
}

int main() {
    const int maxUsers = 100;
    Users users[maxUsers];
    int userCount = 0;

    LoadUsersFromFile(users, userCount);

    ofstream outFile("users.txt", ios::app);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return 1;
    }

    int menu;
    do {
        cout << "1. Create User\n";
        cout << "2. Test Login\n";
        cout << "3. Exit\n";
        cout << "4. Test Hash\n";
        cout << "Enter your choice: ";
        cin >> menu;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (menu) {
            case 1: {
                string username, password;
                cout << "Enter username (email address): ";
                getline(cin, username);
                if (!IsEmailValid(username)) {
                    cout << "Invalid email address format. Please enter a valid email address.\n";
                    break;
                }

                if (UserExists(users, userCount, username)) {
                    cout << "Username already exists. Please choose another username.\n";
                    break;
                }

                do {
                    cout << "Enter password (at least 8 characters with at least one uppercase, one lowercase, one digit, and one special character): ";
                    getline(cin, password);
                } while (!IsPasswordValid(password));

              CreateUser(users, userCount, username, password, outFile);
                break;
            }
            case 2: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                TestLogin(users, userCount, username, password);
                break;
            }
            case 3:
                SaveUsersToFile(users, userCount);
                cout << "Exiting program.\n";
                break;
            case 4: {
                string input = "Hello, world!";
                string hashed = md5(input);
                cout << "MD5 hash of '" << input << "': " << hashed << endl;
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (menu != 3);

    outFile.close();

    return 0;
}