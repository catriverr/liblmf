#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace LMF {
    enum type {
        ENCRYPTABLE, INTERLEAFED, HEADER,
        SCRIPT, MARKDOWN, TOMFOOLERY,
        CONFIG, SMALL_CONFIG, ARG_LIST
    };

    template<typename T>
        struct Hook {
            T* ptr;
        };

    template<typename T>
        struct Entry {
            char* key;
            int id;
            Hook<T> ptr;
        };


    template<LMF::type T>
        struct Data;

    template<>
        struct Data<type::ENCRYPTABLE> {
            std::vector<Entry<std::string>> entries;
            bool usable = false;
        };

    template<LMF::type T>
        class File;

    template<>
        class File<type::ENCRYPTABLE> {
          public:
            static const type T = type::ENCRYPTABLE;
            std::string path; LMF::Data<T> data;
            File(std::string filename) : path(filename) {};
            std::string get(char* key);
            int read();
        };
};
