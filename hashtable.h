#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <list>
#include <string>
#include <utility> // for std::pair

namespace cop4530 {

template <typename K, typename V>
class HashTable {
public:
    HashTable(size_t size = 101);
    ~HashTable();
    bool contains(const K& k) const;
    bool match(const std::pair<K, V>& kv) const;
    bool insert(const std::pair<K, V>& kv);
    bool insert(std::pair<K, V>&& kv);
    bool remove(const K& k);
    void clear();
    bool load(const char* filename);
    void dump() const;
    size_t size() const;
    bool write_to_file(const char* filename) const;

private:
    void makeEmpty();
    void rehash();
    size_t myhash(const K& k) const;
    unsigned long prime_below(unsigned long n);
    void setPrimes(std::vector<unsigned long>& primes);

    std::vector<std::list<std::pair<K, V>>> table;
    size_t currentSize;
};

} // namespace cop4530

#include "hashtable.hpp"

#endif // HASHTABLE_H

