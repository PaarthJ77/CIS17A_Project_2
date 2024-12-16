#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <iostream>
#include <vector>

// Template function to display elements in a vector
template <typename T>
void displayVector(const std::vector<T>& vec) {
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";
}

#endif // TEMPLATES_H
