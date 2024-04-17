#include "NameGenerator.h"

NameGenerator::NameGenerator(std::string seed) : counter(1), seed(std::move(seed)){}
std::string NameGenerator::get() {
    return join('/', seed, counter++);
}