// #include <iostream>
// #include <fstream>
// #include <string>
// #include <cstdlib>
// #include <ctime>

// using namespace std;

// void registerUser() {
//     string username, password;
//     cout << "Enter username: ";
//     cin >> username;
//     cout << "Enter password: ";
//     cin >> password;

//     ofstream file("users.txt", ios::app);
//     file << username << " " << password << endl;
//     file.close();
//     cout << "User registered successfully!" << endl;
// }

// bool loginUser() {
//     string username, password, u, p;
//     cout << "Enter username: ";
//     cin >> username;
//     cout << "Enter password: ";
//     cin >> password;

//     ifstream file("users.txt");
//     while (file >> u >> p) {
//         if (u == username && p == password) {
//             cout << "Login successful!" << endl;
//             return true;
//         }
//     }
//     file.close();
//     cout << "Invalid credentials!" << endl;
//     return false;
// }

// void generateOTP() {
//     srand(time(0));
//     int otp = rand() % 10000; // Generate a 4-digit OTP
//     cout << "Your OTP is: " << otp << endl;
//     int enteredOTP;
//     cout << "Enter OTP: ";
//     cin >> enteredOTP;

//     if (enteredOTP == otp) {
//         cout << "2FA successful!" << endl;
//     } else {
//         cout << "Invalid OTP!" << endl;
//     }
// }

// int main() {
//     int choice;
//     cout << "1. Register\n2. Login\nEnter your choice: ";
//     cin >> choice;
//     if (choice == 1) {
//         registerUser();
//     } else if (choice == 2) {
//         if (loginUser()) {
//             generateOTP();
//         }
//     }
//     return 0;
// }
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h> // For metadata
#include <ctime>      // For converting file modification time to readable format

using namespace std;

// Function for user registration
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

// Function for user login
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

// Function to generate a 2FA OTP
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
        exit(0); // Exit if 2FA fails
    }
}

// Function to write to a file
void writeFile(const string& filename) {
    ofstream file(filename, ios::app); // Open the file in append mode
    if (file.is_open()) {
        string content;
        cout << "Enter content to write into the file: ";
        cin.ignore(); // Ignore leftover newline characters
        getline(cin, content); // Read the entire line as content
        file << content << endl; // Write content to the file
        file.close();
        cout << "Content written successfully!" << endl;
    } else {
        cerr << "Error: Unable to open the file for writing." << endl;
    }
}

// Function to read from a file
void readFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        cout << "File Contents:" << endl;
        while (getline(file, line)) { // Read each line from the file
            cout << line << endl;
        }
        file.close();
    } else {
        cerr << "Error: Unable to open the file for reading." << endl;
    }
}

// Function to check file accessibility
void viewFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        cout << "File: " << filename << " exists and is accessible." << endl;
        file.close();
    } else {
        cerr << "Error: File does not exist or cannot be opened." << endl;
    }
}

// Function to display file metadata
void displayMetadata(const string& filename) {
    struct stat fileStats;
    if (stat(filename.c_str(), &fileStats) == 0) {
        cout << "File Metadata:" << endl;
        cout << "File Size: " << fileStats.st_size << " bytes" << endl;
        cout << "Last Modified: " << ctime(&fileStats.st_mtime); // Convert to human-readable format
    } else {
        cerr << "Error: Unable to retrieve metadata for the file." << endl;
    }
}

int main() {
    int choice;
    bool authenticated = false;
    string filename; // File name will be asked once here

    do {
        if (!authenticated) {
            cout << "\n1. Register\n2. Login\n3. Exit\nEnter your choice: ";
            cin >> choice;
            if (choice == 1) {
                registerUser();
            } else if (choice == 2) {
                if (loginUser()) {
                    generateOTP();
                    authenticated = true; // Mark user as authenticated
                    cout << "Enter the file name you want to work with: ";
                    cin >> filename;
                }
            } else if (choice == 3) {
                cout << "Exiting the program..." << endl;
                break;
            } else {
                cerr << "Invalid choice! Please try again." << endl;
            }
        } else {
            cout << "\nAuthenticated Operations Menu:\n";
            cout << "1. Write to File\n";
            cout << "2. Read File\n";
            cout << "3. View File Accessibility\n";
            cout << "4. Display File Metadata\n";
            cout << "5. Logout\n";
            cout << "6. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    writeFile(filename);
                    break;
                case 2:
                    readFile(filename);
                    break;
                case 3:
                    viewFile(filename);
                    break;
                case 4:
                    displayMetadata(filename);
                    break;
                case 5:
                    authenticated = false; // Logout the user
                    cout << "Logged out successfully." << endl;
                    break;
                case 6:
                    cout << "Exiting the program..." << endl;
                    return 0;
                default:
                    cerr << "Invalid choice! Please try again." << endl;
            }
        }
    } while (true);

    return 0;
}
