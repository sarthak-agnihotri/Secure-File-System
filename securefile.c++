#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h> // For metadata
#include <ctime>      // For converting file modification time to readable format

using namespace std;

// Function to encrypt and decrypt content using XOR
void encryptDecrypt(string& data, const string& key) {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] ^= key[i % key.size()]; // XOR each character with the corresponding key character
    }
}

// Function to validate input length
bool validateInputLength(const string& input, size_t maxLength) {
    if (input.length() > maxLength) {
        cerr << "Error: Input exceeds the allowed length of " << maxLength << " characters." << endl;
        return false;
    }
    return true;
}

// Function for user registration
void registerUser() {
    const size_t MAX_LENGTH = 20; // Maximum length for username/password
    string username, password;

    cout << "Enter username (max " << MAX_LENGTH << " chars): ";
    cin >> username;
    if (!validateInputLength(username, MAX_LENGTH)) return;

    cout << "Enter password (max " << MAX_LENGTH << " chars): ";
    cin >> password;
    if (!validateInputLength(password, MAX_LENGTH)) return;

    ofstream file("users.txt", ios::app);
    file << username << " " << password << endl;
    file.close();
    cout << "User registered successfully!" << endl;
}

// Function for user login
bool loginUser() {
    const size_t MAX_LENGTH = 20; // Maximum length for username/password
    string username, password, u, p;

    cout << "Enter username (max " << MAX_LENGTH << " chars): ";
    cin >> username;
    if (!validateInputLength(username, MAX_LENGTH)) return false;

    cout << "Enter password (max " << MAX_LENGTH << " chars): ";
    cin >> password;
    if (!validateInputLength(password, MAX_LENGTH)) return false;

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

// Function to check file name validity
bool getFileName(string& filename) {
    const size_t MAX_FILENAME_LENGTH = 50; // Maximum length for file names
    cout << "Enter the file name (max " << MAX_FILENAME_LENGTH << " chars): ";
    cin >> filename;

    if (!validateInputLength(filename, MAX_FILENAME_LENGTH)) {
        return false;
    }

    return true;
}

// Function to write encrypted content to a file
void writeFile(const string& filename, const string& key) {
    ofstream file(filename, ios::app); // Open the file in append mode
    if (file.is_open()) {
        string content;
        cout << "Enter content to write into the file: ";
        cin.ignore(); // Ignore leftover newline characters
        getline(cin, content); // Read the entire line as content

        encryptDecrypt(content, key); // Encrypt the content using XOR encryption
        file << content << endl; // Write encrypted content to the file
        file.close();
        cout << "Content written and encrypted successfully!" << endl;
    } else {
        cerr << "Error: Unable to open the file for writing." << endl;
    }
}

// Function to read and decrypt content from a file
void readFile(const string& filename, const string& key) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        cout << "Decrypted File Contents:" << endl;
        while (getline(file, line)) { // Read each line from the file
            encryptDecrypt(line, key); // Decrypt the content using XOR decryption
            cout << line << endl; // Display the decrypted content
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
    string filename; // File name will be asked once
    string key;      // Encryption/Decryption key

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
                    if (!getFileName(filename)) {
                        authenticated = false; // Revert authentication if invalid file name
                        continue;
                    }
                    cout << "Enter an encryption key: ";
                    cin >> key;
                }
            } else if (choice == 3) {
                cout << "Exiting the program..." << endl;
                break;
            } else {
                cerr << "Invalid choice! Please try again." << endl;
            }
        } else {
            cout << "\nAuthenticated Operations Menu:\n";
            cout << "1. Write to File (Encrypted)\n";
            cout << "2. Read File (Decrypted)\n";
            cout << "3. View File Accessibility\n";
            cout << "4. Display File Metadata\n";
            cout << "5. Logout\n";
            cout << "6. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    writeFile(filename, key);
                    break;
                case 2:
                    readFile(filename, key);
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
