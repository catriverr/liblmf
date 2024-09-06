#pragma once
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
    return 0;
};

#define LMF_SOURCE_INIT
