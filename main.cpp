#include "passserver.h"
#include <iostream>
#include <string>

void Menu() {
    std::cout << "\n\n";
    std::cout << "l - Load From File\n";
    std::cout << "a - Add User\n";
    std::cout << "r - Remove User\n";
    std::cout << "c - Change User Password\n";
    std::cout << "f - Find User\n";
    std::cout << "d - Dump HashTable\n";
    std::cout << "s - HashTable Size\n";
    std::cout << "w - Write to Password File\n";
    std::cout << "x - Exit program\n";
    std::cout << "\nEnter choice : ";
}

int main() {
    PassServer ps;  // Initialize PassServer object
    char choice;
    do {
        Menu();
        std::cin >> choice;

        std::string username, password, newPassword;
        std::pair<std::string, std::string> userCredentials;

        switch (choice) {
        case 'l': {  // Load from file
            std::string filename;
            std::cout << "Enter filename to load: ";
            std::cin >> filename;
            if (ps.load(filename.c_str())) {
                std::cout << "File loaded successfully.\n";
            } else {
                std::cout << "Error loading file.\n";
            }
            break;
        }
        case 'a': {  // Add user
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            userCredentials = {username, password};
            if (ps.addUser(userCredentials)) {
                std::cout << "User added successfully.\n";
            } else {
                std::cout << "User already exists.\n";
            }
            break;
        }
        case 'r': {  // Remove user
            std::cout << "Enter username to remove: ";
            std::cin >> username;
            if (ps.removeUser(username)) {
                std::cout << "User removed successfully.\n";
            } else {
                std::cout << "User not found.\n";
            }
            break;
        }
        case 'c': {  // Change user password
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter current password: ";
            std::cin >> password;
            std::cout << "Enter new password: ";
            std::cin >> newPassword;
            userCredentials = {username, password};
            if (ps.changePassword(userCredentials, newPassword)) {
                std::cout << "Password changed successfully.\n";
            } else {
                std::cout << "Failed to change password.\n";
            }
            break;
        }
        case 'f': {  // Find user
            std::cout << "Enter username to find: ";
            std::cin >> username;
            if (ps.find(username)) {
                std::cout << "User found.\n";
            } else {
                std::cout << "User not found.\n";
            }
            break;
        }
        case 'd': {  // Dump hash table
            ps.dump();
            break;
        }
        case 's': {  // Show hash table size
            std::cout << "Hash table size: " << ps.size() << "\n";
            break;
        }
        case 'w': {  // Write to password file
            std::string filename;
            std::cout << "Enter filename to save: ";
            std::cin >> filename;
            if (ps.write_to_file(filename.c_str())) {
                std::cout << "Data written to file successfully.\n";
            } else {
                std::cout << "Failed to write to file.\n";
            }
            break;
        }
        case 'x': {  // Exit program
            std::cout << "Exiting program.\n";
            break;
        }
        default: {
            std::cout << "Invalid choice. Try again.\n";
            break;
        }
        }
    } while (choice != 'x');

    return 0;
}

