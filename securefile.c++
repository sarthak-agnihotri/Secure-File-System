#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

void registerUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ofstream file("users.txt", ios::app);
    file << username << " " << password << endl;
    file.close();
    cout << "User registered successfully!" << endl;
}

bool loginUser() {
    string username, password, u, p;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream file("users.txt");
    while (file >> u >> p) {
        if (u == username && p == password) {
            cout << "Login successful!" << endl;
            return true;
        }
    }
    file.close();
    cout << "Invalid credentials!" << endl;
    return false;
}

void generateOTP() {
    srand(time(0));
    int otp = rand() % 10000; // Generate a 4-digit OTP
    cout << "Your OTP is: " << otp << endl;
    int enteredOTP;
    cout << "Enter OTP: ";
    cin >> enteredOTP;

    if (enteredOTP == otp) {
        cout << "2FA successful!" << endl;
    } else {
        cout << "Invalid OTP!" << endl;
    }
}

int main() {
    int choice;
    cout << "1. Register\n2. Login\nEnter your choice: ";
    cin >> choice;
    if (choice == 1) {
        registerUser();
    } else if (choice == 2) {
        if (loginUser()) {
            generateOTP();
        }
    }
    return 0;
}
 