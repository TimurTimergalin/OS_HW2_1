#ifndef IHW2_1_GET_H
#define IHW2_1_GET_H

#include <string>
#include <memory>

#include "base.h"

template<class T>
using SharedMemory = std::unique_ptr<shm::Base<T>>;

namespace shm {
    template<class T>
    SharedMemory<T> get(const std::string& name);
}

#endif //IHW2_1_GET_H
