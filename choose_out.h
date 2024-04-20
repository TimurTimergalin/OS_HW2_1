#ifndef IHW2_1_CHOOSE_OUT_H
#define IHW2_1_CHOOSE_OUT_H

#include <memory>
#include <iostream>
#include <fstream>

std::unique_ptr<std::ostream> choose_out(int argc, char *argv[]) {
    if (argc == 1) {
        return std::unique_ptr<std::ostream>(&std::cout);
    } else if (argc == 2) {
        std::unique_ptr<std::ostream> res = std::make_unique<std::ofstream>(argv[1]);
        return res;
    } else {
        throw std::runtime_error("Wrong arguments count");
    }
}

#endif //IHW2_1_CHOOSE_OUT_H
