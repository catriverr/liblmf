#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "./strops.h"


template<typename From, typename To, typename = void>
struct is_convertible_to : std::false_type {};

template<typename From, typename To>
struct is_convertible_to<From, To, std::void_t<decltype(static_cast<To>(std::declval<From>()))>> : std::true_type {};


enum lexer_type {
    FILE_HEADER = 0,
    SCRIPT = 1,
    TEXT = 2,
    ENCRYPTED_TEXT = 3,
    MARKDOWN = 4,
    CONFIG = 5,
    INTERLEAF = 6
};

struct lexer_headers {
    bool pedantic = false;
    lexer_type type = TEXT;
    int mvlndx = 0;
};

template<typename T>
inline void set_value(T& obj, std::string _val);


template<>
inline void set_value<bool>(bool& obj, std::string _val) {
    obj = _val == "true";
};

template<>
inline void set_value<int>(int& obj, std::string _val) {
    obj = std::stoi(_val);
};

template<>
inline void set_value<char>(char& obj, std::string _val) {
    obj = _val[0];
};

static lexer_headers parse_headers(std::string line) {
    replace_all(line, " ", "");
    std::vector<std::string> data = split_string(line, ';');
    if (data.size() < 1 || line.length() < 4) throw std::invalid_argument("invalid header, line 1 of a .LMF file must be the header.");
    lexer_headers ldata;
    for (const auto& header : data) {
        std::vector<std::string> hands = split_string(header, '=');
        if (hands.size() < 2) throw std::invalid_argument("headers must be defined with 'KEY=VALUE' syntax");
        if (hands[0] == "pedantic") {
            set_value<bool>(ldata.pedantic, hands[1]);
        } else if (hands[0] == "type") {
            ldata.type = (lexer_type)std::stoi(hands[1]);
        } else if (hands[0] == "moveindex") {
            ldata.mvlndx = std::stoi(hands[1]);
        };
    };
    return ldata;
};

template<lexer_type T>
struct lexer_input_t {
    std::string data;
    lexer_headers headers;
};

template<lexer_type _T>
    class lexer_t {
      public:
        lexer_input_t<_T> input;
        void* data;
        lexer_t(std::string);
    };

template<>
lexer_t<lexer_type::TEXT>::lexer_t(std::string data) {
    std::vector<std::string> _data = split_string(data, '\n');
    if (_data.size() < 2) throw std::invalid_argument("LMF/Lexer::lexer_t<TEXT>::constructor: REQUIRES __DATA__ with line count more than 1.");
    std::string line_1 = _data[0];
    _data.erase(_data.begin());
    this->input.headers = parse_headers(line_1);
    this->input.data = join_string(_data, '\n');
};

static lexer_headers parse_interleaf_header(std::string line) {
    return (lexer_headers) {
        .pedantic=false,
        .type=lexer_type::INTERLEAF
    };
};

template<>
inline lexer_t<lexer_type::ENCRYPTED_TEXT>::lexer_t(std::string data) {
    std::vector<std::string> _data = split_string(data, '\n');
    if (_data.size() < 2) throw std::invalid_argument("LMF/Lexer::lexer_t<TEXT>::constructor: REQUIRES __DATA__ with line count more than 1.");
    std::string line_1 = _data[0];
    _data.erase(_data.begin());
    this->input.headers = parse_headers(line_1);
    this->input.data = join_string(_data, '\n');
};

template<>
inline lexer_t<lexer_type::INTERLEAF>::lexer_t(std::string data) {
    std::vector<std::string> _data = split_string(data, '\n');
    if (_data.size() < 2) throw std::invalid_argument("LMF/Lexer::lexer_t<INTERLEAF>::constructor: REQUIRES __DATA__ with line count more than 1.");
    std::string line_1 = _data[0];
    _data.erase(_data.begin());
    this->input.headers = parse_interleaf_header(line_1);
    this->input.data = join_string(_data, '\n');
};

template<lexer_type T>
static lexer_t<T>* create_lexer(std::string __content__) {
    lexer_t<T>* ob = new lexer_t<T>(__content__);
    return ob;
};
