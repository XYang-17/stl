#ifndef _YXXX_CONTAINER_BASE_H_
#define _YXXX_CONTAINER_BASE_H_

#include <stdexcept>
#include <initializer_list>
#include "../allocator/allocator.h"
#include "../iterator.h"

namespace stl{
    class bad_assign: std::runtime_error{};
};

#endif