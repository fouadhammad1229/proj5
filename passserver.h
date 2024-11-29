#ifndef PASSSERVER_H
#define PASSSERVER_H

#include "hashtable.h"
#include <string>
#include <utility> // for std::pair

class PassServer {
public:
    explicit PassServer(size_t size = 101);
    ~PassServer();
    bool load(const char* filename);
    bool addUser(std::pair<std::string, std::string>& kv);
    bool addUser(std::pair<std::string, std::string>&& kv);
    bool removeUser(const std::string& k);
    bool changePassword(const std::pair<std::string, std::string>& p, const std::string& newpassword);
    bool find(const std::string& user) const;
    void dump() const;
    size_t size() const;
    bool write_to_file(const char* filename) const;

private:
    std::string encrypt(const std::string& str);
    cop4530::HashTable<std::string, std::string> serverData;
};

#endif // PASSSERVER_H

