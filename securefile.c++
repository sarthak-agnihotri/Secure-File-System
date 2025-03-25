#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <sys/stat.h> // For file metadata
#include <ctime>      // For converting file modification time to readable format

using namespace std;

// Declare Access Matrix
map<string, map<string, string>> accessMatrix; // Format: accessMatrix[username][operation] = permission

// Function to initialize Access Matrix for Admin only
void initializeAccessMatrix() {
    accessMatrix["Admin"]["Write"] = "Allow";
    accessMatrix["Admin"]["Read"] = "Allow";
    accessMatrix["Admin"]["Share"] = "Allow";
    accessMatrix["Admin"]["Metadata"] = "Allow";
}

// Function to check access permissions
bool hasPermission(const string& username, const string& operation) {
    if (accessMatrix.find(username) != accessMatrix.end() &&
        accessMatrix[username].find(operation) != accessMatrix[username].end()) {
        return accessMatrix[username][operation] == "Allow";
    }
    return false; // Default: No access if not explicitly defined
}

// Function to modify Access Matrix dynamically (Admin only)
void modifyAccessPermissions(const string& adminUsername) {
    if (adminUsername != "Admin") {
        cerr << "Error: Only Admin can modify access permissions!" << endl;
        return;
    }

    string targetUser, operation, permission;
    cout << "Enter the username to modify permissions for: ";
    cin >> targetUser;

    if (accessMatrix.find(targetUser) == accessMatrix.end()) {
        cerr << "Error: User does not exist!" << endl;
        return;
    }

    cout << "Enter the operation (Write, Read, Share, Metadata): ";
    cin >> operation;
    cout << "Enter the new permission (Allow or NoAccess): ";
    cin >> permission;

    if (permission != "Allow" && permission != "NoAccess") {
        cerr << "Error: Invalid permission! Use 'Allow' or 'NoAccess'." << endl;
        return;
    }

    accessMatrix[targetUser][operation] = permission;
    cout << "Permission updated successfully for " << targetUser << "!" << endl;
}

// Function to encrypt and decrypt using XOR
void encryptDecrypt(string& data, const string& key) {
    for (size_t i = 0; i < data.size(); i++) {
        data[i] ^= key[i % key.size()];
    }
}

// Function to validate input length
bool validateInputLength(const string& input, size_t maxLength) {
    if (input.length() > maxLength) {
        cerr << "Error: Input exceeds the maximum allowed length of " << maxLength << " characters." << endl;
        return false;
    }
    return true;
}

// Function to register a user
void registerUser() {
    const size_t MAX_LENGTH = 20;
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

    // Add new user to the Access Matrix with default permissions
    accessMatrix[username]["Write"] = "NoAccess";
    accessMatrix[username]["Read"] = "NoAccess";
    accessMatrix[username]["Share"] = "NoAccess";
    accessMatrix[username]["Metadata"] = "NoAccess";
}

// Function to log in a user
bool loginUser(string& username) {
    const size_t MAX_LENGTH = 20;
    string password, u, p;

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
    int otp = rand() % 10000; // Generate 4-digit OTP
    cout << "Your OTP is: " << otp << endl;

    int enteredOTP;
    cout << "Enter OTP: ";
    cin >> enteredOTP;

    if (enteredOTP == otp) {
        cout << "2FA successful!" << endl;
    } else {
        cerr << "Invalid OTP!" << endl;
        exit(0);
    }
}

// Function to write to a file
void writeFile(const string& username, const string& filename, const string& key) {
    if (!hasPermission(username, "Write")) {
        cerr << "Error: You do not have permission to write to files!" << endl;
        return;
    }

    ofstream file(filename, ios::app);
    if (file.is_open()) {
        string content;
        cout << "Enter content to write into the file: ";
        cin.ignore();
        getline(cin, content);

        encryptDecrypt(content, key);
        file << content << endl;
        file.close();
        cout << "Content written and encrypted successfully!" << endl;
    } else {
        cerr << "Error: Unable to open the file for writing." << endl;
    }
}

// Function to read from a file
void readFile(const string& username, const string& filename, const string& key) {
    if (!hasPermission(username, "Read")) {
        cerr << "Error: You do not have permission to read files!" << endl;
        return;
    }

    ifstream file(filename);
    if (file.is_open()) {
        string line;
        cout << "Decrypted File Contents:" << endl;
        while (getline(file, line)) {
            encryptDecrypt(line, key);
            cout << line << endl;
        }
        file.close();
    } else {
        cerr << "Error: Unable to open the file for reading." << endl;
    }
}

// Function to display file metadata
void displayMetadata(const string& username, const string& filename) {
    if (!hasPermission(username, "Metadata")) {
        cerr << "Error: You do not have permission to view file metadata!" << endl;
        return;
    }

    struct stat fileStats;
    if (stat(filename.c_str(), &fileStats) == 0) {
        cout << "File Metadata:" << endl;
        cout << "File Size: " << fileStats.st_size << " bytes" << endl;
        cout << "Last Modified: " << ctime(&fileStats.st_mtime);
    } else {
        cerr << "Error: Unable to retrieve metadata for the file." << endl;
    }
}

// Function to share a file
void shareFile(const string& username, const string& sourceFilename, const string& destinationFilename, const string& key) {
    if (!hasPermission(username, "Share")) {
        cerr << "Error: You do not have permission to share files!" << endl;
        return;
    }

    ifstream sourceFile(sourceFilename);
    ofstream destinationFile(destinationFilename);

    if (sourceFile.is_open() && destinationFile.is_open()) {
        string line;
        cout << "Sharing file content..." << endl;
        while (getline(sourceFile, line)) {
            encryptDecrypt(line, key);
            destinationFile << line << endl;
        }
        sourceFile.close();
        destinationFile.close();
        cout << "File successfully shared to: " << destinationFilename << endl;
    } else {
        cerr << "Error: Unable to open source or destination file for sharing." << endl;
    }
}

// Main function
int main() {
    int choice;
    bool authenticated = false;
    string filename, key, username;

    initializeAccessMatrix(); // Initialize access permissions for Admin
    ifstream file;
    do {
        if (!authenticated) {
            cout << "\n1. Register\n2. Login\n3. Exit\nEnter your choice: ";
            cin >> choice;

            if (choice == 1) {
                registerUser();
            } else if (choice == 2) {
                if (loginUser(username)) {
                    generateOTP();
                    authenticated = true;

                    cout << "Enter the file name to work with: ";
                    cin >> filename;

                    cout << "Enter encryption key: ";
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
            cout << "5. Share File\n";
            cout << "6. Modify Access Permissions\n";
            cout << "7. Logout\n";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    writeFile(username, filename, key);
                    break;
                case 2:
                    readFile(username, filename, key);
                    break;
                case 3:
                    file.open(filename);
                    if (file.is_open()) {
                        cout << "File: " << filename << " exists and is accessible." << endl;
                        file.close();
                    } else {
                        cerr << "Error: File does not exist or cannot be opened." << endl;
                    }
                    break;
                case 4:
                    displayMetadata(username, filename);
                    break;
                case 5: {
                    string destinationFilename;
                    cout << "Enter the destination file name to share the content: ";
                    cin >> destinationFilename;
                    shareFile(username, filename, destinationFilename, key);
                    break;
                }
                case 6:
                    modifyAccessPermissions(username);
                    break;
                case 7:
                    authenticated = false; // Logout the user
                    cout << "Logged out successfully." << endl;
                    break;
                case 8:
                    cout << "Exiting the program..." << endl;
                    return 0;
                default:
                    cerr << "Invalid choice! Please try again." << endl;
            }
        }
    } while (true);

    return 0;
}
