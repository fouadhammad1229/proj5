#include "passserver.h"
#include <crypt.h>
#include <iostream>
#include <fstream>

PassServer::PassServer(size_t size) : serverData(size) {}

PassServer::~PassServer() {
    serverData.clear();
}

std::string PassServer::encrypt(const std::string &str) {
    char salt[] = "$1$########"; // Use a simple salt for encryption
    char *encrypted = crypt(str.c_str(), salt); // Encrypt the password
    return std::string(encrypted);
}


bool PassServer::load(const char *filename) {
    return serverData.load(filename); // Use HashTable's load function
}

bool PassServer::addUser(std::pair<std::string, std::string> &kv) {
    kv.second = encrypt(kv.second); // Encrypt the password
    return serverData.insert(kv);        // Use HashTable's insert
}

bool PassServer::addUser(std::pair<std::string, std::string> &&kv) {
    kv.second = encrypt(kv.second); // Encrypt the password
    return serverData.insert(std::move(kv)); // Use HashTable's move-insert
}

bool PassServer::removeUser(const std::string &k) {
    return serverData.remove(k); // Use HashTable's remove function
}

bool PassServer::changePassword(const std::pair<std::string, std::string> &p, const std::string &newpassword) {
    auto encryptedOldPassword = encrypt(p.second);
    auto encryptedNewPassword = encrypt(newpassword);

    if (encryptedOldPassword == encryptedNewPassword) {
        return false; // New password must be different from the old one
    }

    std::pair<std::string, std::string> user{p.first, encryptedOldPassword};
    if (serverData.match(user)) { // Check if user exists and the old password matches
        return serverData.insert({p.first, encryptedNewPassword}); // Update the password
    }

    return false; // User not found or old password doesn't match
}

bool PassServer::find(const std::string &user) const {
    return serverData.contains(user); // Check if the username exists in the hash table
}

void PassServer::dump() const {
    serverData.dump(); // Use HashTable's dump function to display content
}

size_t PassServer::size() const {
    return serverData.size(); // Return the number of username-password pairs
}

bool PassServer::write_to_file(const char *filename) const {
    return serverData.write_to_file(filename); // Use HashTable's write_to_file function
}

