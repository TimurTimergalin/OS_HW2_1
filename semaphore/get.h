#ifndef IHW2_1_SEMAPHORE_GET_H
#define IHW2_1_SEMAPHORE_GET_H

#include "base.h"

#include <memory>
#include <string>

using Semaphore = std::unique_ptr<sem::Base>;

namespace sem {
    Semaphore get(const std::string& name, int init, bool new_ = false);
}

#endif //IHW2_1_SEMAPHORE_GET_H
