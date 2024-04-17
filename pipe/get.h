#ifndef IHW2_1_QUEUE_GET_H
#define IHW2_1_QUEUE_GET_H

#include "base.h"

#include <string>
#include <memory>

template<class MT>
using Pipe = std::unique_ptr<pipes::Base<MT>>;

namespace pipes {
    template<class MT>
    Pipe<MT> get(const std::string&, bool new_ = false);
}

#endif //IHW2_1_QUEUE_GET_H
