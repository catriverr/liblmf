#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <fstream>
#include <sstream>

namespace LMF {
    enum type {
        ENCRYPTABLE, INTERLEAFED, HEADER,
        SCRIPT, MARKDOWN, TOMFOOLERY,
        CONFIG, SMALL_CONFIG, ARG_LIST
    };

    class InterleafDirectory {
      public:
        struct __contents__ {
            std::string name;
        };
        std::string name;
        InterleafDirectory(__contents__ data) {
            this->name = data.name;
        };
    };

    class InterleafedFile {
      public:
        struct __content__ {
            std::string name;
            std::string content;
            InterleafDirectory* parent;
        };
        std::string name;
        std::string content;
        InterleafDirectory* parent = nullptr;

        InterleafedFile(__content__ data) {
            this->name = data.name;
            this->content = data.content;
            this->parent = data.parent;
        };
    };

    template<typename T>
        struct Hook {
            T* ptr;
        };

    template<typename T>
        struct Entry {
            std::string key;
            int id;
            Hook<T> hook;
        };


    template<LMF::type T>

        struct Data {
            bool usable = false;
        };

    template<>
        struct Data<type::ENCRYPTABLE> {
            std::vector<Entry<std::string>> entries;
        };
    template<>
        struct Data<type::INTERLEAFED> {
            std::vector<Entry<LMF::InterleafedFile>> files;
        };


    template<LMF::type T>
        class File {
          public:
            std::string path; LMF::Data<T> data;
            File(std::string filename) : path(filename) {};
            std::string get(char* key);
            int read();
        };
};

static std::string read_file(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    std::ostringstream stringStream;
    stringStream << fileStream.rdbuf();

    return stringStream.str();
}

#define LMF_INIT

#include "./src/lexer.h"
#include "./src/strops.h"

#include "./src/lmf.cc"
