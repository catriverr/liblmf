#pragma once
#include <cstddef>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <iostream>




#include "serializers.cc"

template<typename T>
int write_binary(const char* filename, T& data) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) return 1;

    size_t size = serialize<T>(data);

    std::cout << size << " write_bin\n";

    char* buffer = new char[size];
    std::memcpy(buffer, &data, size);

    outFile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    outFile.write(buffer, size);
    outFile.close();

    return 0;
};

template<typename T>
T* read_binary(const char* filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) return nullptr;

    size_t size = 0;


    inFile.read(reinterpret_cast<char*>(&size), sizeof(size_t));

    std::cout << size << " read_bin\n";


    T* obj = static_cast<T*>(malloc(size));
    if (!obj) return nullptr;

    inFile.read(reinterpret_cast<char*>(obj), size);
    inFile.close();

    return obj;

};
