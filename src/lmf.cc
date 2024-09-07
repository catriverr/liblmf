#pragma once
#include "lexer.h"
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <type_traits>
#include <filesystem>

#define fs std::filesystem


#include "../lmf.h"

using namespace LMF;

template<>
int LMF::File<ENCRYPTABLE>::read() {
    if (!fs::exists(this->path)) return 1;
    std::string data = read_file(this->path);
    lexer_t<ENCRYPTED_TEXT>* lexer = create_lexer<ENCRYPTED_TEXT>(data);
    if (lexer->input.headers.mvlndx < 0 ||
        lexer->input.headers.mvlndx > 15000 ||
        (int)lexer->input.headers.type < 0) return 1;
    std::string rein_data = "";
    for (const char& c : data) {
        rein_data += (char)( ((int)c)-(lexer->input.headers.mvlndx) );
    };
    int i = this->data.entries.size() > 0 ? this->data.entries.size()-1 : 0;
    for (std::string& d : split_string(rein_data, '\n')) {
        this->data.entries.push_back((Entry<std::string>) {
                .key = std::to_string(i) + "_LINE",
                .id = i,
                .hook = {
                    .ptr = &d
                },
        });
        i++;
    };
    return 0;
};

template<>
std::string LMF::File<INTERLEAFED>::get(char* key) {
    int b = -1;
    std::vector<std::string> closest_matches;
    for (auto v : this->data.files) {
        if (v.key == key) {
            return v.hook.ptr->content; break;
        }
        else if ( string_includes(v.key, key) && v.key != key ) {
            b = 0;
            closest_matches.push_back(v.key);
        };
    };
    if (b == 0) {
        std::string matches_str = "";
        for (auto& match : closest_matches) {
            matches_str += match + ",";
        };
        return "$!__CLOSE_MATCHES\n" + matches_str;
    } else return "$!__NO_CONTENT";
};

template<>
int LMF::File<INTERLEAFED>::read() {
    if (!fs::exists(this->path)) return 1;
    std::string data = read_file(this->path);
    lexer_t<INTERLEAF>* lexer = create_lexer<INTERLEAF>(data);
    if (lexer->input.headers.mvlndx < 0 ||
        lexer->input.headers.mvlndx > 15000 ||
        (int)lexer->input.headers.type < 0) return 1;
    std::string rein_data = "";
    for (const char& c : data) {
        rein_data += (char)( ((int)c)-(lexer->input.headers.mvlndx) );
    };
    Entry<LMF::InterleafedFile>* current_file = nullptr;
    LMF::InterleafDirectory* current_directory = nullptr;
    int i = 0; // LMF file length
    int j = 0; // file count
    int f_i = 0; // current file length

    //

    for (std::string& line : split_string(rein_data, '\n')) {
        std::string text = trim(line);
        if ( current_file == nullptr && text.length() < 1 ) { i++; continue; }

        else if ( starts_with(text, "#DIRECTORY") ) {
            if ( text == "#DIRECTORY" ) throw std::invalid_argument("#DIRECTORY keyword requires an argument ( " + path + ":" + std::to_string(i) + ")");
            else if ( current_directory != nullptr ) throw std::invalid_argument("#DIRECTORY keyword cannot be recursively nested, close the directory with #END_DIRECTORY first. ( " + path + ":" + std::to_string(i) + ")");
            replace_all(text, " ", "");
            std::string dir_name = text.substr(10); // 10 is #DIRECTORY keyword's length.
            current_directory = new LMF::InterleafDirectory({ .name = dir_name });
        }

        else if ( starts_with(text, "#END_DIRECTORY") ) {
            if (current_directory == nullptr) throw std::invalid_argument("cannot END_DIRECTORY: not inside directory");
            if (current_file != nullptr) throw std::invalid_argument("cannot END_DIRECTORY: a file ('" + current_file->key + "') is open. #END_FILE to end the file first.");
            current_directory = nullptr;
        }

        // FILE

        else if ( starts_with(text, "#FILE") ) {
            if ( text == "#FILE" )  throw std::invalid_argument("#FILE keyword requires an argument ( " + path + ":" + std::to_string(i) + ")");
            if ( current_file != nullptr ) throw std::invalid_argument("#FILE keyword cannot be recursively nested, end the current file with #END_FILE first.");
            std::string file_name = text.substr(6); // 6 is #FILE keyword's length and ' ' at the end.
            std::string key = (current_directory != nullptr ? current_directory->name+"/" : "") + file_name;
            current_file = new Entry<LMF::InterleafedFile>({
                    .key = key,
                    .id = j,
                    .hook = {
                    .ptr = new InterleafedFile({
                                .name = file_name,
                                .content = "",
                                .parent = current_directory
                            })
                    }
            });
            j++;
        }

        else if (starts_with(text, "#END_FILE")) {
            f_i = 0;
            this->data.files.push_back(*current_file);
            current_file = nullptr;
        }

        else if (starts_with(text, "#$!")) { i++; continue; } // LMF comments

        else {
            if (current_file == nullptr) { i++; continue; };
            current_file->hook.ptr->content += text;
            f_i++;
        };


        i++;
    };

    if (current_directory != nullptr) throw std::invalid_argument("read() failed: current directory was not closed at the end of the file");
    if (current_file != nullptr) throw std::invalid_argument("read() failed: current file was not closed at the end of the file");

    return 0;
};


#define LMF_SOURCE_INIT
