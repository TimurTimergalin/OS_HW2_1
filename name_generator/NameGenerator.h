#pragma once

#ifndef IHW2_1_NAMEGENERATOR_H
#define IHW2_1_NAMEGENERATOR_H

#include <string>
#include <sstream>

class NameGenerator {
private:
    int counter;
    std::string seed;
public:
    explicit NameGenerator(std::string  seed);
    std::string get();
};

class SepSStream {
private:
    std::stringstream ss{};
public:
    SepSStream& operator<<(const auto& o) {
        ss << o << '_';
        return *this;
    }

    std::string str() {
        return ss.str();
    }
};

template<typename... Args>
std::string join(const Args& ... args) {


    SepSStream ss{};
    return (ss << ... << args).str();
}

#endif //IHW2_1_NAMEGENERATOR_H
