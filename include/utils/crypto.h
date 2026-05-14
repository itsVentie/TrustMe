#pragma once
#include <string>
#include <vector>

inline std::string decrypt(const std::vector<unsigned char>& data, char key) {
    std::string output;
    for (auto b : data) output += b ^ key;
    return output;
}