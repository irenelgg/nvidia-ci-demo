#include <cassert>
#include <stdexcept>
#include "math_ops.h"

int main() {
    assert(add(2, 3) == 5);
    assert(add(-1, 1) == 0);

    assert(divide(10, 2) == 5);
    assert(divide(9, 3) == 3);

    try {
        divide(1, 0);
        assert(false);
    } catch (const std::invalid_argument&) {}

    return 0;
}
