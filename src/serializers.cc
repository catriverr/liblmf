#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>

/// EXTEND THIS FUNCTION
/// TO PROVIDE FUNCTIONAL
/// REFLECTION DATA.
template<typename T>
size_t serialize(T& obj) {
    return sizeof(obj);
};

template<typename T>
size_t serialize(T* obj) {
    return sizeof(obj);
};


// Specialization for std::string
template<>
size_t serialize<std::string>(std::string& str) {
    return sizeof(size_t) + str.size(); // Size of the string length + the string data itself
};

template<>
size_t serialize(char* str) {
    return strlen(str) + 1;
};


// Specialization for std::vector
template<typename T>
size_t serialize(const std::vector<T>& vec) {
    size_t total_size = sizeof(size_t);  // store the vector's size
    for (const auto& element : vec) {
        total_size += serialize(element);  // recursively call serialize for each element
    }
    return total_size;
};

// Specialization for std::map
template<typename K, typename V>
size_t serialize(const std::map<K, V>& map) {
    size_t total_size = sizeof(size_t);  // Size for the number of elements
    for (const auto& [key, value] : map) {
        total_size += calculate_size(key) + calculate_size(value);  // Add size of each key-value pair
    }
    return total_size;
};

// Serialize function for uint64_t
template<>
size_t serialize<uint64_t>(uint64_t& value) {
    return sizeof(uint64_t);
}

// Serialize function for uint32_t
template<>
size_t serialize<uint32_t>(uint32_t& value) {
    return sizeof(uint32_t);
}

// Serialize function for uint16_t
template<>
size_t serialize(uint16_t& value) {
    return sizeof(uint16_t);
}

// Serialize function for uint8_t
template<>
size_t serialize(uint8_t& value) {
    return sizeof(uint8_t);
}

// Serialize function for long
template<>
size_t serialize(long& value) {
    return sizeof(long);
}

// Serialize function for float
template<>
size_t serialize(float& value) {
    return sizeof(float);
}

// Serialize function for double
template<>
size_t serialize(double& value) {
    return sizeof(double);
}
