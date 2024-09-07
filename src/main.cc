#include <iostream>
#include <string>

#include "../lmf.h"
#include "lmf.cc"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "\tusage: " << argv[0] << " <file>.lmf [interleaf_file_name]\n";
        return 1;
    };
    std::string dat = argv[1];
    if (!fs::exists(dat)) {
        std::cout << "\tLMF file not found: " << dat << '\n';
        return 2;
    };
    LMF::File<LMF::type::INTERLEAFED> file(dat);
    file.read();
    if (argc > 2) {
        char* ifname = argv[2];
        std::string data = file.get(ifname);
        if (data == "$!__NO_CONTENT") { std::cout << "could not find file " << ifname << " in interleafed LMF " << dat << ".\n"; return 1; };
        if ( starts_with(data, "$!__CLOSE_MATCHES\n") ) {
            std::string frfr = data.substr(18); // length of data, '\n' is 2 chars so remove 1.
            replace_all(frfr, ",", ", ");
            frfr.pop_back(); frfr.pop_back();
            std::cout << "could not find " << ifname << " exactly, did you mean:\n\t" << frfr << "?\n";
            return 4;
        };
        std::cout << '\t' << ifname << '\n';
        std::cout << data << "\n\n";
        return 0;
    };
    std::cout << dat << ": file directory listing (" << file.data.files.size() << " file" << (file.data.files.size() > 1 ? "s" : "") << "):\n";
    for (const auto& f : file.data.files) {
        auto* d = f.hook.ptr;
        std::cout << '\t';
        if (d->parent != nullptr) std::cout << d->parent->name << "/";
        std::cout << d->name + ":\n";
        std::cout << d->content + "\n\n";
    };
    return 0;
};
