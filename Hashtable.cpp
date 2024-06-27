#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <stdexcept>

class HashTable {
private:
    // A node to store key-value pairs
    struct Node {
        std::string key;
        int value;
        Node(std::string k, int v) : key(k), value(v) {}
    };

    // Vector of lists to handle chaining
    std::vector<std::list<Node>> table;
    int numBuckets; // Number of buckets in hash table
    int size;       // Number of elements in hash table

    // Polynomial Rolling Hash function using Horner's rule
    int hashFunction(const std::string &key) {
        const int p = 31;
        const int m = numBuckets;
        long long hashValue = 0;
        long long p_pow = 1;
        
        for (char c : key) {
            hashValue = (hashValue + (c - 'a' + 1) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }
        
        return hashValue % m;
    }

public:
    // Constructor to initialize hash table
    HashTable(int buckets = 101) : numBuckets(buckets), size(0) {
        table.resize(numBuckets);
    }

    // Insert key-value pair into hash table
    void insert(const std::string &key, int value) {
        int index = hashFunction(key);
        for (auto &node : table[index]) {
            if (node.key == key) {
                node.value = value;
                return;
            }
        }
        table[index].emplace_back(key, value);
        size++;
    }

    // Search for a value by key in hash table
    int search(const std::string &key) {
        int index = hashFunction(key);
        for (const auto &node : table[index]) {
            if (node.key == key) {
                return node.value;
            }
        }
        throw std::runtime_error("Key not found");
    }

    // Update value of an existing key in hash table
    void update(const std::string &key, int newValue) {
        int index = hashFunction(key);
        for (auto &node : table[index]) {
            if (node.key == key) {
                node.value = newValue;
                return;
            }
        }
        throw std::runtime_error("Key not found");
    }

    // Delete key-value pair from hash table
    void remove(const std::string &key) {
        int index = hashFunction(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                table[index].erase(it);
                size--;
                return;
            }
        }
        throw std::runtime_error("Key not found");
    }

    // Get current size of hash table
    int getSize() const {
        return size;
    }
};

int main() {
    try {
        HashTable ht;

        ht.insert("apple", 5);
        ht.insert("banana", 10);
        ht.insert("orange", 15);

        std::cout << "Value for 'apple': " << ht.search("apple") << std::endl;
        std::cout << "Value for 'banana': " << ht.search("banana") << std::endl;

        ht.update("apple", 20);
        std::cout << "Updated value for 'apple': " << ht.search("apple") << std::endl;

        ht.remove("banana");
        std::cout << "Removed 'banana'. Size: " << ht.getSize() << std::endl;

        // Uncomment the next line to see the exception handling in action
        // std::cout << "Value for 'banana': " << ht.search("banana") << std::endl;

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
