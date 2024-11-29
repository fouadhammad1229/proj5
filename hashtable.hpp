#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "hashtable.h"
#include <fstream>
#include <iostream>
#include <functional>

namespace cop4530 {

template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size) : table(prime_below(size)), currentSize(0) {}

template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    clear();
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K& k) const {
    auto& whichList = table[myhash(k)];
    for (const auto& kv : whichList) {
        if (kv.first == k) return true;
    }
    return false;
}

template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> &kv) const {
    // Get the index of the bucket using the hash function
    auto &whichList = table[myhash(kv.first)];
    
    // Iterate through the bucket to check for the key-value pair
    for (const auto &entry : whichList) {
        if (entry.first == kv.first && entry.second == kv.second) {
            return true; // Match found
        }
    }

    // No matching key-value pair found
    return false;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> &kv) {
    // Get the bucket index using the hash function
    auto &whichList = table[myhash(kv.first)];

    // Iterate through the bucket to check if the key already exists
    for (auto &entry : whichList) {
        if (entry.first == kv.first) {
            if (entry.second == kv.second) {
                // Key-value pair already exists, no need to update
                return false;
            } else {
                // Key exists but value is different; update the value
                entry.second = kv.second;
                return true;
            }
        }
    }

    // Key does not exist; insert the new key-value pair
    whichList.push_back(kv);
    ++currentSize;

    // Rehash if the load factor exceeds a threshold (e.g., 1.0)
    if (currentSize > table.size()) {
        rehash();
    }

    return true;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> &&kv) {
    // Get the bucket index using the hash function
    auto &whichList = table[myhash(kv.first)];

    // Iterate through the bucket to check if the key already exists
    for (auto &entry : whichList) {
        if (entry.first == kv.first) {
            if (entry.second == kv.second) {
                // Key-value pair already exists, no need to update
                return false;
            } else {
                // Key exists but value is different; update the value
                entry.second = std::move(kv.second);
                return true;
            }
        }
    }

    // Key does not exist; move the new key-value pair into the bucket
    whichList.push_back(std::move(kv));
    ++currentSize;

    // Rehash if the load factor exceeds a threshold (e.g., 1.0)
    if (currentSize > table.size()) {
        rehash();
    }

    return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K &k) {
    // Get the bucket index using the hash function
    auto &whichList = table[myhash(k)];

    // Iterate through the bucket to find the key
    for (auto it = whichList.begin(); it != whichList.end(); ++it) {
        if (it->first == k) {
            // Key found; remove the key-value pair
            whichList.erase(it);
            --currentSize;
            return true; // Successfully removed
        }
    }

    // Key not found
    return false;
}

template <typename K, typename V>
void HashTable<K, V>::clear() {
    // Iterate through all buckets and clear each list
    for (auto &bucket : table) {
        bucket.clear();
    }
    currentSize = 0; // Reset the size to 0
}

template <typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {
    std::ifstream infile(filename);
    if (!infile) {
        // File couldn't be opened
        return false;
    }

    K key;
    V value;

    // Read key-value pairs line by line
    while (infile >> key >> value) {
        // Insert each key-value pair into the hash table
        insert(std::make_pair(key, value));
    }

    infile.close();
    return true; // Successfully loaded all pairs
}

template <typename K, typename V>
void HashTable<K, V>::dump() const {
    for (size_t i = 0; i < table.size(); ++i) {
        std::cout << "[" << i << "]:";
        for (const auto &kv : table[i]) {
            std::cout << " " << kv.first << ":" << kv.second << ";";
        }
        std::cout << std::endl;
    }
}

template <typename K, typename V>
size_t HashTable<K, V>::size() const {
    return currentSize; // Return the number of elements currently in the table
}

template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const {
    std::ofstream outfile(filename);
    if (!outfile) {
        // File couldn't be opened
        return false;
    }

    for (const auto &bucket : table) {
        for (const auto &kv : bucket) {
            outfile << kv.first << " " << kv.second << std::endl;
        }
    }

    outfile.close();
    return true; // Successfully wrote to the file
}

template <typename K, typename V>
void HashTable<K, V>::makeEmpty() {
    for (auto &bucket : table) {
        bucket.clear(); // Clear each bucket (list)
    }
    currentSize = 0; // Reset the size counter
}

template <typename K, typename V>
void HashTable<K, V>::rehash() {
    std::vector<std::list<std::pair<K, V>>> oldTable = table; // Copy current table

    // Resize the table to the next prime size larger than twice its current size
    table.resize(prime_below(2 * table.size()));
    for (auto &bucket : table) {
        bucket.clear(); // Clear each bucket in the resized table
    }
}

template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const {
    static std::hash<K> hashFunction;
    return hashFunction(k) % table.size(); // Compute index using the STL hash function
}

template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n) {
    if (n <= 1) return 0;
    std::vector<unsigned long> primes(n + 1, 1);
    setPrimes(primes);

    for (unsigned long i = n; i > 0; --i) {
        if (primes[i]) return i;
    }

    return 2; // Fallback for very small n
}

template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long> &primes) {
    primes[0] = primes[1] = 0; // 0 and 1 are not primes
    for (unsigned long i = 2; i * i < primes.size(); ++i) {
        if (primes[i]) {
            for (unsigned long j = i * i; j < primes.size(); j += i) {
                primes[j] = 0; // Mark multiples of i as non-prime
            }
        }
    }
}

} // namespace cop4530

#endif // HASHTABLE_HPP
