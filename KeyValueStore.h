#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include <string>
#include <vector>

struct Entry {
    std::string key;
    std::string value;
};

class KeyValueStore {

private:

    std::vector<Entry> entries;
    std::string filename;

public:

    KeyValueStore(const std::string& dbFile);
    void loadFromFile();
    void set(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& value) const;
};

#endif