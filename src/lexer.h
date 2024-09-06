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
    FILE_HEADER,
    SCRIPT,
    TEXT,
    ENCRYPTED_TEXT,
    MARKDOWN,
    CONFIG,
    INTERLEAF
};

struct lexer_headers {
    bool pedantic = false;
    lexer_type type = TEXT;
};

template<typename T>
void set_value(T& obj, std::string _val) {
    std::string val = trim(_val);
    if (!std::is_convertible<std::string,T>::value) throw std::invalid_argument("cannot convert value to pointer object type");
    if (val == "true" || val == "false") { obj = val == "true"; return; }
    else if (std::is_same<T, std::string>::value) { obj = val.c_str(); }
    else if (std::is_same<T, int>::value) {
        obj = std::stoi(val);
    };
};

lexer_headers parse_headers(std::string line) {
    replace_all(line, " ", "");
    std::vector<std::string> data = split_string(line, ';');
    if (data.size() < 1 || line.length() < 4) throw std::invalid_argument("invalid header, line 1 of a .LMF file must be the header.");
    lexer_headers ldata;
    for (const auto& header : data) {
        std::vector<std::string> hands = split_string(header, '=');
        if (hands.size() < 2) throw std::invalid_argument("headers must be defined with 'KEY=VALUE' syntax");
        if (hands[0] == "pedantic") {
            set_value(ldata.pedantic, hands[1]);
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
