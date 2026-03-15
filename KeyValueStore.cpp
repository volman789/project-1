#include "KeyValueStore.h"
#include <fstream>
#include <sstream>
#include <iostream>

KeyValueStore::KeyValueStore(const std::string& dbFile)
    : filename(dbFile) {}

 void KeyValueStore::loadFromFile() {

    entries.clear();

    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        return;
    }

    std::string line;

    // Replay every command in append-only log
    while (std::getline(inFile, line)) {

        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty())
            continue;

        std::istringstream iss(line);

        std::string command;
        std::string key;
        std::string value;

        // Parse the log entry
        iss >> command >> key;
        std::getline(iss, value);

        if (!value.empty() && value[0] == ' ')
            value.erase(0, 1);

        if (!value.empty() && value.back() == '\r')
            value.pop_back();

        // Rebuild the key-value store
        if (command == "SET") {

            bool updated = false;

            // Check if key already exists
            for (auto &entry : entries) {
                if (entry.key == key) {
                    entry.value = value;
                    updated = true;
                    break;
                }
            }

            // If key does not exist insert new entry
            if (!updated) {
                Entry e;
                e.key = key;
                e.value = value;
                entries.push_back(e);
            }
        }
    }
}

void KeyValueStore::set(const std::string& key, const std::string& value) {

    // Append operation to disk log
    std::ofstream outFile(filename, std::ios::app);

    if (!outFile.is_open()) {
        std::cerr << "ERROR: Could not open database file\n";
        return;
    }

    // Write operation to the append-only log
    outFile << "SET " << key << " " << value << "\n";
    outFile.close();

    // Update the in-memory store
    for (auto &entry : entries) {
        if (entry.key == key) {
            entry.value = value;
            return;
        }
    }

    // If key does not exis insert new entry
    Entry e;
    e.key = key;
    e.value = value;
    entries.push_back(e);
}

bool KeyValueStore::get(const std::string& key, std::string& value) const {

    for (const auto &entry : entries) {
        if (entry.key == key) {
            value = entry.value;
            return true;
        }
    }

    return false;
}