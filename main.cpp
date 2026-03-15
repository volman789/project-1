#include <iostream>
#include <sstream>
#include <string>
#include "KeyValueStore.h"

int main() {

    // Create the key-value store object and specify the file used for persistence
    KeyValueStore store("data.db");
    store.loadFromFile();

    std::string line;

    // Read commands from stdin
    while (std::getline(std::cin, line)) {

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        // Parse the command
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "SET") {

            std::string key;
            std::string value;

            // Read the key
            iss >> key;
            std::getline(iss, value);
            if (!value.empty() && value[0] == ' ')
                value.erase(0, 1);

            if (!value.empty() && value.back() == '\r')
                value.pop_back();

            // If no key print error
            if (key.empty()) {
                std::cout << "ERROR" << std::endl;
                continue;
            }

            // Store the key-value pair
            store.set(key, value);
        }

        else if (command == "GET") {

            std::string key;
            iss >> key;

            if (!key.empty() && key.back() == '\r')
                key.pop_back();

            // If  no key print error
            if (key.empty()) {
                std::cout << "ERROR" << std::endl;
                continue;
            }

            std::string value;

            // Print the stored value
            if (store.get(key, value)) {
                std::cout << value << std::endl;
            }
        }

        else if (command == "EXIT") {
            break;
        }
        else {
            std::cout << "ERROR" << std::endl;
        }
    }

    return 0;
}