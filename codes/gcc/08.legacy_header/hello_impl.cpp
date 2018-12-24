module;
#include <iostream>
module hello;
import "legacy.hpp";

#if defined(LEGACY)
void func() {
    ::legacy();
    std::cout << "hello legacy!" << std::endl;
}
#endif

namespace hello {
    void say_hello() {
#   if defined(LEGACY)
        ::func();
#   endif
    }
}
